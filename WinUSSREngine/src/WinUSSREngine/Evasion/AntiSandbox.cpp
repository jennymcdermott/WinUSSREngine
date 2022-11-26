#include "AntiSandbox.h"

namespace WinUSSREngine
{
	AntiSandbox::AntiSandbox() { }
	bool AntiSandbox::foundSandboxModules()
	{
		// This will try to locate sandboxie's dll file, if it finds it inside the proc, it quits.
		if (GetModuleHandle("sbiedll.dll") != NULL || GetModuleHandle("SbieDll.dll") != NULL
			|| GetModuleHandle("avghookx.dll") != NULL || GetModuleHandle("avghooka.dll") != NULL
			|| GetModuleHandle("snxhk.dll") != NULL || GetModuleHandle("api_log.dll") != NULL
			|| GetModuleHandle("dir_watch.dll") != NULL || GetModuleHandle("pstorec.dll") != NULL
			|| GetModuleHandle("vmcheck.dll") != NULL || GetModuleHandle("wpespy.dll") != NULL
			|| GetModuleHandle("cmdvrt32.dll") != NULL || GetModuleHandle("cmdvrt64.dll") != NULL) return true;
		return false;
	}

	// This is a very primitive way to check for a debugger.
	bool AntiSandbox::canDebugbreak()
	{
		__try { DebugBreak(); }
		__except (GetExceptionCode() == EXCEPTION_BREAKPOINT ?
			EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			return false;
		};
		return true;
	}
}