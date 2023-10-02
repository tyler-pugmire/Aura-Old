#pragma once
#include "Event.h"
#include <sstream>

namespace Aura
{
    class KeyEvent : public Event
    {
    public:
        inline KeyCode GetKeyCode() const { return m_key; }

        EVENT_CLASS_CATEGORY(Key)
    protected:
        KeyEvent(KeyCode key) : m_key(key) {}
        KeyCode m_key;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(KeyCode key, int repeatCount)
            : KeyEvent(key), m_repeatCount(repeatCount) {}

        inline int GetRepeatCount() const { return m_repeatCount; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_key << " (" << m_repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(KeyCode key)
            : KeyEvent(key) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_key;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(KeyCode key)
            : KeyEvent(key) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_key;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}