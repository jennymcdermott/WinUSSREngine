#pragma once

#include "pch.h"
#include "Packet.h"
#include <queue>
#include <mutex>

namespace WinUSSREngine::TCP
{
	class PacketManager
	{
	private:
		std::queue<Packet> queue_packets;
		std::mutex mutex_packets;
	public:
		void Clear();
		bool HasPendingPackets();
		void Append(Packet p);
		Packet Retrieve();
	};
}