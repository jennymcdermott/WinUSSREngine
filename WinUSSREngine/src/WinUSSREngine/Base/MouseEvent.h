#pragma once

#include "Event.h"

namespace WinUSSREngine
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int x, int y) : m_xPos(x), m_yPos(y) {}

		int x() const { return m_xPos; }
		int y() const { return m_yPos; }

		std::string toString() const override {
			std::stringstream Stream;
			Stream << "MouseMoved Event (X,Y): (" << m_xPos << "," << m_yPos << ")";
			return Stream.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		int m_xPos, m_yPos;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(int scrollAmount)
			: m_scrollAmount(scrollAmount) { }

		int scrollDelta() const { return m_scrollAmount; }

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		int m_scrollAmount;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline unsigned char mouseButton() const { }
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(unsigned char button) : m_button(button) { }
		unsigned char m_button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(unsigned char button) : MouseButtonEvent(button) {  }

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(unsigned char button) : MouseButtonEvent(button) {  }

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class MouseEnterEvent : public Event
	{
	public:
		MouseEnterEvent() : Event() { }

		EVENT_CLASS_TYPE(MouseEnter)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class MouseLeaveEvent : public Event
	{
	public:
		MouseLeaveEvent() : Event() { }

		EVENT_CLASS_TYPE(MouseLeave)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};
}