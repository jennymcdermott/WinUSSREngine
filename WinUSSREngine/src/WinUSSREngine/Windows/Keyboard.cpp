#include "Keyboard.h"
#include "WinUSSREngine/Base/KeyEvent.h"

//----------------------------------------------------------------------------------------------------------
//----------------------------------- Low Level Keyboard Hook ----------------------------------------------
//----------------------------------------------------------------------------------------------------------

namespace WinUSSREngine
{
	Keyboard* Keyboard::s_Instance = nullptr;
	Keyboard::Keyboard() noexcept : m_hook(0) { s_Instance = this; }
	Keyboard::~Keyboard() noexcept { s_Instance = nullptr; }

	void Keyboard::enableInput() noexcept { m_disableInput = false; }
	void Keyboard::disableInput() noexcept { m_disableInput = true; }
	void Keyboard::eventCallbackFn(const EventCallbackFn& callback) noexcept { m_callbackFn = callback; }
	const bool Keyboard::isHooked() const noexcept { return m_isHooked; }

	void Keyboard::hook() noexcept
	{
		if (!m_isHooked)
		{
			m_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, 0, 0);
			m_hook ? m_isHooked = true : m_isHooked = false;
		}
	}

	void Keyboard::unhook() noexcept
	{
		if (m_isHooked)
		{
			UnhookWindowsHookEx(m_hook);
			m_isHooked = false;
		}

	}

	const bool Keyboard::isDisabled() const noexcept { return m_disableInput; }

	LRESULT Keyboard::KeyboardHook(int p_code, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (p_code != HC_ACTION)
			return CallNextHookEx(s_Instance->m_hook, p_code, wParam, lParam);

		KBDLLHOOKSTRUCT* kb_struct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
		if (kb_struct->flags & LLMHF_INJECTED || kb_struct->flags & LLMHF_LOWER_IL_INJECTED)
			return CallNextHookEx(s_Instance->m_hook, p_code, wParam, lParam);

		switch (wParam)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			KeyPressedEvent Event(kb_struct->vkCode);
			s_Instance->m_callbackFn(Event);
			if (s_Instance->isDisabled())return 0x45;
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			KeyReleasedEvent Event(kb_struct->vkCode);
			s_Instance->m_callbackFn(Event);
			if (s_Instance->isDisabled()) return 0x45;
			break;
		}
		}
		return CallNextHookEx(s_Instance->m_hook, p_code, wParam, lParam);
	}
}