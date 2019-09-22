#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class RectangleModel
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

public:
    RectangleModel() = default;
    RectangleModel(const RectangleModel&) = delete;

    bool initialize(ID3D11Device*, ID3D11DeviceContext*, int, int);
    void shutdown();
    void set_screen_size(int width, int height);
    bool render(ID3D11DeviceContext*, int, int);

    int get_index_count();
    void get_world_matrix(XMMATRIX& mat);

private:
    bool initialize_buffers(ID3D11Device*);
    void shutdown_buffers();
    bool update_buffers(ID3D11DeviceContext*, int, int);
    void render_buffers(ID3D11DeviceContext*);

private:
    ID3D11Buffer*   m_vertexBuffer = nullptr;
    ID3D11Buffer*   m_indexBuffer = nullptr;
    int             m_vertexCount = 0;
    int             m_indexCount = 0;

    int             m_screenWidth, m_screenHeight;
    int             m_rectWidth, m_rectHeight;
    int             m_previousPosX, m_previousPosY;
};
