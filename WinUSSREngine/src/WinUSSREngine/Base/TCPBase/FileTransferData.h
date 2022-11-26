#pragma once

#include "pch.h"
#include <fstream>

namespace WinUSSREngine::TCP
{
	struct FileTransferData
	{
		static const int buffersize = 8192;
		std::string fileName; //For when receiving files
		int bytesWritten{0}; //For when receiving files
		std::streampos fileOffset; //For when reading a file
		std::streampos fileSize; //For when reading a file
		int remainingBytes{0}; //For when reading a file
		std::ifstream infileStream; //For reading a file that is being sent
		std::ofstream outfileStream; //For writing a file that is being received
		char buffer[buffersize]{0}; //buffer used for when sending or receiving to optimize constantly reallocating buffers
	};
}