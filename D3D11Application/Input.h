#pragma once

#include <Windows.h>

class Input
{
public:
    Input() = default;
    Input(const Input&) = delete;

    bool initialize(HWND hWnd);

    void key_down(int key_index);
    void key_up(int key_index);
    bool is_key_down(int key_index);

    void handle_input(HRAWINPUT raw_input);
    void get_mouse_move(int* x, int *y);

    void handle_lbutton_down(HWND hWnd);
    void handle_lbutton_up();
    void handle_rbutton_down(HWND hWnd);
    void handle_rbutton_up();
    void handle_mousemove();

private:
    bool m_keys[256];
    int m_last_mouse_x = -1;
    int m_last_mouse_y = -1;
    int m_mouse_move_x = 0;
    int m_mouse_move_y = 0;

    bool m_mouse_captured = false;
};