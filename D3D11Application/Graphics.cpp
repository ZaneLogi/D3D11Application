#include "Graphics.h"

bool Graphics::initialize(HWND hwnd, int screen_width, int screen_height)
{
    m_d3dcore = new D3DCore;
    if (!m_d3dcore ||
        !m_d3dcore->initialize(hwnd, screen_width, screen_height))
    {
        return false;
    }

    // Create the camera object.
    m_camera = new Camera;
    if (!m_camera)
    {
        return false;
    }

    // Set the initial position of the camera.
    m_camera->set_position(0.0f, 0.0f, -10.0f);

    // Create the model object.
    m_model = new Model;
    if (!m_model)
    {
        return false;
    }

    // Initialize the model object.
    if (!m_model->initialize(m_d3dcore->get_device()))
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    // Create the color shader object.
    m_color_shader = new ColorShader;
    if (!m_color_shader)
    {
        return false;
    }

    // Initialize the color shader object.
    if (!m_color_shader->initialize(m_d3dcore->get_device(), hwnd))
    {
        MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
        return false;
    }

    //
    m_texture_model = new TextureModel;
    if (!m_texture_model)
    {
        return false;
    }

    //
    if (!m_texture_model->initialize(m_d3dcore->get_device(), m_d3dcore->get_device_context(), "stone01.tga"))
    {
        MessageBox(hwnd, L"Could not initialize the texture model object.", L"Error", MB_OK);
        return false;
    }

    // Create the texture shader object.
    m_texture_shader = new TextureShader;
    if (!m_texture_shader)
    {
        return false;
    }

    // Initialize the texture shader object.
    if (!m_texture_shader->initialize(m_d3dcore->get_device(), hwnd))
    {
        MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
        return false;
    }

    return true;
}

void Graphics::shutdown()
{
    if (m_texture_shader)
    {
        m_texture_shader->shutdown();
        delete m_texture_shader;
        m_texture_shader = nullptr;
    }

    if (m_texture_model)
    {
        m_texture_model->shutdown();
        delete m_texture_model;
        m_texture_model = nullptr;
    }

    // Release the color shader object.
    if (m_color_shader)
    {
        m_color_shader->shutdown();
        delete m_color_shader;
        m_color_shader = nullptr;
    }

    // Release the model object.
    if (m_model)
    {
        m_model->shutdown();
        delete m_model;
        m_model = nullptr;
    }

    // Release the camera object.
    if (m_camera)
    {
        delete m_camera;
        m_camera = nullptr;
    }

    if (m_d3dcore)
    {
        m_d3dcore->shutdown();
        delete m_d3dcore;
        m_d3dcore = nullptr;
    }
}

void Graphics::resize_window(int window_width, int window_height)
{
    m_d3dcore->resize_window(window_width, window_height);
}

bool Graphics::update()
{
    /*auto camera_pos = m_camera->get_position();
    camera_pos.z += m_camera_speed;
    if (camera_pos.z < -50.0f)
    {
        m_camera_speed = 0.05f;
    }
    else if (camera_pos.z > -1.0f)
    {
        m_camera_speed = -0.05f;
    }
    m_camera->set_position(camera_pos.x, camera_pos.y, camera_pos.z);
    */

    m_model->update();
    m_texture_model->update();
    return true;
}

bool Graphics::render()
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

    // Clear the buffers to begin the scene.
    m_d3dcore->begin_scene(0.0f, 0.0f, 0.0f, 1.0f);

    // Generate the view matrix based on the camera's position.
    m_camera->render();

    // Get the view, and projection matrices from the camera and d3d objects.
    m_camera->get_view_matrix(viewMatrix);
    m_d3dcore->get_projection_matrix(projectionMatrix);

    m_model->get_world_matrix(worldMatrix);
    // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
    m_model->render(m_d3dcore->get_device_context());
    // Render the model using the color shader.
    if (!m_color_shader->render(m_d3dcore->get_device_context(), m_model->get_index_count(), worldMatrix, viewMatrix, projectionMatrix))
    {
        return false;
    }

    m_texture_model->get_world_matrix(worldMatrix);
    m_texture_model->render(m_d3dcore->get_device_context());
    if (!m_texture_shader->render(m_d3dcore->get_device_context(),
        m_texture_model->get_index_count(),
        worldMatrix, viewMatrix, projectionMatrix,
        m_texture_model->get_texture()))
    {
        return false;
    }


    // Present the rendered scene to the screen.
    m_d3dcore->end_scene();

    return true;
}