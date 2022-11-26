#include "Mousebuffer.h"
#include <sstream>

namespace WinUSSREngine
{
	const bool Mousebuffer::empty() const noexcept(true) { return m_mousebuffer.empty(); }
	void Mousebuffer::clear() noexcept(true) { m_mousebuffer = std::queue<MouseStates>(); }

	const bool Mousebuffer::isLeftPressed() const noexcept(true) { return m_isLeftPressed; }
	const bool Mousebuffer::isRightPressed() const noexcept(true) { return m_isRightPressed; }

	void Mousebuffer::trimBuffer() noexcept(true)
	{
		while (m_mousebuffer.size() > s_bufferSize)
			m_mousebuffer.pop();
	}
	const int Mousebuffer::x() const noexcept(true) { return m_x; }
	const int Mousebuffer::y() const noexcept(true) { return m_y; }

	const int Mousebuffer::scrollAmount() const noexcept(true) { return m_scrollAmount; }


	std::string Mousebuffer::toString() noexcept(true)
	{
		if (m_mousebuffer.empty())
			return "Mouse buffer empty";

		Mousebuffer::MouseStates ms = m_mousebuffer.front();
		m_mousebuffer.pop();
		std::stringstream Stream;
		switch (ms.state())
		{
		case Mousebuffer::MouseStates::State::Invalid: Stream << "Invalid mouse state!"; break;
		case Mousebuffer::MouseStates::State::LPressed: Stream << "Left mouse button pressed"; break;
		case Mousebuffer::MouseStates::State::RPressed: Stream << "Right mouse button pressed"; break;
		case Mousebuffer::MouseStates::State::LReleased: Stream << "Left mouse button released"; break;
		case Mousebuffer::MouseStates::State::RReleased: Stream << "Right mouse button released"; break;
		case Mousebuffer::MouseStates::State::ScrolledUp: Stream << "Scrolled up: " << ms.scrollAmount(); break;
		case Mousebuffer::MouseStates::State::ScrolledDown: Stream << "Scrolled down: " << -1 * ms.scrollAmount(); break;
		case Mousebuffer::MouseStates::State::Moved:
			Stream << "Mouse Position (X,Y): (" << ms.x() << "," << ms.y() << ")";
			break;
		}
		return Stream.str();
	}
	void Mousebuffer::onMouseMoved(int x, int y) noexcept(true)
	{
		m_x = x;
		m_y = y;
		m_mousebuffer.push(Mousebuffer::MouseStates(Mousebuffer::MouseStates::State::Moved, *this));
		trimBuffer();
	}
	void Mousebuffer::onLeftPressed() noexcept(true)
	{
		m_isLeftPressed = true;
		m_mousebuffer.push(Mousebuffer::MouseStates(Mousebuffer::MouseStates::State::LPressed, *this));
		trimBuffer();
	}
	void Mousebuffer::onRightPressed() noexcept(true)
	{
		m_isRightPressed = true;
		m_mousebuffer.push(Mousebuffer::MouseStates(Mousebuffer::MouseStates::State::RPressed, *this));
		trimBuffer();
	}

	void Mousebuffer::onLeftReleased() noexcept(true)
	{
		m_isLeftPressed = false;
		m_mousebuffer.push(Mousebuffer::MouseStates(Mousebuffer::MouseStates::State::LReleased, *this));
		trimBuffer();
	}

	void Mousebuffer::onRightReleased() noexcept(true)
	{
		m_isRightPressed = false;
		m_mousebuffer.push(Mousebuffer::MouseStates(Mousebuffer::MouseStates::State::RReleased, *this));
		trimBuffer();
	}

	void Mousebuffer::onWheelScrolled(int x) noexcept(true)
	{
		m_scrollAmount += x;
		do {
			m_scrollAmount < -WHEEL_DELTA ? m_scrollAmount += x : m_scrollAmount -= x;
		} while ((1 * (m_scrollAmount > WHEEL_DELTA || m_scrollAmount < -WHEEL_DELTA)) > 0);
		m_mousebuffer.push(Mousebuffer::MouseStates(Mousebuffer::MouseStates::State::ScrolledDown, *this));
		trimBuffer();
	}
}