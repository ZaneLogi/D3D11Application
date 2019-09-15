#include "Input.h"

bool Input::initialize()
{
    for (int i = 0; i < 256; i++)
    {
        m_keys[i] = false;
    }
    return true;
}

void Input::key_down(int key_index)
{
    m_keys[key_index] = true;
}

void Input::key_up(int key_index)
{
    m_keys[key_index] = false;
}

bool Input::is_key_down(int key_index)
{
    return m_keys[key_index];
}