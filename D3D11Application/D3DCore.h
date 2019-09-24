#pragma once

#include <d3d11_1.h>
#include <directxmath.h>
using namespace DirectX;

__declspec(align(16)) class D3DCore
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

public:
    D3DCore() = default;
    D3DCore(const D3DCore&) = delete;

    bool initialize(HWND hwnd, int screen_width, int screen_height);
    void shutdown();

    bool resize_window(int window_width, int window_height);

    void begin_scene(float red, float green, float blue, float alpha);
    void end_scene();

    ID3D11Device* get_device();
    ID3D11DeviceContext* get_device_context();

    void get_projection_matrix(XMMATRIX& mat);
    void get_ortho_matrix(XMMATRIX& mat);

    void turn_on_z_buffer();
    void turn_off_z_buffer();

    void turn_on_alpha_blending();
    void turn_off_alpha_blending();

private:
    D3D_DRIVER_TYPE             m_driverType = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL           m_featureLevel = D3D_FEATURE_LEVEL_11_0;

    ID3D11Device*               m_pd3dDevice = nullptr;
    ID3D11Device1*              m_pd3dDevice1 = nullptr;
    ID3D11DeviceContext*        m_pImmediateContext = nullptr;
    ID3D11DeviceContext1*       m_pImmediateContext1 = nullptr;
    IDXGISwapChain*             m_pSwapChain = nullptr;
    IDXGISwapChain1*            m_pSwapChain1 = nullptr;
    ID3D11RenderTargetView*     m_pRenderTargetView = nullptr;
    ID3D11Texture2D*            m_pDepthStencil = nullptr;
    ID3D11DepthStencilView*     m_pDepthStencilView = nullptr;

    bool                        m_vsync_enabled;
    int                         m_videoCardMemory;
    char                        m_videoCardDescription[128];

    ID3D11RasterizerState*      m_rasterState = nullptr;
    ID3D11DepthStencilState*    m_depthStencilState = nullptr;
    ID3D11DepthStencilState*    m_depthDisabledStencilState = nullptr;
    ID3D11BlendState*           m_alphaEnableBlendingState = nullptr;
    ID3D11BlendState*           m_alphaDisableBlendingState = nullptr;

    XMMATRIX                    m_projectionMatrix;
    XMMATRIX                    m_orthoMatrix;
};