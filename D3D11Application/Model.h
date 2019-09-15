#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class Model
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

public:
    Model() = default;
    Model(const Model&) = delete;

    bool initialize(ID3D11Device*);
    void shutdown();
    void update();
    void render(ID3D11DeviceContext*);

    int get_index_count();
    void get_world_matrix(XMMATRIX& mat);

private:
    bool initialize_buffers(ID3D11Device*);
    void shutdown_buffers();
    void render_buffers(ID3D11DeviceContext*);

private:
    ID3D11Buffer*   m_vertexBuffer = nullptr;
    ID3D11Buffer*   m_indexBuffer = nullptr;
    int             m_vertexCount = 0;
    int             m_indexCount = 0;
    XMMATRIX        m_worldMatrix;
};