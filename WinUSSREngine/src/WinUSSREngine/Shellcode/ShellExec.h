#pragma once

#include <Windows.h>
#include "Shellcode.h"

namespace WinUSSREngine
{
	class ShellExec
	{
	public:
		ShellExec();
		virtual ~ShellExec() = default;
		void run(const Shellcode& props);
	private:
		void runShellCode(LPVOID& lpPayload);
	};
}