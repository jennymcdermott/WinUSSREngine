#pragma once

#include <Windows.h>

namespace WinUSSREngine
{
	class RemoteCmd
	{
	public: //Public functions
		RemoteCmd(std::string path);

		static void cmdThread(void* pvPath);

		std::string readCMD();
		void writeCMD(std::string command);

	public:	//Public variables
		static RemoteCmd* cmdptr;
		static bool cmdOpen;

	private: //Private functions
		void createChildProcess(std::string path);

	private:	//variables
		HANDLE g_hChildStd_IN_Rd = NULL;
		HANDLE g_hChildStd_IN_Wr = NULL;
		HANDLE g_hChildStd_OUT_Rd = NULL;
		HANDLE g_hChildStd_OUT_Wr = NULL;

		static HANDLE g_hChildProcess;
		static HANDLE g_hChildThread;

		SECURITY_ATTRIBUTES saAttr;
	};
}