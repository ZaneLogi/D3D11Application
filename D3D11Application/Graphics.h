#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "D3DCore.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include "TextureModel.h"
#include "TextureShader.h"
#include "LightModel.h"
#include "LightShader.h"
#include "Light.h"
#include "BitmapModel.h"
#include "RectangleModel.h"
#include "RectangleShader.h"
#include "Text.h"

class Graphics
{
public:
    Graphics() = default;
    Graphics(const Graphics&) = delete;

    bool initialize(HWND hwnd, int screen_width, int screen_height);
    void shutdown();
    void resize_window(int window_width, int window_height);
    bool update_camera(int x, int y);
    bool update();
    bool render();

private:
    D3DCore*        m_d3dcore = nullptr;
    Camera*         m_camera = nullptr;
    Model*          m_model = nullptr;
    ColorShader*    m_color_shader = nullptr;
    TextureModel*   m_texture_model = nullptr;
    TextureShader*  m_texture_shader = nullptr;
    LightModel*     m_light_model = nullptr;
    LightShader*    m_light_shader = nullptr;
    Light*          m_light = nullptr;
    BitmapModel*    m_bitmap_model = nullptr;
    RectangleModel* m_rectangle_model = nullptr;
    RectangleShader* m_rectangle_shader = nullptr;
    Text*           m_text = nullptr;

    float           m_camera_speed = -0.1f;
    float           m_camera_rotation = 0.5f;
};
