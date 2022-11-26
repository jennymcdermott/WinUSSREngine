#pragma once

/*
* This is being used not as a traditional event handler,
* rather as a way to adhoc shit into existence so that 
* nobody else has to deal with figuring this shit out.
* 
* The AppEvent functionality was stripped since it isn't
* needed here.
* 
*/

#include "Base.h"
#include <functional>
#include <ostream>
#include <sstream>
#include <string>

namespace WinUSSREngine
{
	enum class EventType
	{
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled, MouseEnter, MouseLeave
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryInput		 = BIT(0),
		EventCategoryKeyboard	 = BIT(1),
		EventCategoryMouse		 = BIT(2),
		EventCategoryMouseButton = BIT(3)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
												virtual EventType eventType() const override { return GetStaticType(); }\
												virtual const char* name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int categoryFlags() const override { return category; }

	// Base class for the event handler 
	class Event
	{
	public:
		virtual ~Event() = default;
		bool Handled = false;

		virtual EventType eventType() const = 0;
		virtual const char* name() const = 0;
		virtual int categoryFlags() const = 0;
		virtual std::string toString() const { return name(); }

		// This sees if an event is handled or not.
		bool isInCategory(EventCategory C) { return categoryFlags() & C; }
	};

	// This is the event dispatcher.
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : m_Events(event) { }
		// Error C2440: if EventDispatcher wasn't set up like how it is below.
		template<typename T, typename F>
		bool dispatchEvents(const F& function)
		{
			if (m_Events.eventType() == T::GetStaticType())
			{
				m_Events.Handled |= function(static_cast<T&>(m_Events));
				return true;
			}
			return false;
		}
	private:
		Event& m_Events;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& E) { return os << E.toString(); }
}