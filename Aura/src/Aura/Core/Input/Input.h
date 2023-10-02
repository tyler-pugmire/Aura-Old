#pragma once
#include "KeyCodes.h"

#include <map>

namespace Aura::Input
{
    struct KeyData
    {
        KeyCode key;
        KeyState state = KeyState::None;
        KeyState oldState = KeyState::None;
    };

    struct ButtonData
    {
        MouseButton button;
        KeyState state = KeyState::None;
        KeyState oldState = KeyState::None;
    };

    void Update();

    bool IsKeyPressed(KeyCode key);
    bool IsKeyHeld(KeyCode key);
    //bool IsKeyDown(KeyCode key);
    bool IsKeyRelease(KeyCode key);

    bool IsMouseButtonPressed(MouseButton button);
    bool IsMouseButtonHeld(MouseButton buttony);
    //bool IsMouseButtonDown(MouseButton button);
    bool IsMouseButtonRelease(MouseButton button);

    float GetMouseX();
    float GetMouseY();
    std::pair<float, float> GetMousePosition();

    void SetCursorMode(CursorMode mode);
    CursorMode GetCursorMode();

    void TransitionPressedKeys();
    void UpdateKeyState(KeyCode key, KeyState newState);
    void UpdateButtonState(MouseButton button, KeyState newState);
    void TransitionPressedButtons();
    void ClearReleasedKeys();
}