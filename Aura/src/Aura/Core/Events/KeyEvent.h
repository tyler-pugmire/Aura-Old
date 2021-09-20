#pragma once
#include "Event.h"

namespace Aura
{
    class TextInput : public Event
    {
    public:
        TextInput() {}
        char text[32] = {};
        EVENT_CLASS_TYPE(TextInput);
        EVENT_CLASS_CATEGORY(Key);
    };

    class KeyDown : public Event
    {
    public:
        KeyDown() {}

        EVENT_CLASS_TYPE(KeyDown)
            EVENT_CLASS_CATEGORY(Mouse)
    };

    class KeyUp : public Event
    {
    public:
        KeyUp() {}

        EVENT_CLASS_TYPE(KeyUp)
            EVENT_CLASS_CATEGORY(Mouse)
    };
}