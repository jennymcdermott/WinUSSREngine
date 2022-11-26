#pragma once

#include "Keybuffer.h"
#include "Logfile.h"

namespace WinUSSREngine
{
	class Keylogger
	{
	public:
		Keylogger() noexcept;

		void start(const std::string& name) noexcept;
		void start() noexcept;
		void log() noexcept;
		void stop() noexcept;

		bool OnKeyPressed(unsigned char keycode);
		bool OnKeyReleased(unsigned char keycode);
	private:
		void init() noexcept;
	private:
		Keybuffer m_keybuffer;
		Logfile m_keylogs;
	};
}