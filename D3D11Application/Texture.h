#pragma once

#include <d3d11.h>

class Texture
{
private:
    struct TargaHeader
    {
        unsigned char data1[12];
        unsigned short width;
        unsigned short height;
        unsigned char bpp;
        unsigned char data2;
    };

public:
    Texture() = default;
    Texture(const Texture&) = delete;

    bool initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
    void shutdown();

    ID3D11ShaderResourceView* get_texture();

private:
    bool load_targa(char*, int&, int&);

private:
    unsigned char*              m_targaData = nullptr;
    ID3D11Texture2D*            m_texture = nullptr;
    ID3D11ShaderResourceView*   m_textureView = nullptr;
};