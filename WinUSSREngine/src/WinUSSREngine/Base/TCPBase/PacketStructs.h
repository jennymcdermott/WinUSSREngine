#pragma once

#include "Packet.h"
#include "PacketType.h"

namespace WinUSSREngine::TCP
{
	class Message
	{
	public:
		Message(std::string msg);
		Packet toPacket(PacketType _packettype); //Converts ChatMessage to packet
	private:
		std::string message;
	};
}