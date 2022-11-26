#include "Packet.h"

namespace WinUSSREngine::TCP
{
	Packet::Packet() { /* Idk, fuck you I guess :P */ }

	Packet::Packet(char* buf, int size)
	{
		m_Buffer = buf;
		m_Size = size;
	}

	Packet::Packet(const Packet& packet)
	{
		m_Size = packet.m_Size;
		m_Buffer = new char[m_Size];
		memcpy(m_Buffer, packet.m_Buffer, m_Size);
	}
}