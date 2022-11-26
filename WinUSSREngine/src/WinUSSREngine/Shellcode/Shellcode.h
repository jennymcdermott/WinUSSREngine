#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

namespace WinUSSREngine
{
	class Shellcode
	{
	public:
		Shellcode() = default;
		Shellcode(unsigned char firstbyte) noexcept(true);

		void readFile(const std::string& name) noexcept(true);

		void push(const std::vector<unsigned char>& code) noexcept(true);
		void push(unsigned char* carray, unsigned short int size) noexcept(true);

		const unsigned short int size() const noexcept(true);
		bool empty() const noexcept(true);

		void setfirstbyte(unsigned char byte) noexcept(true);

		void reset() noexcept;
		unsigned char firstbyte() const noexcept(true);

		const std::vector<unsigned char>& bytes() const noexcept(true);

		void toBytes(const std::string& str) noexcept(true);

		void toFile(const std::string& name) noexcept(true);

		std::vector<unsigned char>::iterator begin();
		std::vector<unsigned char>::iterator end();

		std::string string() noexcept(true);
	private:
		std::vector<unsigned char> m_bytes;
		unsigned char m_firstbyteOriginal = 0u;
	};
}