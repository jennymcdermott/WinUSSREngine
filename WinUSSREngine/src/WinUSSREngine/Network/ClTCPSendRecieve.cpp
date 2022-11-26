#include "ClientTCP.h"

namespace WinUSSREngine::TCP
{
	bool Client::sendAllData(char* data, int totalbytes)
	{
		int BytesRecieved = 0;
		if (BytesRecieved < totalbytes)
		{
			int RecvCheck = send(m_svSocket, data + BytesRecieved, totalbytes - BytesRecieved, NULL);
			if (RecvCheck == SOCKET_ERROR)
			{
				return false;
			}
			BytesRecieved += RecvCheck;
		}
		return true;
	}

	bool Client::recieveAllData(char* data, int totalbytes)
	{
		int BytesRecieved = 0;
		if (BytesRecieved < totalbytes)
		{
			int RetCheck = recv(m_svSocket, data + BytesRecieved, totalbytes - BytesRecieved, NULL);
			if (RetCheck == SOCKET_ERROR)
			{
				return false;
			}
			BytesRecieved += RetCheck; //Add to total bytes sent
		}
		return true;
	}

	bool Client::sendint32_t(int32_t _int32_T)
	{
		_int32_T = htonl(_int32_T);
		if (!sendAllData((char*)&_int32_T, sizeof(int32_t)))return false;
		return true;
	}

	bool Client::recieveInt32_t(int32_t& _int32_T)
	{
		if (!recieveAllData((char*)&_int32_T, sizeof(int32_t)))
			return false;
		_int32_T = ntohl(_int32_T);
		return true;
	}

	bool Client::sendPacketType(PacketType Packetyype)
	{
		if (!sendint32_t((int32_t)Packetyype))return false;
		return true;
	}

	bool Client::getPacketType(PacketType& Packetyype)
	{
		int packettype;
		if (!recieveInt32_t(packettype))//Try to receive PacketType type... If PacketType type fails to be recv'd
			return false; //Return false: PacketType type not successfully received
		Packetyype = (PacketType)packettype;
		return true;
	}

	bool Client::sendStringMsg(std::string string, PacketType packetType)
	{
		if (!sendPacketType(packetType)) //Send PacketType type: Chat Message, If sending PacketType type fails...
			return false; //Return false: Failed to send string
		int32_t bufferlength = string.size(); //Find string buffer length
		if (!sendint32_t(bufferlength)) //Send length of string buffer, If sending buffer length fails...
			return false; //Return false: Failed to send string buffer length
		//Return false: Failed to send string buffer
		//Try to send string buffer... If buffer fails to send,
		if (!sendAllData((char*)string.c_str(), bufferlength)) return false;
		return true;					//Return true: string successfully sent
	}

	bool Client::recieveStringMsg(std::string& string)
	{
		int32_t bufferlength; //Holds length of the message
		if (!recieveInt32_t(bufferlength)) //Get length of buffer and store it in variable: bufferlength
			return false; //If get int fails, return false
		char* buffer = new char[bufferlength + 1]; //Allocate buffer
		buffer[bufferlength] = '\0'; //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
		if (!recieveAllData(buffer, bufferlength)) //receive message and store the message in buffer array. If buffer fails to be received...
		{
			delete[] buffer; //delete buffer to prevent memory leak
			return false;
		}
		string = buffer;
		delete[] buffer;
		return true;
	}
}