#pragma once

#include "AppCmds.h"

#include <type_traits> 
#include <fstream>
#include <variant>
#include <memory>
#include <vector>
#include <string>

namespace WinUSSREngine
{
	class AppCmdParser
	{
	public:
	public:
		AppCmdParser();
		virtual ~AppCmdParser();

		void setPrefix(const std::string& prefix);
		void addCmdsByName(const std::string& cmdName);		// For creating custom commands
		void addCmdsByList(std::vector<std::string> cmdList); // For adding a vector of commands

		void writeCommandsToFile(const std::string& fname);
		void readCommandsFromFile(const std::string& fname);

		void clear();
		void reset();

		int determineCommand(std::string cmdName);

		int commandById(int id);
		int commandIdByName(const std::string& cmdName);
		std::string commandNameById(int Id);

		const std::string& prefix() const noexcept;
	private:
		void initCmdHandler();
	private:
		struct CData
		{
			std::string cmdName = "";
			int cmdId = 0;
		};
		std::vector<CData> m_cmdBuffer;
	private:
		int m_CmdIds = 0;
		std::string m_prefix = "$";
		std::fstream m_cmdsFile;
	};
}