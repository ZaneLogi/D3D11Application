#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class BitmapFont
{
private:
    struct FontType
    {
        float left, right;
        int size;
    };

    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };

public:
    BitmapFont() = default;
    BitmapFont(const BitmapFont&) = delete;

    bool initialize(ID3D11Device*, const char*, const char*);
    void shutdown();

    ID3D11ShaderResourceView* GetTexture();

    void build_vertex_array(void*, char*, float, float);

private:
    bool LoadFontData(const char*);
    void ReleaseFontData();
    bool LoadTexture(ID3D11Device*, const char*);
    void ReleaseTexture();

private:
    FontType* m_Font = nullptr;
    ID3D11Resource* m_pTexture = nullptr;
    ID3D11ShaderResourceView* m_pTextureView = nullptr;
};

