#include "Keybuffer.h"

namespace WinUSSREngine
{
	bool Keybuffer::isCapslock() const noexcept { return (GetKeyState(VK_CAPITAL) > 0); }
	bool Keybuffer::isShift() const noexcept { return (m_keystates[VK_LSHIFT] || m_keystates[VK_RSHIFT]); }

	unsigned char Keybuffer::toLower(unsigned char keycode) noexcept { return keycode += 32; }
	unsigned char Keybuffer::toUpper(unsigned char keycode) noexcept { return keycode -= 32; }

	std::string Keybuffer::toString() noexcept
	{
		std::lock_guard<std::mutex> lock(m_bufferMutex);
		while (!m_keybuffer.empty())
		{
			unsigned char keycode = m_keybuffer.front();
			m_keybuffer.pop();

			if (isSpecialKey(keycode))
				return m_toString;

			!(isShift() || isCapslock()) ?
				keycode = toLower(keycode) : keycode;

			m_toString.push_back(keycode);
		}
		m_toString.append("\0");
		return m_toString;
	}

	void Keybuffer::onKeyPressed(unsigned char keycode) noexcept
	{
		std::lock_guard<std::mutex> lock(m_bufferMutex);
		m_keystates[keycode] = true;
		m_keybuffer.push(keycode);
		trimBuffer(m_keybuffer);
	}

	void Keybuffer::onKeyReleased(unsigned char keycode) noexcept
	{
		std::lock_guard<std::mutex> lock(m_bufferMutex);
		m_keystates[keycode] = false;
	}

	void Keybuffer::clear() noexcept
	{
		std::lock_guard<std::mutex> lock(m_bufferMutex);
		m_keybuffer = std::queue<unsigned char>();
		m_toString.clear();
	}

	void Keybuffer::flush() noexcept
	{
		std::lock_guard<std::mutex> lock(m_bufferMutex);
		m_keybuffer = std::queue<unsigned char>();
		m_toString.clear();
		m_keystates.reset();
	}

	bool Keybuffer::empty() const noexcept { return m_keybuffer.empty(); }

	bool Keybuffer::isAlphabetic(unsigned char keycode) const noexcept
	{
		return keycode *= ((keycode > 64 && keycode < 91)
			|| (keycode > 96 && keycode < 123)); // 97 = a, 122 = z
	}

