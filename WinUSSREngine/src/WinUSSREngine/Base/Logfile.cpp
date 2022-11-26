#include "Logfile.h"
#include <ios>
#include <ctime>
#include <random>
#include <time.h>
#include <string>
#include <ostream>
#include <iomanip>
#include <vector>
#include "Config.h"

namespace WinUSSREngine
{
	void Logfile::initLogging() noexcept
	{
		if (!m_isInitialized)
		{
			// Logs Time
			m_Data.now = time(NULL);
			localtime_s(&m_Data.nowLocal, &m_Data.now);
			if (m_Data.now == NULL)return;
			m_isInitialized = true;
		}
	}
	bool Logfile::isOpen() const noexcept { return m_Data.logfile.is_open(); }

	void Logfile::open(const std::string& filename)
	{
		if (!m_Data.logfile.is_open())
		{
			m_Data.logfile.open(filename, std::ios::out | std::ios::app);
		}
	}

	void Logfile::flush() noexcept { m_Data.logfile.flush(); }


	void Logfile::close() noexcept { m_Data.logfile.close(); }

	Logfile::Logfile() = default;
	Logfile::~Logfile()
	{
		if (m_Data.logfile.is_open()) { close(); }
	}

	template<typename T>
	void Logfile::writeVectorToLog(std::vector<T> iVector)
	{
		for (unsigned int i = 0; i < iVector.size(); i++)
		{
			m_Data.logfile << "["
				<< std::dec << 1900 + m_Data.nowLocal.tm_year
				<< ":" << std::dec << m_Data.nowLocal.tm_mon + 1
				<< ":" << std::dec << m_Data.nowLocal.tm_mday
				<< ":" << std::dec << m_Data.nowLocal.tm_hour
				<< ":" << std::dec << m_Data.nowLocal.tm_min
				<< "]: " << "Vector Quantity (Index: " << i << "): " << iVector[i] << "\n";
		}
	}

	void Logfile::writeCoordsToLog(const CVector& vFloat)
	{
		m_Data.logfile << "["
			<< std::dec << 1900 + m_Data.nowLocal.tm_year
			<< ":" << std::dec << m_Data.nowLocal.tm_mon + 1
			<< ":" << std::dec << m_Data.nowLocal.tm_mday
			<< ":" << std::dec << m_Data.nowLocal.tm_hour
			<< ":" << std::dec << m_Data.nowLocal.tm_min
			<< "]: " << "Coordinate Pair (X,Y): (X: " << vFloat.x << ",Y: " << vFloat.y << ") " << "\n";
	}

	void Logfile::writeMousePosToLog(POINT pt)
	{
		m_Data.logfile << "["
			<< std::dec << 1900 + m_Data.nowLocal.tm_year
			<< ":" << m_Data.nowLocal.tm_mon + 1
			<< ":" << m_Data.nowLocal.tm_mday
			<< ":" << m_Data.nowLocal.tm_hour
			<< ":" << m_Data.nowLocal.tm_min
			<< "]: " << "Mouse Position (X,Y): (X: " << pt.x << ",Y: " << pt.y << ") " << "\n";
	}

	void Logfile::writeToLog(std::string text)
	{
		m_Data.logfile << "["
			<< std::dec << 1900 + m_Data.nowLocal.tm_year
			<< ":" << m_Data.nowLocal.tm_mon + 1
			<< ":" << m_Data.nowLocal.tm_mday
			<< ":" << m_Data.nowLocal.tm_hour
			<< ":" << m_Data.nowLocal.tm_min
			<< "]: " << text << std::dec << "\n";
	}

	void Logfile::writeToLog(LPCSTR text)
	{
		m_Data.logfile << "["
			<< std::dec << 1900 + m_Data.nowLocal.tm_year
			<< ":" << m_Data.nowLocal.tm_mon + 1
			<< ":" << m_Data.nowLocal.tm_mday
			<< ":" << m_Data.nowLocal.tm_hour
			<< ":" << m_Data.nowLocal.tm_min
			<< "]: " << text << std::dec << "\n";
	}

