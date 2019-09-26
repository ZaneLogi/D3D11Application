#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#include "Texture.h"

class BitmapModel
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };

public:
    BitmapModel() = default;
    BitmapModel(const BitmapModel&) = delete;

    bool initialize(ID3D11Device*, ID3D11DeviceContext*, char*, int, int, int, int);
    void shutdown();
    void set_screen_size(int width, int height);
    bool render(ID3D11DeviceContext*, int, int);

    int get_index_count();
    ID3D11ShaderResourceView* get_texture();

private:
    bool initialize_buffers(ID3D11Device*);
    void shutdown_buffers();
    bool update_buffers(ID3D11DeviceContext*, int, int);
    void render_buffers(ID3D11DeviceContext*);

    bool load_texture(ID3D11Device*, ID3D11DeviceContext*, char*);
    void release_texture();

private:
    ID3D11Buffer*   m_vertexBuffer = nullptr;
    ID3D11Buffer*   m_indexBuffer = nullptr;
    int             m_vertexCount = 0;
    int             m_indexCount = 0;

    Texture*        m_texture = nullptr;

    int             m_screenWidth, m_screenHeight;
    int             m_bitmapWidth, m_bitmapHeight;
    int             m_previousPosX, m_previousPosY;
};
