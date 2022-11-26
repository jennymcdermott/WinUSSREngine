#include "ServerTCP.h"

namespace WinUSSREngine::TCP
{
	void Server::sendStringMsg(int Id, std::string& msg, PacketType packettype)
	{
		Message message(msg);
		if (Id == -2)
		{
			for (int i = 0; i < m_connections.size(); i++)
			{
				m_connections[i]->pm.Append(message.toPacket(packettype));
			}
		}
		else
		{
			m_connections[Id]->pm.Append(message.toPacket(packettype));
		}
	}

	bool Server::sendint32_t(int Id, uint32_t number)
	{
		number = htonl(number);
		if (!sendAllData(Id, (char*)&number, sizeof(int32_t)))return false;
		return true;
	}

	bool Server::sendAllData(int Id, char* data, int totalbytes)
	{
		int BytesSent = 0;
		while (BytesSent < totalbytes)
		{
			int SendCheck = send(m_connections[Id]->socket(), data + BytesSent, totalbytes - BytesSent, NULL);
			if (SendCheck == SOCKET_ERROR) return false;
			BytesSent += SendCheck;
		}
		return true;
	}

	bool Server::recieveAllData(int Id, char* msg, int totalbytes)
	{
		int BytesRecieved = 0;
		while (BytesRecieved < totalbytes)
		{
			int RecvCheck = recv(m_connections[Id]->socket(), msg, totalbytes - BytesRecieved, NULL);
			if (RecvCheck == SOCKET_ERROR)return false;
			BytesRecieved += RecvCheck;
		}
		return true;
	}

	bool Server::recieveInt32_t(int Id, int32_t& number)
	{
		if (!recieveAllData(Id, (char*)&number, sizeof(int32_t)))return false;
		number = ntohl(number);
		return true;
	}

	bool Server::sendPacketType(int Id, const PacketType& packettype)
	{
		if (!sendint32_t(Id, (int32_t)packettype))return false;
		return true;
	}

	bool Server::recieveStringMsg(int Id, std::string& Message)
	{
		int32_t bufferlength;						   //Holds length of the message
		if (!recieveInt32_t(Id, bufferlength))		   //Get length of buffer and store it in variable: bufferlength
			return false;							   //If get int fails, return false
		char* buffer = new char[bufferlength + 1];     //Allocate buffer
		buffer[bufferlength] = '\0';				   //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
		if (!recieveAllData(Id, buffer, bufferlength)) //receive message and store the message in buffer array. If buffer fails to be received...
		{
			delete[] buffer; //delete buffer to prevent memory leak
			return false;    //return false: Fails to receive string buffer
		}
		Message = buffer; //set string to received buffer message
		delete[] buffer;  //Deallocate buffer memory (cleanup to prevent memory leak)
		return true;	  //Return true if we were successful in retrieving the string
	}

	bool Server::getPacketType(int Id, PacketType& packetType)
	{
		int Packettype;
		if (!recieveInt32_t(Id, Packettype))
			return false;
		packetType = (PacketType)Packettype;
		return true;
	}

	bool Server::recievePacketType(int Id, PacketType& packettype)
	{
		int packetType;
		if (!recieveInt32_t(Id, packetType))
			return false;
		packettype = (PacketType)packetType;
		return true;
	}
}