	void Logfile::writeToLog(char text)
	{
		m_Data.logfile << "["
			<< std::dec << 1900 + m_Data.nowLocal.tm_year
			<< ":" << std::dec << m_Data.nowLocal.tm_mon + 1
			<< ":" << std::dec << m_Data.nowLocal.tm_mday
			<< ":" << std::dec << m_Data.nowLocal.tm_hour
			<< ":" << std::dec << m_Data.nowLocal.tm_min
			<< "]: " << text << std::dec << "\n";
	}

	void Logfile::writeHexToLog(LPCSTR text)
	{
		m_Data.logfile << "["
			<< std::dec << 1900 + m_Data.nowLocal.tm_year
			<< ":" << m_Data.nowLocal.tm_mon + 1
			<< ":" << m_Data.nowLocal.tm_mday
			<< ":" << m_Data.nowLocal.tm_hour
			<< ":" << m_Data.nowLocal.tm_min
			<< "]: " << "0x" << std::setfill('0')
			<< std::setw(sizeof(unsigned char) * 2)
			<< std::hex << text << std::dec << "\n";
	}

	void Logfile::writeHexToLog(char text)
	{
		m_Data.logfile << "["
			<< std::dec << 1900 + m_Data.nowLocal.tm_year
			<< ":" << m_Data.nowLocal.tm_mon + 1
			<< ":" << m_Data.nowLocal.tm_mday
			<< ":" << m_Data.nowLocal.tm_hour
			<< ":" << m_Data.nowLocal.tm_min
			<< "]: " << "0x" << std::setfill('0')
			<< std::setw(sizeof(unsigned char) * 2)
			<< std::hex << (int)text << std::dec << "\n";
	}

	char Logfile::toUpper(unsigned char keycode) noexcept { return keycode -= 32; }
	char Logfile::toLower(unsigned char keycode) noexcept { return keycode += 32; }

	bool Logfile::isShift() noexcept { return (GetKeyState(0x10) & 0x8000) != 0; }
	bool Logfile::isCapslock() noexcept { return ((GetKeyState(VK_CAPITAL) & 0x8000) != 0); }

