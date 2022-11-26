#pragma once

#include "Event.h"

namespace WinUSSREngine
{
	class KeyEvent : public Event
	{
	public:
		unsigned char keycode() const { return m_keycode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(unsigned char keycode) noexcept : m_keycode(keycode) {}
		unsigned char m_keycode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(unsigned char keycode)
			noexcept : KeyEvent(keycode) {}

		std::string toString() const override
		{
			std::stringstream Stream;
			Stream << "KeyPressed Event:" << m_keycode;
			return Stream.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(unsigned char keycode)
			noexcept : KeyEvent(keycode) {}

		std::string toString() const override
		{
			std::stringstream Stream;
			Stream << "KeyReleased Event:" << m_keycode;
			return Stream.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};
}