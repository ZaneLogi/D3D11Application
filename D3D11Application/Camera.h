#pragma once

#include <directxmath.h>
using namespace DirectX;

class Camera
{
public:
    Camera() = default;
    Camera(const Camera&) = delete;

    void set_position(float, float, float);
    void set_rotation(float, float, float);

    XMFLOAT3 get_position();
    XMFLOAT3 get_rotation();

    void render();
    void get_view_matrix(XMMATRIX&);

private:
    float m_positionX = 0, m_positionY = 0, m_positionZ = 0;
    float m_rotationX = 0, m_rotationY = 0, m_rotationZ = 0;
    XMMATRIX m_viewMatrix;
};