	bool Logfile::keyListened(unsigned char keycode) noexcept
	{
		switch (keycode)
		{
		case VK_SPACE:			writeToLog("[SPACEBAR]");																						break;
		case VK_SHIFT:			writeToLog("[SHIFT]");																							break;
		case VK_LSHIFT:			writeToLog("[LEFTSHIFT]");																						break;
		case VK_RSHIFT:			writeToLog("[RIGHTSHIFT]");																						break;
		case VK_RETURN:			writeToLog("[ENTER]");																							break;
		case VK_BACK:			writeToLog("[BACKSPACE]");																						break;
		case VK_RBUTTON:		writeToLog("[RIGHTMOUSEBUTTON]");																				break;
		case VK_LBUTTON:		writeToLog("[LEFTMOUSEBUTTON]");																				break;
		case VK_TAB:			writeToLog("[TAB]");																							break;
		case VK_CAPITAL:		writeToLog("[CAPSLOCK]");																						break;
		case VK_ESCAPE:			writeToLog("[ESCAPE]");																							break;
		case VK_INSERT:			writeToLog("[INSERT]");																							break;
		case VK_CONTROL:		writeToLog("[CTRL]");																							break;
		case VK_LCONTROL:		writeToLog("[LEFTCONTROL]");																					break;
		case VK_RCONTROL:		writeToLog("[RIGHTCONTROL]");																					break;
		case VK_MENU:			writeToLog("[ALT]");																							break;
		case VK_LMENU:			writeToLog("[LEFTALT]");																						break;
		case VK_RMENU:			writeToLog("[RIGHTALT]");																						break;
		case VK_LEFT:			writeToLog("[LEFTARROW]");																						break;
		case VK_RIGHT:			writeToLog("[RIGHTARROW]");																						break;
		case VK_DOWN:			writeToLog("[DOWNARROW]");																						break;
		case VK_UP:				writeToLog("[UPARROW]");																						break;
		case VK_F1:				writeToLog("[F1]");																								break;
		case VK_F2:				writeToLog("[F2]");																								break;
		case VK_F3:				writeToLog("[F3]");																								break;
		case VK_F4:				writeToLog("[F4]");																								break;
		case VK_F5:				writeToLog("[F5]");																								break;
		case VK_F6:				writeToLog("[F6]");																								break;
		case VK_F7:				writeToLog("[F7]");																								break;
		case VK_F8:				writeToLog("[F8]");																								break;
		case VK_F9:				writeToLog("[F9]");																								break;
		case VK_F10:			writeToLog("[F10]");																							break;
		case VK_F11:			writeToLog("[F11]");																							break;
		case VK_F12:			writeToLog("[F12]");																							break;
		case VK_NUMLOCK:		writeToLog("[NUMLOCK]");																						break;
		case VK_SNAPSHOT:		writeToLog("[PRINTSCREEN]");																					break;
		case VK_MBUTTON:		writeToLog("[MIDDLEMOUSE]");																					break;
		case VK_DECIMAL:		writeToLog("[NUMPAD: . ]");																						break;
		case VK_DIVIDE:			writeToLog("[NUMPAD: / ]");																						break;
		case VK_ADD:			writeToLog("[NUMPAD: + ]");																						break;
		case VK_LWIN:			writeToLog("[LEFTWINKEY]");																						break;
		case VK_RWIN:			writeToLog("[RIGHTWINKEY]");																					break;
		case VK_SUBTRACT:		writeToLog("[NUMPAD: - ]");																						break;
		case VK_NUMPAD0:		if (!GetAsyncKeyState(0x90)) { writeToLog("[NUMPAD: 0 ]"); }
					   else { writeToLog("[NUMPAD: INSERT]"); }			break;
		case VK_NUMPAD1:		if (!GetAsyncKeyState(0x90)) { writeToLog("[NUMPAD: 1 ]"); }
					   else { writeToLog("[NUMPAD: END_KEY]"); }			break;
		case VK_NUMPAD2:		if (!GetAsyncKeyState(0x90)) { writeToLog("[NUMPAD: 2 ]"); }
					   else { writeToLog("[NUMPAD: DOWNARROW]"); }		break;
		case VK_NUMPAD3:		if (!GetAsyncKeyState(0x90)) { writeToLog("[NUMPAD: 3 ]"); }
					   else { writeToLog("[NUMPAD: PAGE_DOWN]"); }		break;
		case VK_NUMPAD4:		if (!GetAsyncKeyState(0x90)) { writeToLog("[NUMPAD: 4 ]"); }
					   else { writeToLog("[NUMPAD: LEFTARROW]"); }		break;
		case VK_NUMPAD5:		if (!GetAsyncKeyState(0x90)) { writeToLog("[NUMPAD: 5 ]"); }
					   else { writeToLog("[NUMPAD: NONE]"); }				break;
		case VK_NUMPAD6:		if (!GetAsyncKeyState(0x90)) { writeToLog("[NUMPAD: 6 ]"); }
					   else { writeToLog("[NUMPAD: RIGHTARROW]"); }		break;
		case VK_NUMPAD7:		if (!GetAsyncKeyState(0x90)) { writeToLog("[NUMPAD: 7 ]"); }
					   else { writeToLog("[NUMPAD: HOME]"); }				break;
		case VK_NUMPAD8:		if (!GetAsyncKeyState(0x90)) { writeToLog("[NUMPAD: 8 ]"); }
					   else { writeToLog("[NUMPAD: UPARROW]"); }			break;
		case VK_NUMPAD9:		if (!GetAsyncKeyState(0x90)) { writeToLog("[NUMPAD: 9 ]"); }
					   else { writeToLog("[NUMPAD: PAGE_UP]"); }			break;
		case 48: if (GetAsyncKeyState(0x10)) writeToLog(")"); else writeToLog("0");																break;
		case 49: if (GetAsyncKeyState(0x10)) writeToLog("!"); else writeToLog("1");																break;
		case 50: if (GetAsyncKeyState(0x10)) writeToLog("@"); else writeToLog("2");																break;
		case 51: if (GetAsyncKeyState(0x10)) writeToLog("#"); else writeToLog("3");																break;
		case 52: if (GetAsyncKeyState(0x10)) writeToLog("$"); else writeToLog("4");																break;
		case 53: if (GetAsyncKeyState(0x10)) writeToLog("%"); else writeToLog("5");																break;
		case 54: if (GetAsyncKeyState(0x10)) writeToLog("^"); else writeToLog("6");																break;
		case 55: if (GetAsyncKeyState(0x10)) writeToLog("&"); else writeToLog("7");																break;
		case 56: if (GetAsyncKeyState(0x10)) writeToLog("*"); else writeToLog("8");																break;
		case 57: if (GetAsyncKeyState(0x10)) writeToLog("("); else writeToLog("9");																break;
		case VK_OEM_1: if (GetAsyncKeyState(0x10)) writeToLog(":"); else writeToLog(";");																break;
		case VK_OEM_PLUS:  if (GetAsyncKeyState(0x10)) writeToLog("+"); else writeToLog("=");															break;
		case VK_OEM_MINUS: if (GetAsyncKeyState(0x10)) writeToLog("_"); else writeToLog("-");															break;
		case VK_OEM_COMMA: if (GetAsyncKeyState(0x10)) writeToLog("<"); else writeToLog(",");															break;
		case VK_OEM_PERIOD: if (GetAsyncKeyState(0x10)) writeToLog(">"); else writeToLog(".");															break;
		case VK_OEM_2: if (GetAsyncKeyState(0x10)) writeToLog("?"); else writeToLog("/");															break;
		case VK_OEM_3: if (GetAsyncKeyState(0x10)) writeToLog("~"); else writeToLog("`");															break;
		case VK_OEM_4: if (GetAsyncKeyState(0x10)) writeToLog("{"); else writeToLog("[");															break;
		case VK_OEM_5: if (GetAsyncKeyState(0x10)) writeToLog("|"); else writeToLog("\\");															break;
		case VK_OEM_6: if (GetAsyncKeyState(0x10)) writeToLog("}"); else writeToLog("]");															break;
		case VK_OEM_7: if (GetAsyncKeyState(0x10)) writeToLog("\""); else writeToLog("\'");															break;
		case 255: writeToLog("No key mapped!");																									break;
		default: return false;
		}
		return true;
	}

