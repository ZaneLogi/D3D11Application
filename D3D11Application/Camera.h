#pragma once

#include <directxmath.h>
using namespace DirectX;

__declspec(align(16)) class Camera
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
