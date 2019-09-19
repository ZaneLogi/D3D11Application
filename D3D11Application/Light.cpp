#include "Light.h"

void Light::set_diffuse_color(float red, float green, float blue, float alpha)
{
    m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::set_direction(float x, float y, float z)
{
    m_direction = XMFLOAT3(x, y, z);
}

XMFLOAT4 Light::get_diffuse_color()
{
    return m_diffuseColor;
}

XMFLOAT3 Light::get_direction()
{
    return m_direction;
}

