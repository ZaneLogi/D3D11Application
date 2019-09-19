#pragma once

#include <directxmath.h>
using namespace DirectX;

class Light
{
public:
    Light() = default;
    Light(const Light&) = delete;

    void set_diffuse_color(float, float, float, float);
    void set_direction(float, float, float);

    XMFLOAT4 get_diffuse_color();
    XMFLOAT3 get_direction();

private:
    XMFLOAT4 m_diffuseColor;
    XMFLOAT3 m_direction;
};
