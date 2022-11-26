#pragma once

#include "pch.h"

namespace WinUSSREngine::TCP
{
	enum CPacket
	{
		p_AppCommand,
		p_CMDCommand,
		p_Error
	};

	struct NetiData
	{
		const char* ServerIp = "10.0.0.6";
		unsigned short ServerPort = 4444;
		bool IsPublicBroadcast = false;
	};

	class WSAWrapper
	{
	public:
		void CleanUp() { WSACleanup(); }
		int GetError() { return WSAGetLastError(); }
	};

	enum class UserInputState
	{
		AppCmdMode = 0x00,
		RemoteCtrlMode = 0x01
	};

}