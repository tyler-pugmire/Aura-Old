#pragma once
#include "Event.h"
#include "Aura/Core/Input/KeyCodes.h"

namespace Aura
{
    class MouseScroll : public Event
    {
    public:
        MouseScroll() : vertical(0), horizontal(0) {}
        int vertical;
        int horizontal;

        EVENT_CLASS_TYPE(MouseScroll);
        EVENT_CLASS_CATEGORY(Mouse);
    };

    class MouseButtonEvent : public Event
    {
    public:
        MouseButtonEvent() {}
        MouseButton button;

        EVENT_CLASS_TYPE(MouseButtonEvent);
        EVENT_CLASS_CATEGORY(Mouse);
    };

    class MouseButtonDown : public MouseButtonEvent
    {
    public:
        MouseButtonDown() {}

        EVENT_CLASS_TYPE(MouseButtonDown);
        EVENT_CLASS_CATEGORY(Mouse);
    };
}