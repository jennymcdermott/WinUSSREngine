#include "Shellcode.h"

namespace WinUSSREngine
{
	Shellcode::Shellcode(unsigned char firstbyte)
		noexcept(true) : m_firstbyteOriginal(firstbyte) { }

	void Shellcode::readFile(const std::string& name) noexcept(true)
	{
		std::fstream file(name, std::fstream::in | std::fstream::binary);
		!m_bytes.empty() ? m_bytes = std::vector<unsigned char>() : m_bytes;
		m_bytes = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	}

	void Shellcode::push(const std::vector<unsigned char>& code) noexcept(true)
	{
		!m_bytes.empty() ? m_bytes = std::vector<unsigned char>() : m_bytes;
		for (int i = 0; i < code.size(); ++i)
			m_bytes.push_back(code[i]);
	}

	void Shellcode::push(unsigned char* carray, unsigned short int size) noexcept(true)
	{
		!m_bytes.empty() ? m_bytes = std::vector<unsigned char>() : m_bytes;
		for (unsigned short int i = 0; i < size; i++)
			m_bytes.push_back(carray[i]);
	}

	const unsigned short int Shellcode::size() const noexcept(true) { return m_bytes.size(); }
	bool Shellcode::empty() const noexcept(true) { return m_bytes.empty(); }
	void Shellcode::setfirstbyte(unsigned char byte) noexcept(true) { m_firstbyteOriginal = byte; }

	void Shellcode::reset() noexcept(true)
	{
		m_firstbyteOriginal ? m_firstbyteOriginal = 0u : m_firstbyteOriginal;
		m_bytes.clear();
	}

	unsigned char Shellcode::firstbyte() const noexcept(true) { return m_firstbyteOriginal; }
	const std::vector<unsigned char>& Shellcode::bytes() const noexcept(true) { return m_bytes; }

	void Shellcode::toBytes(const std::string& str) noexcept(true)
	{
		!m_bytes.empty() ? m_bytes = std::vector<unsigned char>() : m_bytes;

		for (int i = 0; i < str.size(); ++i)
			m_bytes.push_back(str.data()[i]);
	}

	void Shellcode::toFile(const std::string& name) noexcept(true) 
	{
		std::fstream file(name, std::fstream::out | std::fstream::binary);
		m_firstbyteOriginal > 0 ? m_bytes[0] = m_firstbyteOriginal : m_firstbyteOriginal;
		for (auto byte : m_bytes) file << byte;
		file.close();
	}

	
	std::string Shellcode::string() noexcept(true) {
		std::stringstream Stream;
		m_firstbyteOriginal > 0 ? m_bytes[0] = m_firstbyteOriginal : m_firstbyteOriginal;
		for (auto byte : m_bytes) Stream << byte;
		return Stream.str();
	}


	std::vector<unsigned char>::iterator Shellcode::begin() { return m_bytes.begin(); }
	std::vector<unsigned char>::iterator Shellcode::end() { return m_bytes.end(); }
}