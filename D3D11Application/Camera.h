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
    Camera();
    Camera(const Camera&) = delete;

    void set_view_parameters(const XMFLOAT3& eyePos, const XMFLOAT3& lookAt, const XMFLOAT3& upVec);
    XMFLOAT3 get_position() const { return m_position; }
    XMFLOAT3 get_up_vector() const { return m_up; }
    XMFLOAT3 get_view_direction() const { return m_look; }
    XMFLOAT3 get_cross_vector() const { return m_right; }

    XMMATRIX get_view_matrix() const { return XMLoadFloat4x4(&m_view_matrix); }
    XMMATRIX get_billboard_matrix() const;

    void set_position(const XMFLOAT3& pos);
    void rotate(float pitch, float yaw, float roll);
    void move(float forward, float strafe, float up);

    void fix_yaw_axis(const XMFLOAT3& axis);

    void render();

private:
    XMFLOAT3 m_position;
    XMFLOAT3 m_look;
    XMFLOAT3 m_up;
    XMFLOAT3 m_right;

    XMFLOAT4X4 m_view_matrix;

    bool        m_is_fixed_yaw_axis;
    XMVECTOR    m_fixed_yaw_axis;
};
