#include "Input.h"

void Input::Init()
{
    for (int i = 0; i < 256; ++i)
    {
        m_keys[i] = false;
    }
}

void Input::Release()
{
    delete this;
}

void Input::KeyDown(const unsigned int& input)
{
    m_keys[input] = true;
}

void Input::KeyUp(const unsigned int& input)
{
    m_keys[input] = false;
}

bool Input::IsKeyDown(const unsigned int& key)
{
    return m_keys[key];
}