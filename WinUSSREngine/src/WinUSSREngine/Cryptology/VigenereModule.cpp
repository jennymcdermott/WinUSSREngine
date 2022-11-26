#include "VigenereModule.h"

namespace WinUSSREngine
{
	VigenereModule::VigenereModule()
		noexcept(true) : CipherModule() { }

	void VigenereModule::createNewKey() noexcept(true)
	{
		unsigned char rNumSet1 = 97 + (rand() % ((122 + 1) - 97));
		m_currentKey += rNumSet1;
	}

	void VigenereModule::createNewKey(int msgsize) noexcept(true)
	{
		for (unsigned int i = 0; i < msgsize; i++)
		{
			unsigned char rNumSet1 = 97 + (rand() % ((122 + 1) - 97));
			m_currentKey.push_back(rNumSet1);
		}
	}

	void VigenereModule::encrypt(std::string& msg) noexcept(true)
	{
		for (unsigned short i = 0; i < msg.size(); ++i)
			msg[i] = (((msg[i] - 97) + ((int)m_currentKey[i % msg.size()] - 97)) % 26) + 97;
	}

	void VigenereModule::decrypt(std::string& msg) noexcept(true)
	{
		for (unsigned char i = 0; i < msg.size(); i++)
			msg[i] = ((((msg[i] - 97) - ((int)m_currentKey[i % msg.size()] - 97)) + 26) % 26) + 97;
	}

	void VigenereModule::encrypt(Shellcode& shellcode) noexcept(true)
	{
		unsigned char fbtemp = 0u;
		std::vector<unsigned char> temp;

		for (auto bytes : shellcode)
			temp.push_back(bytes);

		std::string key = this->m_currentKey;

		if (shellcode.firstbyte() != 0)
		{
			fbtemp = shellcode.firstbyte();
			fbtemp = (((fbtemp)+((int)key.c_str()[1 % sizeof(int)])) % 26);
		}
		shellcode.reset();


		for (unsigned short i = 0; i < temp.size(); ++i)
			temp.data()[i] = (((temp[i] - 97) + ((int)key.c_str()[i % temp.size()] - 97)) % 26) + 97;

		fbtemp != 0 ? shellcode = Shellcode(fbtemp) : shellcode = Shellcode();
		shellcode.push(temp);
	}

	void VigenereModule::decrypt(Shellcode& shellcode) noexcept(true)
	{
		unsigned char fbtemp = 0u;
		std::vector<unsigned char> temp;
		for (auto bytes : shellcode)
			temp.push_back(bytes);
		std::string key = this->m_currentKey;

		if (shellcode.firstbyte() != 0)
		{
			fbtemp = shellcode.firstbyte();
			fbtemp = (((fbtemp)+((int)key.c_str()[0 % sizeof(int)])) % 26);
		}
		shellcode.reset();

		for (unsigned char i = 0; i < temp.size(); i++)
			temp.data()[i] = ((((temp[i] - 97) - ((int)key.c_str()[i % temp.size()] - 97)) + 26) % 26) + 97;

		fbtemp != 0 ? shellcode.setfirstbyte(fbtemp) : shellcode.setfirstbyte(0);
		shellcode.push(temp);
	}

	void VigenereModule::toFile(const std::string& fname) noexcept(true)
	{
		std::fstream file(fname, std::fstream::out | std::fstream::binary);
		for (auto letter : m_currentKey) file << letter;
		file.close();
	}

	void VigenereModule::fromFile(const std::string& fname) noexcept(true)
	{
		std::fstream file(fname, std::fstream::in | std::fstream::binary);
		std::vector<char> rawData = { std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>() };
		for (auto bytes : rawData) m_currentKey += bytes;
		rawData.clear();
		file.close();
	}

	std::string& VigenereModule::currentKey() noexcept(true) { return m_currentKey; }
}