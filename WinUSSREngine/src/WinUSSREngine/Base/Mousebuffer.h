#pragma once

#include "WinUSSREngine/Base/MouseEvent.h"
#include <queue>
#include <bitset>
#include <string>
#include <mutex>
/*
* Mouse output flags:
* - Determines which things to output based on which flag is active
* - upper bits are used as the flags, lower bits will be reserved if
*	I need some extra behind the scenes flags that will be used
* - Purpose: Allows for user to specify what they want to log without having
*	too many boolean values. (i.e. save memory, give options)
*/
#define MOUSE_OUTPUT_NONE		 0b00000000 // 0x0 << 4
#define MOUSE_OUTPUT_BUTTON		 0b00010000 // 0x1 << 4  
#define MOUSE_OUTPUT_POSITION	 0b00100000 // 0x2 << 4  
#define MOUSE_OUTPUT_SCROLL		 0b01000000 // 0x4 << 4  
#define MOUSE_OUTPUT_ALL		 0b01110000 // 0x7 << 4

namespace WinUSSREngine
{
	class Mousebuffer
	{
	private:
		class MouseStates
		{
		public:
			enum class State
			{
				Invalid,
				LPressed,
				RPressed,
				LReleased,
				RReleased,
				ScrolledUp,
				ScrolledDown,
				Moved
			};
		private:
			State m_state;
			int m_x, m_y, m_scrollAmount;
			bool m_isLeftPressed, m_isRightPressed;
		public:
			MouseStates() noexcept(true)
				: m_state(State::Invalid), m_x(0u), m_y(0u), m_scrollAmount(0u), m_isLeftPressed(false), m_isRightPressed(false) { }

			MouseStates(const State& state, const Mousebuffer& parent) noexcept(true)
				: m_state(state), m_x(parent.m_x), m_y(parent.m_y), m_scrollAmount(parent.m_scrollAmount),
				m_isLeftPressed(parent.m_isLeftPressed), m_isRightPressed(parent.m_isRightPressed) { }

			const bool isInvalid() const noexcept(true) { m_state == State::Invalid; }
			std::pair<int, int> position() const noexcept(true) { return { m_x,m_y }; }
			const int x() const noexcept(true) { return m_x; }
			const int y() const noexcept(true) { return m_y; }
			const int scrollAmount() const noexcept(true) { return m_scrollAmount; }

			const bool isLeftPressed() const noexcept(true) { return m_state == State::LPressed; }
			const bool isRightPressed() const noexcept(true) { return m_state == State::RPressed; }

			const bool isLeftReleased() const noexcept(true) { return m_state == State::LReleased; }
			const bool isRightReleased() const noexcept(true) { return m_state == State::RReleased; }
			State state() const noexcept(true) { return m_state; }
		};
	public:
		const bool empty() const noexcept(true);
		void clear() noexcept(true);

		void outputFlags(unsigned char flags) noexcept;

		const int x() const noexcept(true);
		const int y() const noexcept(true);
		const int scrollAmount() const noexcept(true);

		const bool isLeftPressed() const noexcept(true);
		const bool isRightPressed() const noexcept(true);

		std::string toString() noexcept;

		void onMouseMoved(int x, int y) noexcept(true);
		
		void onLeftPressed() noexcept(true);
		void onRightPressed() noexcept(true);

		void onLeftReleased() noexcept(true);
		void onRightReleased() noexcept(true);

		void onWheelScrolled(int x) noexcept(true);
	private:
		std::string mouseButton(unsigned char code) noexcept(true);
		void trimBuffer() noexcept(true);
	private:
		std::mutex m_bufferMutex;
		static constexpr const unsigned char s_bufferSize = 16u;
		int m_x, m_y, m_scrollAmount;
		bool m_isLeftPressed, m_isRightPressed;
		std::queue<MouseStates> m_mousebuffer;
		unsigned char m_outputFlags = 0;
	};
}