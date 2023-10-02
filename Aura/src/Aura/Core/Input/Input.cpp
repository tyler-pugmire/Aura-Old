#include "pch.h"
#include "Input.h"
#include "Aura/Core/Events/Event.h"
#include "Aura/Core/Events/KeyEvent.h"

namespace
{
    std::map<Aura::KeyCode, Aura::Input::KeyData> s_Keys;
    std::map<Aura::MouseButton, Aura::Input::ButtonData> s_Mouse;


}

namespace Aura::Input
{
    void Update()
    {

    }


    bool IsKeyPressed(KeyCode key)
    {
        return s_Keys.find(key) != s_Keys.end() && s_Keys[key].state == KeyState::Pressed;
    }
    bool IsKeyHeld(KeyCode key)
    {
        return s_Keys.find(key) != s_Keys.end() && s_Keys[key].state == KeyState::Held;
    }
    //bool IsKeyDown(KeyCode key)
    //{
    //
    //}
    bool IsKeyRelease(KeyCode key)
    {
        return s_Keys.find(key) != s_Keys.end() && s_Keys[key].state == KeyState::Released;
    }

    bool IsMouseButtonPressed(MouseButton button)
    {
        return s_Mouse.find(button) != s_Mouse.end() && s_Mouse[button].state == KeyState::Pressed;
    }
    bool IsMouseButtonHeld(MouseButton button)
    {
        return s_Mouse.find(button) != s_Mouse.end() && s_Mouse[button].state == KeyState::Held;
    }
    //bool IsMouseButtonDown(MouseButton button)
    //{
    //
    //}
    bool IsMouseButtonRelease(MouseButton button)
    {
        return s_Mouse.find(button) != s_Mouse.end() && s_Mouse[button].state == KeyState::Released;
    }

    float GetMouseX()
    {
        return 0.0f;
    }

    float GetMouseY()
    {
        return 0.0f;

    }
    std::pair<float, float> GetMousePosition()
    {
        return { 0, 0 };
    }

    void SetCursorMode(CursorMode mode)
    {

    }

    CursorMode GetCursorMode()
    {
        return CursorMode::Normal;
    }

    void TransitionPressedKeys()
    {
        for (const auto& [key, keyData] : s_Keys)
        {
            if (keyData.state == KeyState::Pressed)
                UpdateKeyState(key, KeyState::Held);
        }
    }

    void TransitionPressedButtons()
    {
        for (const auto& [button, buttonData] : s_Mouse)
        {
            if (buttonData.state == KeyState::Pressed)
                UpdateButtonState(button, KeyState::Held);
        }
    }

    void UpdateKeyState(KeyCode key, KeyState newState)
    {
        auto& keyData = s_Keys[key];
        keyData.key = key;
        keyData.oldState = keyData.state;
        keyData.state = newState;
    }

    void UpdateButtonState(MouseButton button, KeyState newState)
    {
        auto& mouseData = s_Mouse[button];
        mouseData.button = button;
        mouseData.oldState = mouseData.state;
        mouseData.state = newState;
    }

    void ClearReleasedKeys()
    {
        for (const auto& [key, keyData] : s_Keys)
        {
            if (keyData.state == KeyState::Released)
                UpdateKeyState(key, KeyState::None);
        }

        for (const auto& [button, buttonData] : s_Mouse)
        {
            if (buttonData.state == KeyState::Released)
                UpdateButtonState(button, KeyState::None);
        }
    }
}

