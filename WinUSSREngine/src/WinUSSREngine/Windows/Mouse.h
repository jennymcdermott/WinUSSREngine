#pragma once

#include "WinUSSREngine/Base/Event.h"
#include <Windows.h>

namespace WinUSSREngine
{
	class Mouse
	{
	private:
		static Mouse* s_Instance;
		using EventCallbackFn = std::function<void(Event&)>;
	public:
		Mouse() noexcept;
		virtual ~Mouse() noexcept;

		void hook() noexcept;
		void unhook() noexcept;

		const bool isDisabled() const noexcept;
		const bool isHooked() const noexcept;
		void enableInput() noexcept;
		void disableInput() noexcept;

		void eventCallbackFn(const EventCallbackFn& callback) noexcept;
	private:
		static LRESULT CALLBACK MouseHook(int p_code, WPARAM wParam, LPARAM lParam) noexcept;
	private:
		HHOOK m_hook{ 0 };
		EventCallbackFn m_callbackFn;
		bool m_isHooked = false;
		bool m_disabled = false;
	};
}
