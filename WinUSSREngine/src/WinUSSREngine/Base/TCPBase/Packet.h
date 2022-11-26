#pragma once

namespace WinUSSREngine::TCP
{
	class Packet
	{
	public:
		Packet();
		Packet(char* buffer, int size);
		Packet(const Packet& packet);

		char* m_Buffer;
		int m_Size;
	};
}