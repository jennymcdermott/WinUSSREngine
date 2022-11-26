#pragma once

#include "WinUSSREngine/Base/Event.h"
#include <Windows.h>

namespace WinUSSREngine
{
	class Keyboard
	{
	private:
		static Keyboard* s_Instance;
		using EventCallbackFn = std::function<void(Event&)>;
	public:
		Keyboard() noexcept;
		virtual ~Keyboard() noexcept;

		const bool isDisabled() const noexcept;
		const bool isHooked() const noexcept;

		void hook() noexcept;
		void unhook() noexcept;

		void enableInput() noexcept;
		void disableInput() noexcept;

		void eventCallbackFn(const EventCallbackFn& callback) noexcept;
	private:
		static LRESULT CALLBACK KeyboardHook(int p_code, WPARAM wParam, LPARAM lParam) noexcept;
	private:
		HHOOK m_hook;
		EventCallbackFn m_callbackFn;
		bool m_disableInput = false;
		bool m_isHooked = false;
	};
}