#pragma once

#include <string>
#include <vector>

namespace WinUSSREngine
{
	class StringTools {
	public:
		static std::string toBinary(const std::string& str);
		static std::string toString(const char* str, int size);
		static std::string toString(const std::string& str);
		static std::string toString(const std::vector<int>& vec);
	};
}