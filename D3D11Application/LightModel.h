#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#include "Texture.h"

__declspec(align(16)) class LightModel
{
public:
    void* operator new(size_t i)
    {
        return _mm_malloc(i, 16);
    }

    void operator delete(void* p)
    {
        _mm_free(p);
    }

private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
        XMFLOAT3 normal;
    };

    static VertexType s_box_vertices[];

public:
    LightModel() = default;
    LightModel(const LightModel&) = delete;

    bool initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename);
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