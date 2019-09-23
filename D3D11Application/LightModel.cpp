#include "LightModel.h"

void LightModel::create_box(float width, float height, float depth, std::vector<LightModel::VertexType>& vertices, std::vector<unsigned long>& indices)
{
    const auto w2 = width * 0.5f;
    const auto h2 = height * 0.5f;
    const auto d2 = depth * 0.5f;

    vertices.clear();
    indices.clear();

    // left-hand coordinates
    unsigned long vertex_index = 0;

    // front face
    vertex_index = (unsigned long)vertices.size();
    vertices.push_back({ { -w2,  h2, -d2 }, { 0.0f, 0.0f }, { 0.0f,  0.0f, -1.0f } });
    vertices.push_back({ {  w2,  h2, -d2 }, { 1.0f, 0.0f }, { 0.0f,  0.0f, -1.0f } });
    vertices.push_back({ { -w2, -h2, -d2 }, { 0.0f, 1.0f }, { 0.0f,  0.0f, -1.0f } });
    vertices.push_back({ {  w2, -h2, -d2 }, { 1.0f, 1.0f }, { 0.0f,  0.0f, -1.0f } });
    indices.push_back(vertex_index);
    indices.push_back(vertex_index + 1);
    indices.push_back(vertex_index + 2);
    indices.push_back(vertex_index + 2);
    indices.push_back(vertex_index + 1);
    indices.push_back(vertex_index + 3);
    // right face
    vertex_index = (unsigned long)vertices.size();
    vertices.push_back({ { w2,  h2, -d2 }, { 0.0f, 0.0f }, { 1.0f,  0.0f,  0.0f } });
    vertices.push_back({ { w2,  h2,  d2 }, { 1.0f, 0.0f }, { 1.0f,  0.0f,  0.0f } });
    vertices.push_back({ { w2, -h2, -d2 }, { 0.0f, 1.0f }, { 1.0f,  0.0f,  0.0f } });
    vertices.push_back({ { w2, -h2,  d2 }, { 1.0f, 1.0f }, { 1.0f,  0.0f,  0.0f } });
    indices.push_back(vertex_index);
    indices.push_back(vertex_index + 1);
    indices.push_back(vertex_index + 2);
    indices.push_back(vertex_index + 2);
    indices.push_back(vertex_index + 1);
    indices.push_back(vertex_index + 3);
    // back face
    vertex_index = (unsigned long)vertices.size();
    vertices.push_back({ {  w2,  h2,  d2 }, { 0.0f, 0.0f }, { 0.0f,  0.0f,  1.0f } });
    vertices.push_back({ { -w2,  h2,  d2 }, { 1.0f, 0.0f }, { 0.0f,  0.0f,  1.0f } });
    vertices.push_back({ {  w2, -h2,  d2 }, { 0.0f, 1.0f }, { 0.0f,  0.0f,  1.0f } });
    vertices.push_back({ { -w2, -h2,  d2 }, { 1.0f, 1.0f }, { 0.0f,  0.0f,  1.0f } });
    indices.push_back(vertex_index);
    indices.push_back(vertex_index + 1);
    indices.push_back(vertex_index + 2);
    indices.push_back(vertex_index + 2);
    indices.push_back(vertex_index + 1);
    indices.push_back(vertex_index + 3);
    // left face
    vertex_index = (unsigned long)vertices.size();
    vertices.push_back({ { -w2,  h2,  d2 }, { 0.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } });
    vertices.push_back({ { -w2,  h2, -d2 }, { 1.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } });
    vertices.push_back({ { -w2, -h2,  d2 }, { 0.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } });
    vertices.push_back({ { -w2, -h2, -d2 }, { 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } });
    indices.push_back(vertex_index);
    indices.push_back(vertex_index + 1);
    indices.push_back(vertex_index + 2);
    indices.push_back(vertex_index + 2);
    indices.push_back(vertex_index + 1);
    indices.push_back(vertex_index + 3);
    // top face
    vertex_index = (unsigned long)vertices.size();
    vertices.push_back({ { -w2, h2,  d2 }, { 0.0f, 0.0f }, { 0.0f,  1.0f,  0.0f } });
    vertices.push_back({ { w2,  h2,  d2 }, { 1.0f, 0.0f }, { 0.0f,  1.0f,  0.0f } });
    vertices.push_back({ { -w2, h2, -d2 }, { 0.0f, 1.0f }, { 0.0f,  1.0f,  0.0f } });
    vertices.push_back({ { w2,  h2, -d2 }, { 1.0f, 1.0f }, { 0.0f,  1.0f,  0.0f } });
    indices.push_back(vertex_index);
    indices.push_back(vertex_index + 1);
    indices.push_back(vertex_index + 2);
    indices.push_back(vertex_index + 2);
    indices.push_back(vertex_index + 1);
    indices.push_back(vertex_index + 3);
    // bottom face
    vertex_index = (unsigned long)vertices.size();
    vertices.push_back({ { -w2, -h2, -d2 }, { 0.0f, 0.0f }, { 0.0f, -1.0f,  0.0f } });
    vertices.push_back({ {  w2, -h2, -d2 }, { 1.0f, 0.0f }, { 0.0f, -1.0f,  0.0f } });
    vertices.push_back({ { -w2, -h2,  d2 }, { 0.0f, 1.0f }, { 0.0f, -1.0f,  0.0f } });
    vertices.push_back({ {  w2, -h2,  d2 }, { 1.0f, 1.0f }, { 0.0f, -1.0f,  0.0f } });
    indices.push_back(vertex_index);
    indices.push_back(vertex_index+1);
    indices.push_back(vertex_index+2);
    indices.push_back(vertex_index+2);
    indices.push_back(vertex_index+1);
    indices.push_back(vertex_index+3);
}

