#pragma once

#include "WinUSSREngine/Base/Logfile.h"
#include "WinUSSREngine/Base/TCPBase.h"
#include "WinUSSREngine/Base/AppCmdParser.h"
#include <sstream>
#include <string>
#include <vector>
#include <mutex>

namespace WinUSSREngine::TCP
{
	class Server : public NetObject
	{
	private:
		static Server* s_Instance;

		virtual void setiNetData(const NetiData& data) noexcept override;
		virtual void initNetwork() noexcept override;
		virtual bool peerDisconnected() noexcept override; // This would most likely be a skeleton class method
		bool acceptConnection();
		void disconnectClient(int Id);
	public:
		Server();
		virtual ~Server() { s_Instance = nullptr; }
		//static Server* GetInstance();
		static Server& GetServer();
	private:
		static void ListenerThread();
		static void ClientHandlerThread(int Id);
		static void PacketSenderThread();
	public:
		virtual void create(const NetiData& data = NetiData()) override;
		virtual bool isInitialized() const noexcept override;


		void updateClientState();
		void handleUserInput(std::string& input);
		void listenForNewClient();
		virtual void destroy() noexcept override; // This would most likely be a skeleton class method
	private:
		virtual void closeNetwork() override; // This would most likely be a skeleton class method
		bool isInputAnInt();
		void parseCommand() noexcept;

		bool waitForPeer();
		bool handleSendFile(int Id);
		bool processPacket(int Id, PacketType packet); // This would most likely be a skeleton class method
		bool processParameter(std::string& command, std::string compCommand);

		bool getPacketType(int Id, PacketType& packetType);
	private:
		void handleRemoteCtrlInput();

		// Send Methods, all of these would most likely be skeleton class methods
		bool sendAllData(int Id, char* data, int totalbytes);
		bool sendint32_t(int Id, uint32_t number);
		bool sendPacketType(int Id, const PacketType& packettype);
		void sendStringMsg(int Id, std::string& msg, PacketType packettype);

		// Recieve Methods, all of these would most likely be skeleton class methods
		bool recieveAllData(int Id, char* msg, int totalbytes);
		bool recieveInt32_t(int Id, int32_t& number);
		bool recieveStringMsg(int Id, std::string& Message);
		bool recievePacketType(int Id, PacketType& packettype);
	private:
		WSAWrapper m_Wrapper;
		AppCmdParser m_CmdHandler;
	private:
		NetiData m_Settings;
		bool m_isInitialized = false;
		sockaddr_in m_Addr{};
		int m_AddrLen = sizeof(m_Addr);
		SOCKET m_Socket{ 0 };
		SOCKET m_ClientSocket{ 0 };
		int m_currentSessionID = -1;
		int m_unusedConnections = 0; //# of Inactive Connection Objects that can be reused
		std::vector<std::shared_ptr<Connections>> m_connections;
		std::mutex m_connectionMgr_mutex; //mutex for managing connections (used when a client disconnects)
	private:
		int m_valueFromInput = 0; // used to get a number from input
		std::string m_primary; // used as primary input
		std::string m_secondary; // used as secondary input
		std::vector<std::string> m_inputBuffer; // input buffer
		std::stringstream m_Stream;
		UserInputState m_InputState = UserInputState::AppCmdMode;
		Logfile m_Keylogs;
	};
}