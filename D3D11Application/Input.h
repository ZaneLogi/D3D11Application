#pragma once

class Input
{
public:
    Input() = default;
    Input(const Input&) = delete;

    bool initialize();

    void key_down(int key_index);
    void key_up(int key_index);
    bool is_key_down(int key_index);

private:
    bool m_keys[256];
};