	bool Keybuffer::isSpecialKey(unsigned char keycode) noexcept
	{
		switch (keycode)
		{
		case VK_SPACE:			m_toString = ("[SPACEBAR]");																						break;
		case VK_SHIFT:			m_toString = ("[SHIFT]");																						break;
		case VK_LSHIFT:			m_toString = ("[LEFTSHIFT]");																						break;
		case VK_RSHIFT:			m_toString = ("[RIGHTSHIFT]");																						break;
		case VK_RETURN:			m_toString = ("[ENTER]");																							break;
		case VK_BACK:			m_toString = ("[BACKSPACE]");																						break;
		case VK_RBUTTON:		m_toString = ("[RIGHTMOUSEBUTTON]");																				break;
		case VK_LBUTTON:		m_toString = ("[LEFTMOUSEBUTTON]");																				break;
		case VK_TAB:			m_toString = ("[TAB]");																							break;
		case VK_CAPITAL:		m_toString = ("[CAPSLOCK]");																						break;
		case VK_ESCAPE:			m_toString = ("[ESCAPE]");																							break;
		case VK_INSERT:			m_toString = ("[INSERT]");																							break;
		case VK_CONTROL:		m_toString = ("[CTRL]");																							break;
		case VK_LCONTROL:		m_toString = ("[LEFTCONTROL]");																					break;
		case VK_RCONTROL:		m_toString = ("[RIGHTCONTROL]");																					break;
		case VK_MENU:			m_toString = ("[ALT]");																							break;
		case VK_LMENU:			m_toString = ("[LEFTALT]");																						break;
		case VK_RMENU:			m_toString = ("[RIGHTALT]");																						break;
		case VK_LEFT:			m_toString = ("[LEFTARROW]");																						break;
		case VK_RIGHT:			m_toString = ("[RIGHTARROW]");																						break;
		case VK_DOWN:			m_toString = ("[DOWNARROW]");																						break;
		case VK_UP:				m_toString = ("[UPARROW]");																						break;
		case VK_F1:				m_toString = ("[F1]");																								break;
		case VK_F2:				m_toString = ("[F2]");																								break;
		case VK_F3:				m_toString = ("[F3]");																								break;
		case VK_F4:				m_toString = ("[F4]");																								break;
		case VK_F5:				m_toString = ("[F5]");																								break;
		case VK_F6:				m_toString = ("[F6]");																								break;
		case VK_F7:				m_toString = ("[F7]");																								break;
		case VK_F8:				m_toString = ("[F8]");																								break;
		case VK_F9:				m_toString = ("[F9]");																								break;
		case VK_F10:			m_toString = ("[F10]");																							break;
		case VK_F11:			m_toString = ("[F11]");																							break;
		case VK_F12:			m_toString = ("[F12]");																							break;
		case VK_NUMLOCK:		m_toString = ("[NUMLOCK]");																						break;
		case VK_SNAPSHOT:		m_toString = ("[PRINTSCREEN]");																					break;
		case VK_MBUTTON:		m_toString = ("[MIDDLEMOUSE]");																					break;
		case VK_DECIMAL:		m_toString = ("[NUMPAD: . ]");																						break;
		case VK_DIVIDE:			m_toString = ("[NUMPAD: / ]");																						break;
		case VK_ADD:			m_toString = ("[NUMPAD: + ]");																						break;
		case VK_LWIN:			m_toString = ("[LEFTWINKEY]");																						break;
		case VK_RWIN:			m_toString = ("[RIGHTWINKEY]");																					break;
		case VK_SUBTRACT:		m_toString = ("[NUMPAD: - ]");																						break;
		case VK_NUMPAD0:		if (!GetAsyncKeyState(0x90)) { m_toString = ("[NUMPAD: 0 ]"); }
					   else { m_toString = ("[NUMPAD: INSERT]"); }			break;
		case VK_NUMPAD1:		if (!GetAsyncKeyState(0x90)) { m_toString = ("[NUMPAD: 1 ]"); }
					   else { m_toString = ("[NUMPAD: END_KEY]"); }			break;
		case VK_NUMPAD2:		if (!GetAsyncKeyState(0x90)) { m_toString = ("[NUMPAD: 2 ]"); }
					   else { m_toString = ("[NUMPAD: DOWNARROW]"); }		break;
		case VK_NUMPAD3:		if (!GetAsyncKeyState(0x90)) { m_toString = ("[NUMPAD: 3 ]"); }
					   else { m_toString = ("[NUMPAD: PAGE_DOWN]"); }		break;
		case VK_NUMPAD4:		if (!GetAsyncKeyState(0x90)) { m_toString = ("[NUMPAD: 4 ]"); }
					   else { m_toString = ("[NUMPAD: LEFTARROW]"); }		break;
		case VK_NUMPAD5:		if (!GetAsyncKeyState(0x90)) { m_toString = ("[NUMPAD: 5 ]"); }
					   else { m_toString = ("[NUMPAD: NONE]"); }				break;
		case VK_NUMPAD6:		if (!GetAsyncKeyState(0x90)) { m_toString = ("[NUMPAD: 6 ]"); }
					   else { m_toString = ("[NUMPAD: RIGHTARROW]"); }		break;
		case VK_NUMPAD7:		if (!GetAsyncKeyState(0x90)) { m_toString = ("[NUMPAD: 7 ]"); }
					   else { m_toString = ("[NUMPAD: HOME]"); }				break;
		case VK_NUMPAD8:		if (!GetAsyncKeyState(0x90)) { m_toString = ("[NUMPAD: 8 ]"); }
					   else { m_toString = ("[NUMPAD: UPARROW]"); }			break;
		case VK_NUMPAD9:		if (!GetAsyncKeyState(0x90)) { m_toString = ("[NUMPAD: 9 ]"); }
					   else { m_toString = ("[NUMPAD: PAGE_UP]"); }			break;
		case 48: if (GetAsyncKeyState(VK_SHIFT) & 0x8000) m_toString = (")"); else m_toString = ("0");																break;
		case 49: if (GetAsyncKeyState(0x10) & 0x8000) m_toString = ("!"); else m_toString = ("1");																break;
		case 50: if (GetAsyncKeyState(0x10) & 0x8000) m_toString = ("@"); else m_toString = ("2");																break;
		case 51: if (GetAsyncKeyState(0x10) & 0x8000) m_toString = ("#"); else m_toString = ("3");																break;
		case 52: if (GetAsyncKeyState(0x10) & 0x8000) m_toString = ("$"); else m_toString = ("4");																break;
		case 53: if (GetAsyncKeyState(0x10) & 0x8000) m_toString = ("%"); else m_toString = ("5");																break;
		case 54: if (GetAsyncKeyState(0x10) & 0x8000) m_toString = ("^"); else m_toString = ("6");																break;
		case 55: if (GetAsyncKeyState(0x10) & 0x8000) m_toString = ("&"); else m_toString = ("7");																break;
		case 56: if (GetAsyncKeyState(0x10) & 0x8000) m_toString = ("*"); else m_toString = ("8");																break;
		case 57: if (GetAsyncKeyState(0x10) & 0x8000) m_toString = ("("); else m_toString = ("9");																break;
		case VK_OEM_1: if (GetAsyncKeyState(0x10)) m_toString = (":"); else m_toString = (";");														break;
		case VK_OEM_PLUS:  if (GetAsyncKeyState(0x10)) m_toString = ("+"); else m_toString = ("=");													break;
		case VK_OEM_MINUS: if (GetAsyncKeyState(0x10)) m_toString = ("_"); else m_toString = ("-");													break;
		case VK_OEM_COMMA: if (GetAsyncKeyState(0x10)) m_toString = ("<"); else m_toString = (",");													break;
		case VK_OEM_PERIOD: if (GetAsyncKeyState(0x10)) m_toString = (">"); else m_toString = (".");													break;
		case VK_OEM_2: if (GetAsyncKeyState(0x10)) m_toString = ("?"); else m_toString = ("/");														break;
		case VK_OEM_3: if (GetAsyncKeyState(0x10)) m_toString = ("~"); else m_toString = ("`");														break;
		case VK_OEM_4: if (GetAsyncKeyState(0x10)) m_toString = ("{"); else m_toString = ("[");														break;
		case VK_OEM_5: if (GetAsyncKeyState(0x10)) m_toString = ("|"); else m_toString = ("\\");														break;
		case VK_OEM_6: if (GetAsyncKeyState(0x10)) m_toString = ("}"); else m_toString = ("]");														break;
		case VK_OEM_7: if (GetAsyncKeyState(0x10)) m_toString = ("\""); else m_toString = ("\'");														break;
		default: return false;
		}
		return true;
	}

	template<typename T>
	void Keybuffer::trimBuffer(std::queue<T> buffer)
	{
		while (buffer.size() > m_maxbuffSize)
			buffer.pop();
	}
}