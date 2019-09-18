#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class TextureShader
{
private:
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

public:
    TextureShader() = default;
    TextureShader(const TextureShader&) = delete;

    bool initialize(ID3D11Device*, HWND);
    void shutdown();
    bool render(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*);

private:
    bool initialize_shader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
    void shutdown_shader();
    void output_shader_error_message(ID3D10Blob*, HWND, WCHAR*);

    bool set_shader_parameters(ID3D11DeviceContext*, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*);
    void render_shader(ID3D11DeviceContext*, int);

private:
    ID3D11VertexShader* m_vertexShader = nullptr;
    ID3D11PixelShader*  m_pixelShader = nullptr;
    ID3D11InputLayout*  m_layout = nullptr;
    ID3D11Buffer*       m_matrixBuffer = nullptr;

    ID3D11SamplerState* m_sampleState = nullptr;
};