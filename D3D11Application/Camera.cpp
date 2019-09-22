#include "Camera.h"
#include <Windows.h>

Camera::Camera()
{
    set_view_parameters(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 1), XMFLOAT3(0, 1, 0));
    m_is_fixed_yaw_axis = false;
}

void Camera::set_view_parameters(const XMFLOAT3& eyePos, const XMFLOAT3& lookAt, const XMFLOAT3& upVec)
{
    auto xm_matrix = XMMatrixLookAtLH(XMLoadFloat3(&eyePos), XMLoadFloat3(&lookAt), XMLoadFloat3(&upVec));
    XMStoreFloat4x4(&m_view_matrix, xm_matrix);

    m_right.x = m_view_matrix._11; m_up.x = m_view_matrix._12; m_look.x = m_view_matrix._13;
    m_right.y = m_view_matrix._21; m_up.y = m_view_matrix._22; m_look.y = m_view_matrix._23;
    m_right.z = m_view_matrix._31; m_up.z = m_view_matrix._32; m_look.z = m_view_matrix._33;

    m_position = eyePos;
}

XMMATRIX Camera::get_billboard_matrix() const
{
    auto xm_inverse_matrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_view_matrix));
    XMFLOAT4X4 billboard_matrix;
    XMStoreFloat4x4(&billboard_matrix, xm_inverse_matrix);
    billboard_matrix._41 = 0.0f;
    billboard_matrix._42 = 0.0f;
    billboard_matrix._43 = 0.0f;
    return XMLoadFloat4x4(&billboard_matrix);
}

void Camera::set_position(const XMFLOAT3& pos)
{
    m_position = pos;

    XMFLOAT3 r;
    XMStoreFloat3(&r, XMVector3Dot(XMLoadFloat3(&m_position), XMLoadFloat3(&m_right)));
    m_view_matrix._41 = -r.x;

    XMStoreFloat3(&r, XMVector3Dot(XMLoadFloat3(&m_position), XMLoadFloat3(&m_up)));
    m_view_matrix._42 = -r.x;

    XMStoreFloat3(&r, XMVector3Dot(XMLoadFloat3(&m_position), XMLoadFloat3(&m_look)));
    m_view_matrix._43 = -r.x;
}

void Camera::rotate(float pitch, float yaw, float roll)
{
    bool changed = false;

    if (m_is_fixed_yaw_axis)
    {
        // limit the angle between the m_look and m_fixed_yaw_axis
        XMFLOAT3 r;
        XMStoreFloat3(&r, XMVector3Dot(XMLoadFloat3(&m_look), m_fixed_yaw_axis));
        float cosTheta = r.x;
        float theta = acosf(cosTheta);
        if (theta + pitch < 0.5236f) // 30 degrees
            pitch = 0.5236f - theta;
        else if (theta + pitch > 2.618f) // 150 degrees
            pitch = 2.618f - theta;
    }

    if (pitch != 0) {
        auto matRot = XMMatrixRotationAxis(XMLoadFloat3(&m_right), pitch);
        XMStoreFloat3(&m_up, XMVector3TransformCoord(XMLoadFloat3(&m_up), matRot));
        XMStoreFloat3(&m_look, XMVector3TransformCoord(XMLoadFloat3(&m_look), matRot));
        changed = true;
    }

    if (yaw != 0)
    {
        XMVECTOR up_vec;
        if (m_is_fixed_yaw_axis)
            up_vec = m_fixed_yaw_axis;
        else
            up_vec = XMLoadFloat3(&m_up);

        auto matRot = XMMatrixRotationAxis(up_vec, yaw);
        XMStoreFloat3(&m_look, XMVector3TransformCoord(XMLoadFloat3(&m_look), matRot));
        XMStoreFloat3(&m_right, XMVector3TransformCoord(XMLoadFloat3(&m_right), matRot));
        changed = true;
    }

    if (roll != 0 && !m_is_fixed_yaw_axis)
    {
        auto matRot = XMMatrixRotationAxis(XMLoadFloat3(&m_look), roll);
        XMStoreFloat3(&m_right, XMVector3TransformCoord(XMLoadFloat3(&m_right), matRot));
        XMStoreFloat3(&m_up, XMVector3TransformCoord(XMLoadFloat3(&m_up), matRot));
        changed = true;
    }

    if (changed)
    {
        auto look_vec = XMVector3Normalize(XMLoadFloat3(&m_look));
        XMStoreFloat3(&m_look, look_vec);

        auto right_vec = XMVector3Normalize(
            m_is_fixed_yaw_axis
            ? XMVector3Cross(m_fixed_yaw_axis, look_vec)
            : XMVector3Cross(XMLoadFloat3(&m_up), look_vec));

        XMStoreFloat3(&m_right, right_vec);

        auto up_vec = XMVector3Normalize(XMVector3Cross(look_vec, right_vec));
        XMStoreFloat3(&m_up, up_vec);

        m_view_matrix._11 = m_right.x; m_view_matrix._12 = m_up.x; m_view_matrix._13 = m_look.x;
        m_view_matrix._21 = m_right.y; m_view_matrix._22 = m_up.y; m_view_matrix._23 = m_look.y;
        m_view_matrix._31 = m_right.z; m_view_matrix._32 = m_up.z; m_view_matrix._33 = m_look.z;

        XMFLOAT3 r;
        XMStoreFloat3(&r, XMVector3Dot(XMLoadFloat3(&m_position), right_vec));
        m_view_matrix._41 = -r.x;

        XMStoreFloat3(&r, XMVector3Dot(XMLoadFloat3(&m_position), up_vec));
        m_view_matrix._42 = -r.x;

        XMStoreFloat3(&r, XMVector3Dot(XMLoadFloat3(&m_position), look_vec));
        m_view_matrix._43 = -r.x;
    }
}

void Camera::move(float forward, float strafe, float up)
{
    auto look_vec = XMLoadFloat3(&m_look);
    auto right_vec = XMLoadFloat3(&m_right);
    auto up_vec = XMLoadFloat3(&m_up);

    auto pos_vec = look_vec * forward + right_vec * strafe + up_vec * up;
    XMStoreFloat3(&m_position, pos_vec);

    XMFLOAT3 r;
    XMStoreFloat3(&r, XMVector3Dot(pos_vec, right_vec));
    m_view_matrix._41 = -r.x;

    XMStoreFloat3(&r, XMVector3Dot(pos_vec, up_vec));
    m_view_matrix._42 = -r.x;

    XMStoreFloat3(&r, XMVector3Dot(pos_vec, look_vec));
    m_view_matrix._43 = -r.x;
}

void Camera::fix_yaw_axis(const XMFLOAT3& axis)
{
    m_is_fixed_yaw_axis = true;
    m_fixed_yaw_axis = XMVector3Normalize(XMLoadFloat3(&axis));
}

void Camera::render()
{
    static float t = 0.0f;
    static ULONGLONG timeStart = 0;
    ULONGLONG timeCur = GetTickCount64();
    if (timeStart == 0)
        timeStart = timeCur;
    t = (timeCur - timeStart) / 1000.0f;

    //set_view_parameters(XMFLOAT3(0, 0, -10), XMFLOAT3(sinf(t) * 10, 0.0f, 0.0f), XMFLOAT3(0, 1, 0));
    //set_position(XMFLOAT3(0.0f, sinf(t) * 10, -20.0f));
    //set_position(XMFLOAT3(cosf(t) * 10, 0.0f, -20.0f));
}