void LightModel::create_sphere(float radius, int sliceCount, int stackCount, std::vector<VertexType>& vertices, std::vector<unsigned long>& indices)
{
    vertices.clear();
    indices.clear();

    float phiStep = XM_PI / stackCount;
    float thetaStep = XM_2PI / sliceCount;

    vertices.push_back({ { 0, 1, 0 },{ 0, 0 },{ 0, 1, 0 } });

    for (int i = 1; i <= stackCount - 1; i++)
    {
        float phi = i * phiStep;
        for (int j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;
            XMFLOAT3 pos(
                radius * sinf(phi) * cosf(theta),
                radius * cosf(phi),
                radius * sinf(phi) * sinf(theta));

            XMFLOAT2 uv(theta / (XM_PI * 2), phi / XM_PI);

            XMFLOAT3 norm(pos);

            vertices.push_back({ pos, uv, norm });
        }
    }

    vertices.push_back({ { 0, -1, 0 },{ 0, 1 },{ 0, -1, 0 } });

    for (int i = 1; i <= sliceCount; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i);
    }

    unsigned long baseIndex = 1;
    unsigned long ringVertexCount = sliceCount + 1;
    for (int i = 0; i < stackCount - 2; i++)
    {
        for (int j = 0; j < sliceCount; j++)
        {
            indices.push_back(baseIndex + i*ringVertexCount + j);
            indices.push_back(baseIndex + i*ringVertexCount + j + 1);
            indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

            indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
            indices.push_back(baseIndex + i*ringVertexCount + j + 1);
            indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
        }
    }

    unsigned long southPoleIndex = (unsigned long)vertices.size() - 1;
    baseIndex = southPoleIndex - ringVertexCount;
    for (int i = 0; i < sliceCount; i++)
    {
        indices.push_back(southPoleIndex);
        indices.push_back(baseIndex + i);
        indices.push_back(baseIndex + i + 1);
    }
}

