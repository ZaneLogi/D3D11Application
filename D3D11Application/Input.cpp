#include "Input.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

bool Input::initialize(HWND hWnd)
{
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = hWnd;
    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));


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

void Input::handle_input(HRAWINPUT raw_input)
{
    UINT dwSize = sizeof(RAWINPUT);
    static BYTE lpb[sizeof(RAWINPUT)];

    GetRawInputData(raw_input, RID_INPUT,
        lpb, &dwSize, sizeof(RAWINPUTHEADER));

    RAWINPUT* raw = (RAWINPUT*)lpb;

    if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        m_mouse_move_x += raw->data.mouse.lLastX;
        m_mouse_move_y += raw->data.mouse.lLastY;
    }
}

void Input::get_mouse_move(int* x, int *y)
{
    if (m_mouse_captured)
    {
        *x = m_mouse_move_x;
        *y = m_mouse_move_y;
    }
    else
    {
        *x = 0;
        *y = 0;
    }
    m_mouse_move_x = 0;
    m_mouse_move_y = 0;
}

void Input::handle_lbutton_down(HWND hWnd)
{
    m_mouse_captured = true;
    SetCapture(hWnd);
}

void Input::handle_lbutton_up()
{
    m_mouse_captured = false;
    ReleaseCapture();
}

void Input::handle_rbutton_down(HWND hWnd)
{
    m_mouse_captured = true;
    SetCapture(hWnd);
}

void Input::handle_rbutton_up()
{
    m_mouse_captured = false;
    ReleaseCapture();
}

void Input::handle_mousemove()
{

}
