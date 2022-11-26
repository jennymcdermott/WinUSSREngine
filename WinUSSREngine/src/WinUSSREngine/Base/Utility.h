#pragma once

#include "pch.h"

namespace WinUSSREngine
{
	class Utility
	{
	public:
		static bool FileExists(std::string file);
		static bool DirectoryExists(std::string dirName);
		static void OutputMsg(std::string message, int msgType);
		static void handleError(int errType, bool errSevere);

		static std::string GetExeFileName();
		static std::string GetExeFilePath();
		static std::string GetAppDataPath();
	};
}