void LightModel::create_cylinder(float top_radius, float bottom_radius, float height, int sliceCount, int stackCount, std::vector<VertexType>& vertices, std::vector<unsigned long>& indices)
{
    vertices.clear();
    indices.clear();

    int baseIndex = 0;
    float x, y, z, u, v, r;
    float dTheta;

    // top
    y = 0.5f * height;
    dTheta = XM_2PI / sliceCount;

    for (int i = 0; i <= sliceCount; i++)
    {
        x = top_radius * cosf(i*dTheta);
        z = top_radius * sinf(i*dTheta);

        u = x / height + 0.5f;
        v = z / height + 0.5f;

        vertices.push_back({ {x,y,z}, {u,v}, {0,1,0} });
    }

    vertices.push_back({ { 0,y,0 }, { 0.5f,0.5f }, { 0, 1, 0 } });
    int centerIndex = (int)vertices.size() - 1;
    for (int i = 0; i < sliceCount; i++)
    {
        indices.push_back(centerIndex);
        indices.push_back(baseIndex + i + 1);
        indices.push_back(baseIndex + i);
    }

    // body
    baseIndex = (int)vertices.size();

    float stackHeight = height / stackCount;
    float radiusStep = (top_radius - bottom_radius) / stackCount;
    int ringCount = stackCount + 1;

    for (int i = 0; i < ringCount; i++)
    {
        y = -0.5f*height + i*stackHeight;
        r = bottom_radius + i*radiusStep;
        dTheta = XM_2PI / sliceCount;
        for (int j = 0; j <= sliceCount; j++)
        {
            float c = cosf(j*dTheta);
            float s = sinf(j*dTheta);

            float dr = bottom_radius - top_radius;

            auto t = XMFLOAT3(-s, 0.0f, c);
            auto bitangent = XMFLOAT3(dr*c, -height, dr*s);

            auto n = XMVector3Cross(XMLoadFloat3(&t), XMLoadFloat3(&bitangent));
            n = XMVector3Normalize(n);
            XMFLOAT3 norm;
            XMStoreFloat3(&norm, n);

            vertices.push_back({ {r*c, y, r*s}, { (float)j / sliceCount, 1.0f - (float)i / stackCount }, norm });
        }
    }

    int ringVertexCount = sliceCount + 1;
    for (int i = 0; i < stackCount; i++)
    {
        for (int j = 0; j < sliceCount; j++)
        {
            indices.push_back(baseIndex + i*ringVertexCount + j);
            indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
            indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);

            indices.push_back(baseIndex + i*ringVertexCount + j);
            indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
            indices.push_back(baseIndex + i*ringVertexCount + j + 1);
        }
    }

    // bottom
    baseIndex = (int)vertices.size();

    y = -0.5f * height;
    dTheta = XM_2PI / sliceCount;

    for (int i = 0; i <= sliceCount; i++)
    {
        x = bottom_radius * cosf(i * dTheta);
        z = bottom_radius * sinf(i * dTheta);

        u = x / height + 0.5f;
        v = z / height + 0.5f;
        vertices.push_back({ { x,y,z },{ u,v },{ 0,-1,0 } });
    }
    vertices.push_back({ { 0,y,0 },{ 0.5f,0.5f },{ 0, -1, 0 } });
    centerIndex = (int)vertices.size() - 1;
    for (int i = 0; i < sliceCount; i++)
    {
        indices.push_back(centerIndex);
        indices.push_back(baseIndex + i);
        indices.push_back(baseIndex + i + 1);
    }
}

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
    auto rot_matrix = XMMatrixRotationRollPitchYaw(0, rotation, 0);
    auto scale_mat = XMMatrixScaling(1.5f, 1.5f, 1.5f);
    m_worldMatrix = rot_matrix * scale_mat * pos_matrix;
}

void LightModel::render(ID3D11DeviceContext* deviceContext)
{
    // Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
    render_buffers(deviceContext);
}

unsigned long LightModel::get_index_count()
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
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    std::vector<VertexType> vertices;
    std::vector<unsigned long> indices;
    //create_box(1, 2, 3, vertices, indices);
    //create_sphere(1, 20, 20, vertices, indices);
    create_cylinder(0.3, 0.5, 1, 10, 10, vertices, indices);

    // Set the number of vertices in the vertex array.
    m_vertexCount = (unsigned long)vertices.size();

    // Set the number of indices in the index array.
    m_indexCount = (unsigned long)indices.size();

    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices.data();
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
    indexData.pSysMem = indices.data();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

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

