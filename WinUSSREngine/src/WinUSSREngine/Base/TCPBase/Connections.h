#pragma once

#include "FileTransferData.h"
#include "NetTypes.h"
#include "PacketType.h"
#include "PacketManager.h"
#include "PacketStructs.h"
#include <Windows.h>
#include <string>

namespace WinUSSREngine::TCP
{
	class Connections
	{
	private:
		unsigned char m_ClientId = 0;
		bool m_isActive = true;
		std::string m_Name;
		SOCKET m_Socket;
	public:
		Connections(SOCKET socket) : m_Socket(socket) { }

		SOCKET& socket() noexcept { return m_Socket; }

		void setId(unsigned char id) noexcept { m_ClientId = id; }
		void setname(const std::string& name) noexcept { m_Name = name; }
		void setActive() noexcept { m_isActive = !m_isActive; }

		bool isActive() const noexcept { return m_isActive; }
		const std::string& name() const noexcept { return m_Name; }
		unsigned char clientId() const noexcept { return m_ClientId; }

		FileTransferData file; //Object that contains information about our file that is being sent to the client from this server
		PacketManager pm; //Packet Manager for outgoing data for this connection
	};
}