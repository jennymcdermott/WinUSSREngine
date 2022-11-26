#include "CaesarModule.h"

namespace WinUSSREngine
{
	CaesarModule::CaesarModule() noexcept(true)
		: CipherModule() { }
	void CaesarModule::createNewKey() noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		unsigned char rNumSet1 = 97 + (rand() % ((122 + 1) - 97));
		m_caesarkey.push_back(rNumSet1);
	}

	void CaesarModule::createNewKey(int msgsize) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		for (unsigned int i = 0; i < msgsize; i++)
		{
			unsigned char rNumSet1 = 97 + (rand() % ((122 + 1) - 97));
			m_caesarkey.push_back(rNumSet1);
		}
	}

	void CaesarModule::encrypt(std::string& msg) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		for (unsigned short int i = 0; i < msg.size(); ++i)
			msg[i] += m_caesarkey[i];
	}

	void CaesarModule::decrypt(std::string& msg) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		for (unsigned short int i = 0; i < msg.size(); ++i)
			msg[i] -= m_caesarkey[i];
	}

	void CaesarModule::encrypt(Shellcode& shellcode) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		unsigned char fbtemp =
			(shellcode.firstbyte() + m_caesarkey[0]) * (shellcode.firstbyte() != 0x00u);
		std::vector<unsigned char> temp;
		for (unsigned short int i = 0; shellcode.size(); ++i)
			temp.push_back(shellcode.bytes()[i] + m_caesarkey[i]);

		fbtemp != 0 ? shellcode.setfirstbyte(fbtemp) : shellcode.setfirstbyte(0);
		shellcode.push(temp);
	}

	void CaesarModule::decrypt(Shellcode& shellcode) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		unsigned char fbtemp = // if the first byte var is equal to zero, then this value is also zero
			(shellcode.firstbyte() + m_caesarkey[0]) * (shellcode.firstbyte() != 0x00u);

		std::vector<unsigned char> temp;
		for (unsigned short int i = 0; shellcode.size(); ++i)
			temp.push_back(shellcode.bytes()[i] - m_caesarkey[i]);

		fbtemp != 0 ? shellcode.setfirstbyte(fbtemp) : shellcode.setfirstbyte(0);
		shellcode.push(temp);
	}

	void CaesarModule::toFile(const std::string& fname) noexcept(true)
	{
		std::fstream file(fname, std::fstream::out | std::fstream::binary);
		for (auto letter : m_caesarkey) file << letter;
		file.close();
	}

	void CaesarModule::fromFile(const std::string& fname) noexcept(true)
	{
		std::fstream file(fname, std::fstream::in | std::fstream::binary);
		std::vector<char> rawData = { std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>() };
		for (auto bytes : rawData) m_caesarkey += bytes;
		rawData.clear();
		file.close();
	}

	std::string& CaesarModule::currentKey() noexcept(true) { return m_caesarkey; }
}