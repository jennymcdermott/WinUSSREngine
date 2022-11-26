#pragma once

#include <fstream>
#include <vector>
#include <sstream>
#include <string>

namespace WinUSSREngine
{
	class NetFile
	{
	public:
		NetFile() = default;
		virtual ~NetFile() = default;

		void readfile(const std::string& file) noexcept(true);
		void writefile(const std::string& file) noexcept(true);
		
		void writeraw(const std::string& bytes) noexcept(true);
		void writeraw(const std::vector<char>& bytes) noexcept(true);

		std::vector<char> raw() noexcept(true);

		const int size() const noexcept(true);
		std::string string() noexcept(true);

		void flush() noexcept(true);
	private:
		std::vector<char> m_rawFileData;
	};
}