	bool Logfile::isNumeric(unsigned char keycode) const noexcept { return (keycode > 57 && keycode < 65); }
	bool Logfile::isAlphabetic(unsigned char keycode) const noexcept { return (keycode > 64 && keycode < 91); }

	// this is useful for checking if a special key is held so that way it can checked without being logged twice
	bool Logfile::isSpecialKey(unsigned char keycode) noexcept
	{
		switch (keycode)
		{
		case VK_SPACE:
		case VK_SHIFT:
		case VK_LSHIFT:
		case VK_RSHIFT:
		case VK_RETURN:
		case VK_BACK:
		case VK_RBUTTON:
		case VK_LBUTTON:
		case VK_TAB:
		case VK_CAPITAL:
		case VK_ESCAPE:
		case VK_INSERT:
		case VK_CONTROL:
		case VK_LCONTROL:
		case VK_RCONTROL:
		case VK_MENU:
		case VK_LMENU:
		case VK_RMENU:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_DOWN:
		case VK_UP:
		case VK_F1:
		case VK_F2:
		case VK_F3:
		case VK_F4:
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F10:
		case VK_F11:
		case VK_F12:
		case VK_NUMLOCK:
		case VK_SNAPSHOT:
		case VK_MBUTTON:
		case VK_DECIMAL:
		case VK_DIVIDE:
		case VK_ADD:
		case VK_LWIN:
		case VK_RWIN:
		case VK_SUBTRACT:
		case VK_NUMPAD0:
		case VK_NUMPAD1:
		case VK_NUMPAD3:
		case VK_NUMPAD4:
		case VK_NUMPAD5:
		case VK_NUMPAD6:
		case VK_NUMPAD7:
		case VK_NUMPAD8:
		case VK_NUMPAD9:
		case VK_SEPARATOR:
		case VK_SLEEP:
		case VK_MEDIA_NEXT_TRACK:
		case VK_MEDIA_PLAY_PAUSE:
		case VK_MEDIA_PREV_TRACK:
		case VK_VOLUME_DOWN:
		case VK_VOLUME_MUTE:
		case VK_VOLUME_UP:
		case 61:
		case 188:
		case 190:
		case 191:
		case 192:
		case 219:
		case 220:
		case 221:
		case 222:
		case 255: return true; break;
		default: return false;
		}
	}

}