#include "XorModule.h"

namespace WinUSSREngine
{
	XorModule::XorModule() noexcept(true)
		: CipherModule() { }
	void XorModule::createNewKey() noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		int randNumSet1 = 97 + (rand() % ((122 + 1) - 97));

		m_xorkey.push_back(randNumSet1);
	}

	void XorModule::createNewKey(int msgsize) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		for (unsigned int i = 0; i < msgsize; i++)
		{
			int randNumSet1 = 97 + (rand() % ((122 + 1) - 97));
			
			m_xorkey.push_back(randNumSet1);
		}
	}

	void XorModule::encrypt(std::string& msg) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		for (unsigned short int i = 0; i < msg.size(); ++i)
			msg.data()[i] = msg.data()[i] ^ m_xorkey.data()[i];
	}

	void XorModule::decrypt(std::string& msg) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		for (unsigned short int i = 0; i < msg.size(); ++i)
			msg.data()[i] = msg.data()[i] ^ m_xorkey.data()[i];
	}

	void XorModule::encrypt(Shellcode& shellcode) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		unsigned char fbtemp = 0u;
		shellcode.firstbyte() != 0x00u ?
			fbtemp = shellcode.firstbyte() ^ m_xorkey[0] : fbtemp;

		std::vector<unsigned char> temp;
		for (unsigned short int i = 0; shellcode.size(); ++i)
			temp.push_back(shellcode.bytes()[i] ^ m_xorkey[i]);

		fbtemp != 0 ? shellcode.setfirstbyte(fbtemp) : shellcode.setfirstbyte(0);
		shellcode.push(temp);
	}

	void XorModule::decrypt(Shellcode& shellcode) noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		unsigned char fbtemp = 0u;
		shellcode.firstbyte() != 0x00u ?
			fbtemp = shellcode.firstbyte() ^ m_xorkey[0] : fbtemp;

		std::vector<unsigned char> temp;
		for (unsigned short int i = 0; shellcode.size(); ++i)
			temp.push_back(shellcode.bytes()[i] ^ m_xorkey[i]);

		fbtemp != 0 ? shellcode.setfirstbyte(fbtemp) : shellcode.setfirstbyte(0);
		shellcode.push(temp);
	}

	void XorModule::toFile(const std::string& fname) noexcept(true)
	{
		std::fstream file(fname, std::fstream::out | std::fstream::binary);
		for (auto letter : m_xorkey) file << letter;
		file.close();
	}

	void XorModule::fromFile(const std::string& fname) noexcept(true)
	{
		std::fstream file(fname, std::fstream::in | std::fstream::binary);
		std::vector<char> rawData = { std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>() };
		for (auto bytes : rawData) m_xorkey += bytes;
		rawData.clear();
		file.close();
	}

	std::string& XorModule::currentKey() noexcept(true) { return m_xorkey; }
}