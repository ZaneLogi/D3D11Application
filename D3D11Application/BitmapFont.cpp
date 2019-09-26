#include <fstream>
#include "BitmapFont.h"
#include "DDSTextureLoader.h"

bool SaveBitmapImage(const BITMAPINFOHEADER& bi, void* bits, LPCWSTR filename)
{
    BITMAPFILEHEADER   bmfHeader;
    DWORD dwBmpSize = ((bi.biWidth * bi.biBitCount + 31) / 32) * 4 * abs(bi.biHeight);

    // A file is created, this is where we will save the screen capture.
    HANDLE hFile = CreateFile(filename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);

    // Add the size of the headers to the size of the bitmap to get the total file size
    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    //Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    //Size of the file
    bmfHeader.bfSize = dwSizeofDIB;

    //bfType must always be BM for Bitmaps
    bmfHeader.bfType = 0x4D42; //BM

    DWORD dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)bits, dwBmpSize, &dwBytesWritten, NULL);

    //Close the handle for the file that was created
    CloseHandle(hFile);

    return true;
}

bool BitmapFont::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* fontFilename, const char* textureFilename)
{
    bool result;

    // Load in the text file containing the font data.
    result = LoadFontData(fontFilename);
    if (!result)
    {
        return false;
    }

    // Load the texture that has the font characters on it.
    result = LoadTexture(device, textureFilename);
    if (!result)
    {
        return false;
    }

    CreateFontTexture(device, deviceContext);

    return true;
}

void BitmapFont::shutdown()
{
    // Release the font texture.
    ReleaseTexture();

    // Release the font data.
    ReleaseFontData();

    ReleaseFontTexture();
}

bool BitmapFont::CreateFontTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    HDC hdc = ::CreateCompatibleDC(NULL);

    //Logical units are device dependent pixels, so this will create a handle to a logical font that is 48 pixels in height.
    //The width, when set to 0, will cause the font mapper to choose the closest matching value.
    //The font face name will be Impact.
    HFONT hFont = CreateFont(32, // height
        0, // width
        0, // escapement
        0, // orientation
        FW_DONTCARE, // weight
        FALSE, // italic
        FALSE, // underline
        FALSE, // strikeout
        DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, // out precision
        CLIP_DEFAULT_PRECIS, // clip precision
        CLEARTYPE_QUALITY, // quality
        VARIABLE_PITCH, // pitch and family
        TEXT("Arial")); // face name
    auto hOldFont = ::SelectObject(hdc, hFont);

    TEXTMETRIC tm;
    ::GetTextMetrics(hdc, &tm);
    m_char_width = tm.tmMaxCharWidth;
    m_char_height = tm.tmHeight;
    m_dib_width = m_char_width * 16;
    m_dib_height = m_char_height * 16;
    int stride = ((((m_dib_width * 32) + 31) & ~31) >> 3);

    BITMAPINFOHEADER bmih =
    {
        sizeof(BITMAPINFOHEADER),
        m_dib_width,
        -m_dib_height, // minus means top-down bitmap
        1, // plane
        32, // bit count
        BI_RGB, // compression
        0, // the size of the image
        0,
        0,
        0,
        0
    };
    PBYTE bits = nullptr;
    HBITMAP hBitmap = ::CreateDIBSection(hdc, (const BITMAPINFO*)&bmih, DIB_RGB_COLORS, (void **)&bits, NULL, 0);

    auto hOldBitmap = ::SelectObject(hdc, hBitmap);

    ::PatBlt(hdc, 0, 0, m_dib_width, m_dib_height, BLACKNESS);
    ::SetTextColor(hdc, RGB(255, 255, 255));
    ::SetBkColor(hdc, RGB(0, 0, 0));

    for (int i = 0; i < 256; i++)
    {
        int x = (i % 16) * m_char_width;
        int y = (i / 16) * m_char_height;
        ::TextOutA(hdc, x, y, (PCSTR)&i, 1);
    }

    GetCharABCWidths(hdc, 0, 255, m_abc);

    //SaveBitmapImage(bmih, bits, L"\\save_image.bmp");

    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = m_dib_width;
    textureDesc.Height = m_dib_height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    if (FAILED(device->CreateTexture2D(&textureDesc, NULL, &m_pFontTexture)))
    {
        return false;
    }

    deviceContext->UpdateSubresource(m_pFontTexture, 0, NULL, bits, stride, 0);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1;

    // Create the shader resource view for the texture.
    if (FAILED(device->CreateShaderResourceView(m_pFontTexture, &srvDesc, &m_pFontTextureView)))
    {
        return false;
    }

    ::SelectObject(hdc, hOldBitmap);
    ::DeleteObject(hBitmap);
    ::SelectObject(hdc, hOldFont);
    ::DeleteObject(hFont);
    ::DeleteDC(hdc);
    return true;
}

void BitmapFont::ReleaseFontTexture()
{
    if (m_pFontTexture)
    {
        m_pFontTexture->Release();
        m_pFontTexture = nullptr;
    }

    if (m_pFontTextureView)
    {
        m_pFontTextureView->Release();
        m_pFontTextureView = nullptr;
    }
}

bool BitmapFont::LoadFontData(const char* filename)
{
    std::ifstream fin;
    int i;
    char temp;

    // Create the font spacing buffer.
    m_Font = new FontType[95];
    if (!m_Font)
    {
        return false;
    }

    // Read in the font size and spacing between chars.
    fin.open(filename);
    if (fin.fail())
    {
        return false;
    }

    // Read in the 95 used ascii characters for text.
    for (i = 0; i<95; i++)
    {
        fin.get(temp);
        while (temp != ' ')
        {
            fin.get(temp);
        }
        fin.get(temp);
        while (temp != ' ')
        {
            fin.get(temp);
        }

        fin >> m_Font[i].left;
        fin >> m_Font[i].right;
        fin >> m_Font[i].size;
    }

    // Close the file.
    fin.close();

    return true;
}

