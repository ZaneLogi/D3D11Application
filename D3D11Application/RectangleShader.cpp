#include "RectangleShader.h"
#include <d3dcompiler.h>
#include <fstream>

extern const char VertexShaderText[];
extern const char PixelShaderText[];
extern const int VertexShaderTextLength;
extern const int PixelShaderTextLength;

bool RectangleShader::initialize(ID3D11Device* device, HWND hwnd)
{
    // Initialize the vertex and pixel shaders.
    return initialize_shader(device, hwnd);
}

void RectangleShader::shutdown()
{
    // Shutdown the vertex and pixel shaders as well as the related objects.
    shutdown_shader();
}

bool RectangleShader::render(
    ID3D11DeviceContext* deviceContext,
    int indexCount,
    const XMMATRIX& worldMatrix,
    const XMMATRIX& viewMatrix,
    const XMMATRIX& projectionMatrix)
{
    // Set the shader parameters that it will use for rendering.
    if (!set_shader_parameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix))
    {
        return false;
    }

    // Now render the prepared buffers with the shader.
    render_shader(deviceContext, indexCount);

    return true;
}

bool RectangleShader::initialize_shader(ID3D11Device* device, HWND hwnd)
{
    HRESULT result;
    ID3D10Blob* errorMessage;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc;


    // Initialize the pointers this function will use to null.
    errorMessage = 0;
    vertexShaderBuffer = 0;
    pixelShaderBuffer = 0;

    // Compile the vertex shader code.
    result = D3DCompile(VertexShaderText, VertexShaderTextLength, NULL, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
        &vertexShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if (errorMessage)
        {
            output_shader_error_message(errorMessage, hwnd, L"vertext shader");
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(hwnd, L"vertext shader", L"Missing Shader File", MB_OK);
        }

        return false;
    }

    // Compile the pixel shader code.
    result = D3DCompile(PixelShaderText, PixelShaderTextLength, NULL, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
        &pixelShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if (errorMessage)
        {
            output_shader_error_message(errorMessage, hwnd, L"pixel shader");
        }
        // If there was nothing in the error message then it simply could not find the file itself.
        else
        {
            MessageBox(hwnd, L"pixel shader", L"Missing Shader File", MB_OK);
        }

        return false;
    }

    // Create the vertex shader from the buffer.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
    if (FAILED(result))
    {
        return false;
    }

    // Create the pixel shader from the buffer.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
    if (FAILED(result))
    {
        return false;
    }

    // Create the vertex input layout description.
    // This setup needs to match the VertexType stucture in the ModelClass and in the shader.
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "COLOR";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    // Get a count of the elements in the layout.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // Create the vertex input layout.
    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result))
    {
        return false;
    }

    // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
    vertexShaderBuffer->Release();
    vertexShaderBuffer = 0;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = 0;

    // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void RectangleShader::shutdown_shader()
{
    // Release the matrix constant buffer.
    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = nullptr;
    }

    // Release the layout.
    if (m_layout)
    {
        m_layout->Release();
        m_layout = nullptr;
    }

    // Release the pixel shader.
    if (m_pixelShader)
    {
        m_pixelShader->Release();
        m_pixelShader = nullptr;
    }

    // Release the vertex shader.
    if (m_vertexShader)
    {
        m_vertexShader->Release();
        m_vertexShader = nullptr;
    }
}

void RectangleShader::output_shader_error_message(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
    char* compileErrors;
    unsigned long long bufferSize, i;
    std::ofstream fout;

    // Get a pointer to the error message text buffer.
    compileErrors = (char*)(errorMessage->GetBufferPointer());

    // Get the length of the message.
    bufferSize = errorMessage->GetBufferSize();

    // Open a file to write the error message to.
    fout.open("shader-error.txt");

    // Write out the error message.
    for (i = 0; i<bufferSize; i++)
    {
        fout << compileErrors[i];
    }

    // Close the file.
    fout.close();

    // Release the error message.
    errorMessage->Release();
    errorMessage = 0;

    // Pop a message up on the screen to notify the user to check the text file for compile errors.
    MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
}

bool RectangleShader::set_shader_parameters(ID3D11DeviceContext* deviceContext,
    const XMMATRIX& worldMatrix,
    const XMMATRIX& viewMatrix,
    const XMMATRIX& projectionMatrix)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    unsigned int bufferNumber;

    // Transpose the matrices to prepare them for the shader.
    auto worldMatrix1 = XMMatrixTranspose(worldMatrix);
    auto viewMatrix1 = XMMatrixTranspose(viewMatrix);
    auto projectionMatrix1 = XMMatrixTranspose(projectionMatrix);

    // Lock the constant buffer so it can be written to.
    result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // Get a pointer to the data in the constant buffer.
    dataPtr = (MatrixBufferType*)mappedResource.pData;

    // Copy the matrices into the constant buffer.
    dataPtr->world = worldMatrix1;
    dataPtr->view = viewMatrix1;
    dataPtr->projection = projectionMatrix1;

    // Unlock the constant buffer.
    deviceContext->Unmap(m_matrixBuffer, 0);

    // Set the position of the constant buffer in the vertex shader.
    bufferNumber = 0;

    // Finanly set the constant buffer in the vertex shader with the updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

    return true;
}

void RectangleShader::render_shader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    // Set the vertex input layout.
    deviceContext->IASetInputLayout(m_layout);

    // Set the vertex and pixel shaders that will be used to render this triangle.
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);

    // Render the triangle.
    deviceContext->DrawIndexed(indexCount, 0, 0);
}


const char VertexShaderText[] =
"\
cbuffer MatrixBuffer { matrix worldMatrix; matrix viewMatrix; matrix projectionMatrix; };\n\
struct VertexInputType { float4 position : POSITION; float4 color : COLOR; };\n\
struct PixelInputType { float4 position : SV_POSITION; float4 color : COLOR; };\n\
PixelInputType ColorVertexShader(VertexInputType input)\n\
{\n\
    PixelInputType output;\n\
\n\
    input.position.w = 1.0f;\n\
\n\
    output.position = mul(input.position, worldMatrix);\n\
    output.position = mul(output.position, viewMatrix);\n\
    output.position = mul(output.position, projectionMatrix);\n\
\n\
    output.color = input.color;\n\
\n\
    return output;\n\
}\0";

const int VertexShaderTextLength = sizeof(VertexShaderText);


const char PixelShaderText[] =
"\
struct PixelInputType { float4 position : SV_POSITION; float4 color : COLOR; };\n\
float4 ColorPixelShader(PixelInputType input) : SV_TARGET\n\
{\n\
    return input.color;\n\
}\0";

const int PixelShaderTextLength = sizeof(PixelShaderText);
