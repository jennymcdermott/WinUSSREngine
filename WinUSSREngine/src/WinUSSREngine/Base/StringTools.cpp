#include "StringTools.h"

namespace WinUSSREngine
{
	std::string StringTools::toBinary(const std::string& str)
	{
		std::string temp = std::string();
		for (auto bytes : str) {
			bytes -= 0x20 * (bytes > 0x60 && bytes < 0x7B);
			bytes -= 0x40 * (bytes > 0x40 && bytes < 0x5B);
			temp += bytes;
		}
		return temp;
	}

	std::string StringTools::toString(const char* str, int size)
	{
		std::string temp = std::string();
		for (int i = 0; i < size; ++i) {
			char byte = str[i];
			byte += 0x41 * (byte > 0x00 && byte < 0x1B);
			byte += 0x20 * (byte > 0x40 && byte < 0x59);
			temp += byte;
		}
		return temp;
	}

	std::string StringTools::toString(const std::string& str)
	{
		std::string temp = std::string();
		for (auto bytes : str) {
			bytes += 0x40 * (bytes > 0x00 && bytes < 0x1B);
			bytes += 0x20 * (bytes > 0x40 && bytes < 0x5B);
			temp += bytes;
		}
		return temp;
	}

	std::string StringTools::toString(const std::vector<int>& vec)
	{
		std::string temp = std::string();
		for (auto bytes : vec) {
			bytes += 0x40 * (bytes > 0x00 && bytes < 0x1B);
			bytes += 0x20 * (bytes > 0x40 && bytes < 0x5B);
			temp += bytes;
		}
		return temp;
	}
}