void BitmapFont::ReleaseFontData()
{
    // Release the font data array.
    if (m_Font)
    {
        delete[] m_Font;
        m_Font = nullptr;
    }
}

bool BitmapFont::LoadTexture(ID3D11Device* device, const char* filename)
{
    char* buffer = nullptr;
    long buffer_size = 0;

    std::ifstream f(filename, std::ios_base::in | std::ios_base::binary);
    f.seekg(0, std::ios_base::end);
    buffer_size = (long)f.tellg();
    f.seekg(0, std::ios_base::beg);

    buffer = new char[buffer_size];
    if (!buffer)
    {
        return false;
    }

    f.read(buffer, buffer_size);
    f.close();

    CreateDDSTextureFromMemory(device,
        (const byte *)buffer,
        buffer_size,
        &m_pTexture,
        &m_pTextureView);

    delete buffer;
    buffer = nullptr;

    return m_pTexture != nullptr && m_pTextureView != nullptr;
}

void BitmapFont::ReleaseTexture()
{
    // Release the texture object.
    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = nullptr;
    }

    if (m_pTextureView)
    {
        m_pTextureView->Release();
        m_pTextureView = nullptr;
    }
}

ID3D11ShaderResourceView* BitmapFont::GetTexture()
{
    return m_pTextureView;
}

ID3D11ShaderResourceView* BitmapFont::GetTexture2()
{
    return m_pFontTextureView;
}

void BitmapFont::build_vertex_array(void* vertices, char* sentence, float drawX, float drawY)
{
    VertexType* vertexPtr;
    int numLetters, index, i, letter;


    // Coerce the input vertices into a VertexType structure.
    vertexPtr = (VertexType*)vertices;

    // Get the number of letters in the sentence.
    numLetters = (int)strlen(sentence);

    // Initialize the index to the vertex array.
    index = 0;

    // Draw each letter onto a quad.
    for (i = 0; i<numLetters; i++)
    {
        letter = ((int)sentence[i]) - 32;

        // If the letter is a space then just move over three pixels.
        if (letter == 0)
        {
            drawX = drawX + 3.0f;
        }
        else
        {
            // First triangle in quad.
            vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
            index++;

            // Second triangle in quad.
            vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
            vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
            index++;

            // Update the x location for drawing by the size of the letter and one pixel.
            drawX = drawX + m_Font[letter].size + 1.0f;
        }
    }
}

void BitmapFont::build_vertex_array2(void* vertices, char* sentence, float drawX, float drawY)
{
    VertexType* vertexPtr;
    int numLetters, index;


    // Coerce the input vertices into a VertexType structure.
    vertexPtr = (VertexType*)vertices;

    // Get the number of letters in the sentence.
    numLetters = (int)strlen(sentence);

    // Initialize the index to the vertex array.
    index = 0;

    // Draw each letter onto a quad.
    for (int i = 0; i < numLetters; i++)
    {
        auto letter = sentence[i];
        float bmp_x = (float)(letter % 16) * m_char_width;
        float bmp_y = (float)(letter / 16) * m_char_height;

        PABC char_abc = &m_abc[letter];
        int char_w = char_abc->abcA + char_abc->abcB + char_abc->abcC;

        float u0 = bmp_x / m_dib_width;
        float v0 = bmp_y / m_dib_height;
        float u1 = (bmp_x + char_w) / m_dib_width;
        float v1 = (bmp_y + m_char_height) / m_dib_height;

        // the world matrix is the identity matrix
        // the view matrix is the identity matrix, this means that the camera is position at (0,0,0), looks at (0,0,1), up vector is (0,1,0)
        // the project matrix is the orthogonal matrix, near plan = 1.0f, far plan = 1000.0f
        // set Z = 10.0f to make the triangle visible by the camera
        const float Z = 10.0f;

        // First triangle in quad.
        vertexPtr[index].position = XMFLOAT3(drawX, drawY, Z);  // Top left.
        vertexPtr[index].texture = XMFLOAT2(u0, v0);
        index++;

        vertexPtr[index].position = XMFLOAT3((drawX + char_w), (drawY - m_char_height), Z);  // Bottom right.
        vertexPtr[index].texture = XMFLOAT2(u1, v1);
        index++;

        vertexPtr[index].position = XMFLOAT3(drawX, (drawY - m_char_height), Z);  // Bottom left.
        vertexPtr[index].texture = XMFLOAT2(u0, v1);
        index++;

        // Second triangle in quad.
        vertexPtr[index].position = XMFLOAT3(drawX, drawY, Z);  // Top left.
        vertexPtr[index].texture = XMFLOAT2(u0, v0);
        index++;

        vertexPtr[index].position = XMFLOAT3(drawX + char_w, drawY, Z);  // Top right.
        vertexPtr[index].texture = XMFLOAT2(u1, v0);
        index++;

        vertexPtr[index].position = XMFLOAT3((drawX + char_w), (drawY - m_char_height), Z);  // Bottom right.
        vertexPtr[index].texture = XMFLOAT2(u1, v1);
        index++;

        // Update the x location for drawing by the size of the letter and one pixel.
        drawX = drawX + char_w + 1.0f;
    }
}
