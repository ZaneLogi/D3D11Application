#include "LightModel.h"

LightModel::VertexType LightModel::s_box_vertices[] =
{
    {{-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}},
    {{ 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}},
    {{ 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f}, { 0.0f,  0.0f, -1.0f}},
    {{ 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f}, { 0.0f,  0.0f, -1.0f}},
    {{ 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}},
    {{ 1.0f,  1.0f,  1.0f}, {1.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}},
    {{ 1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}},
    {{ 1.0f, -1.0f, -1.0f}, {0.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}},
    {{ 1.0f,  1.0f,  1.0f}, {1.0f, 0.0f}, { 1.0f,  0.0f,  0.0f}},
    {{ 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f}, { 1.0f,  0.0f,  0.0f}},
    {{ 1.0f,  1.0f,  1.0f}, {0.0f, 0.0f}, { 0.0f,  0.0f,  1.0f}},
    {{-1.0f,  1.0f,  1.0f}, {1.0f, 0.0f}, { 0.0f,  0.0f,  1.0f}},
    {{ 1.0f, -1.0f,  1.0f}, {0.0f, 1.0f}, { 0.0f,  0.0f,  1.0f}},
    {{ 1.0f, -1.0f,  1.0f}, {0.0f, 1.0f}, { 0.0f,  0.0f,  1.0f}},
    {{-1.0f,  1.0f,  1.0f}, {1.0f, 0.0f}, { 0.0f,  0.0f,  1.0f}},
    {{-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f}, { 0.0f,  0.0f,  1.0f}},
    {{-1.0f,  1.0f,  1.0f}, {0.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}},
    {{-1.0f,  1.0f, -1.0f}, {1.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}},
    {{-1.0f, -1.0f,  1.0f}, {0.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}},
    {{-1.0f, -1.0f,  1.0f}, {0.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}},
    {{-1.0f,  1.0f, -1.0f}, {1.0f, 0.0f}, {-1.0f,  0.0f,  0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f,  0.0f,  0.0f}},
    {{-1.0f,  1.0f,  1.0f}, {0.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
    {{ 1.0f,  1.0f,  1.0f}, {1.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
    {{-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}},
    {{-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}},
    {{ 1.0f,  1.0f,  1.0f}, {1.0f, 0.0f}, { 0.0f,  1.0f,  0.0f}},
    {{ 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f}, { 0.0f,  1.0f,  0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}},
    {{ 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}},
    {{-1.0f, -1.0f,  1.0f}, {0.0f, 1.0f}, { 0.0f, -1.0f,  0.0f}},
    {{-1.0f, -1.0f,  1.0f}, {0.0f, 1.0f}, { 0.0f, -1.0f,  0.0f}},
    {{ 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}, { 0.0f, -1.0f,  0.0f}},
    {{ 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f}, { 0.0f, -1.0f,  0.0f}},
};

bool LightModel::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename)
{
    // Initialize the vertex and index buffers.
    if (!initialize_buffers(device))
    {
        return false;
    }

    // Load the texture for this model.
    if (!load_texture(device, deviceContext, textureFilename))
    {
        return false;
    }

    // Initialize the world matrix to the identity matrix.
    m_worldMatrix = XMMatrixIdentity();

    return true;
}

void LightModel::shutdown()
{
    // Release the model texture.
    release_texture();

    // Shutdown the vertex and index buffers.
    shutdown_buffers();
}

void LightModel::update()
{
    static float t = 0.0f;
    static ULONGLONG timeStart = 0;
    ULONGLONG timeCur = GetTickCount64();
    if (timeStart == 0)
        timeStart = timeCur;
    t = (timeCur - timeStart) / 1000.0f;

    auto pos_matrix = XMMatrixTranslation(cosf(t) * 3, sinf(t) * 3, 0.0f);

    // Rotate the world matrix by the rotation value so that the triangle will spin.
    static float rotation = 0.0f;
    rotation += 0.01f;
    auto rot_matrix = XMMatrixRotationRollPitchYaw(rotation, rotation, 0);
    m_worldMatrix = rot_matrix * pos_matrix;
}

void LightModel::render(ID3D11DeviceContext* deviceContext)
{
    // Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
    render_buffers(deviceContext);
}

int LightModel::get_index_count()
{
    return m_indexCount;
}

void LightModel::get_world_matrix(XMMATRIX& mat)
{
    mat = m_worldMatrix;
}

ID3D11ShaderResourceView* LightModel::get_texture()
{
    return m_texture->get_texture();
}

bool LightModel::initialize_buffers(ID3D11Device* device)
{
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    // Set the number of vertices in the vertex array.
    m_vertexCount = _countof(s_box_vertices);

    // Set the number of indices in the index array.
    m_indexCount = _countof(s_box_vertices);

    // Create the index array.
    indices = new unsigned long[m_indexCount];
    if (!indices)
    {
        return false;
    }

    for (int i = 0; i < m_indexCount; i++)
    {
        indices[i] = i;
    }

    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = s_box_vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Release the arrays now that the vertex and index buffers have been created and loaded.
    delete[] indices;
    indices = nullptr;

    return true;
}

void LightModel::shutdown_buffers()
{
    // Release the index buffer.
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = 0;
    }

    // Release the vertex buffer.
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = 0;
    }
}

void LightModel::render_buffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;
    unsigned int offset;


    // Set vertex buffer stride and offset.
    stride = sizeof(VertexType);
    offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool LightModel::load_texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
    // Create the texture object.
    m_texture = new Texture;
    if (!m_texture)
    {
        return false;
    }

    // Initialize the texture object.
    if (!m_texture->initialize(device, deviceContext, filename))
    {
        return false;
    }

    return true;
}

void LightModel::release_texture()
{
    // Release the texture object.
    if (m_texture)
    {
        m_texture->shutdown();
        delete m_texture;
        m_texture = nullptr;
    }
}

