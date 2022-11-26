#pragma once

#include <Windows.h>

namespace WinUSSREngine
{
	// This is a way for trojans to bypass Virtual Machines
	class AntiVM
	{
	public:
		// This is the constructor
		AntiVM();

		/*
		*	This will look for DLL files that only exist in
		*	machines that run in virtualbox or vmware.
		*/
		bool isVMGuestModules();
		
		/*
		*	The idea was that the mouse position
		*	would be checked and if it wasn't moved
		*	for a bit of time, then it would detect a vm.
		*/
		bool isMouseSamePos();
	};
}