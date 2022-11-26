#include "Mouse.h"
#include "WinUSSREngine/Base/MouseEvent.h"

namespace WinUSSREngine
{
	Mouse* Mouse::s_Instance = nullptr;

	LRESULT Mouse::MouseHook(int p_code, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (p_code != HC_ACTION)
			return CallNextHookEx(s_Instance->m_hook, p_code, wParam, lParam);

		MSLLHOOKSTRUCT* mb_struct = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

		// redundant but useful for ignoring injected values
		if (mb_struct->flags & LLMHF_INJECTED || mb_struct->flags & LLMHF_LOWER_IL_INJECTED)
			return CallNextHookEx(s_Instance->m_hook, p_code, wParam, lParam);

		switch (wParam)
		{
		case WM_MOUSEMOVE:
		{
			MouseMovedEvent Event(mb_struct->pt.x, mb_struct->pt.y);
			s_Instance->m_callbackFn(Event);
			if (s_Instance->isDisabled())return 0x45;
			break;
		}
		case WM_MOUSEWHEEL:
		{
			MouseScrollEvent Event(HIWORD(mb_struct->mouseData));
			s_Instance->m_callbackFn(Event);
			if (s_Instance->isDisabled())return 0x45;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			MouseButtonPressedEvent Event(GET_XBUTTON_WPARAM(mb_struct->mouseData));
			s_Instance->m_callbackFn(Event);
			if (s_Instance->isDisabled())return 0x45;
			break;
		}
		case WM_LBUTTONUP:
		{
			MouseButtonReleasedEvent Event(GET_XBUTTON_WPARAM(mb_struct->mouseData));
			s_Instance->m_callbackFn(Event);
			if (s_Instance->isDisabled())return 0x45;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			MouseButtonPressedEvent Event(GET_XBUTTON_WPARAM(mb_struct->mouseData));
			s_Instance->m_callbackFn(Event);
			if (s_Instance->isDisabled())return 0x45;
			break;
		}
		case WM_RBUTTONUP:
		{
			MouseButtonReleasedEvent Event(GET_XBUTTON_WPARAM(mb_struct->mouseData));
			s_Instance->m_callbackFn(Event);
			if (s_Instance->isDisabled())return 0x45;
			break;
		}
		}
		return CallNextHookEx(s_Instance->m_hook, p_code, wParam, lParam);
	}
	Mouse::Mouse() noexcept { s_Instance = this; }

	Mouse::~Mouse() noexcept { s_Instance = nullptr; }

	const bool Mouse::isHooked() const noexcept { return m_isHooked; }
	void Mouse::enableInput() noexcept { m_disabled = false; }
	void Mouse::disableInput() noexcept { m_disabled = true; }
	const bool Mouse::isDisabled() const noexcept { return m_disabled; }

	void Mouse::hook() noexcept
	{
		if (m_isHooked)return;
		m_hook = SetWindowsHookEx(WH_MOUSE_LL, MouseHook, 0, 0);
		m_isHooked = true;
	}

	void Mouse::unhook() noexcept
	{
		if (!m_isHooked) return;
		UnhookWindowsHookEx(m_hook);
		m_isHooked = false;
	}

	void Mouse::eventCallbackFn(const EventCallbackFn& callback) noexcept { m_callbackFn = callback; }
}