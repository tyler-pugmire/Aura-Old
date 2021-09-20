#pragma once

#include <string>
#include <functional>

namespace Aura
{

    enum class EventType
    {
        None,
        WindowClosed, WindowLostFocus,
        TextInput, KeyDown, KeyUp,
        MouseScroll, MouseButtonEvent, MouseButtonDown
    };

    enum class EventCategory
    {
        None,
        Window = 1,
        Key = 1 << 1,
        Mouse = 1 << 2
    };

    inline bool operator&(EventCategory lhs, EventCategory rhs)
    {
        return std::underlying_type<EventCategory>::type(lhs) & std::underlying_type<EventCategory>::type(rhs);
    }

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual EventCategory GetCategoryFlags() const override { return EventCategory::##category; }

    class Event
    {
    public:
        bool handled = false;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual EventCategory GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& event)
            : event(event)
        {
        }

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if (event.GetEventType() == T::GetStaticType())
            {
                event.handled = func(*(T*)&event);
                return true;
            }
            return false;
        }
    private:
        Event& event;
    };
}

