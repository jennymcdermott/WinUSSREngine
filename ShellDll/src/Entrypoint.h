#pragma once

#if defined(WIN32) | defined(WIN64)
#include <Windows.h>

#include <WinUSSREngine/Base/Application.h>

extern WinUSSREngine::Application* WinUSSREngine::CreateApp();

int __stdcall DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved) 
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		auto SandBox = WinUSSREngine::CreateApp();
		SandBox->run();
		delete SandBox;
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return 0;
}


#else
#error Dlls can only be compiled on Windows!
#endif