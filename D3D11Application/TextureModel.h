#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#include "Texture.h"

class TextureModel
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };

public:
    TextureModel() = default;
    TextureModel(const TextureModel&) = delete;

    bool initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
    void shutdown();
    void update();
    void render(ID3D11DeviceContext*);

    int get_index_count();

    void get_world_matrix(XMMATRIX& mat);

    ID3D11ShaderResourceView* get_texture();

private:
    bool initialize_buffers(ID3D11Device*);
    void shutdown_buffers();
    void render_buffers(ID3D11DeviceContext*);

    bool load_texture(ID3D11Device*, ID3D11DeviceContext*, char*);
    void release_texture();

private:
    ID3D11Buffer*   m_vertexBuffer = nullptr;
    ID3D11Buffer*   m_indexBuffer = nullptr;
    int             m_vertexCount = 0;
    int             m_indexCount = 0;
    XMMATRIX        m_worldMatrix;

    Texture*        m_texture = nullptr;
};