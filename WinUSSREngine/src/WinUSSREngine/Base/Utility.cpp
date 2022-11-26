#include "Utility.h"
#include <iostream>
#include <ShlObj_core.h>
#include "StringTools.h"
#include "WinUSSREngine/Network/ClientTCP.h"

namespace WinUSSREngine
{
	std::string Utility::GetExeFileName()
	{
		char buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		return std::string(buffer);
	}

	std::string Utility::GetExeFilePath()
	{
		std::string f = GetExeFileName();
		return f.substr(0, f.find_last_of("\\/"));
	}

	std::string Utility::GetAppDataPath()
	{
		char buffer[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, buffer);
		return std::string(buffer);
	}

	void Utility::OutputMsg(std::string message, int msgType)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (msgType)
		{
		case 1:
			SetConsoleTextAttribute(hConsole, 10);
			std::cout << message << "\r\n";
			SetConsoleTextAttribute(hConsole, 7);
			break;
		case 2:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << "[ERROR] " << message << "\r\n";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		case 3:
			//SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
			std::cout << message;
			//SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		}
	}

	void Utility::handleError(int errType, bool errSevere)
	{
		if (errSevere)
		{
			// TODO: Restart self
		}
		else
		{
			switch (errType)
			{
			case 1:
			{
				TCP::Client::GetClient().sendStringMsg("General error", TCP::PacketType::Warning);
				break;
			}
			case 2:
			{
				TCP::Client::GetClient().sendStringMsg("CMD error", TCP::PacketType::Warning);
				break;
			}
			case 3:
			{
				TCP::Client::GetClient().sendStringMsg("Networking error", TCP::PacketType::Warning);
				break;
			}
			}
		}
	}


	bool Utility::FileExists(std::string path)
	{
		FILE* file;
		if (fopen_s(&file, path.c_str(), "r") == 0)
		{
			fclose(file);
			return true;
		}
		else
			return false;
	}
	bool Utility::DirectoryExists(std::string dirName)
	{
		DWORD attribs = ::GetFileAttributesA(dirName.c_str());
		return (attribs == INVALID_FILE_ATTRIBUTES);
	}
}