#pragma once
#include "Event.h"

namespace Aura
{
    class WindowClosedEvent : public Event
    {
    public:
        WindowClosedEvent() {}

        EVENT_CLASS_TYPE(WindowClosed)
        EVENT_CLASS_CATEGORY(Window)
    };
}