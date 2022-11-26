#include "AntiVirtualMachine.h"

namespace WinUSSREngine
{
	// This is where all the AntiVM stuff gets implemented.
	AntiVM::AntiVM() { /* ctor */ }
	bool AntiVM::isVMGuestModules()
	{
		// If it doesn't get an error while loading this dll found on VirtualBox machines, the program exits.
		// If it doesn't get an error while loading this dll found on VMWare machines, the program exits.
		if (LoadLibrary("VBoxHook.dll") != NULL
			|| LoadLibrary("vm3dgl.dll") != NULL) return true;
		return false;
	}

	bool AntiVM::isMouseSamePos()
	{
		POINT Point1, Point2;
		if (GetCursorPos(&Point1) == ERROR)return false;

		WaitForSingleObject(nullptr, 30000);
		if (GetCursorPos(&Point2) == ERROR) return false;

		return Point1.x == Point2.x && Point1.y == Point2.y;
	}
}