#pragma once
#include "Event.h"
#include <sstream>

namespace Aura
{
    class WindowClosedEvent : public Event
    {
    public:
        WindowClosedEvent() {}

        EVENT_CLASS_TYPE(WindowClosed);
        EVENT_CLASS_CATEGORY(Window);
    };

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned width, unsigned height) 
            : m_width(width), m_height(height) {}

        inline unsigned GetWidth() const { return m_width; }
        inline unsigned GetHeight() const { return m_height; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(Window)

    private:
        unsigned m_width;
        unsigned m_height;
    };
}