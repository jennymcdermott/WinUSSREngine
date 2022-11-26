#include "Ciphers.h"

namespace WinUSSREngine
{
	Ciphers::Ciphers() noexcept(true) { }

	void Ciphers::addmodule(Ref<CipherModule> module) noexcept(true) { m_cipherModules.push_back(std::move(module)); }

	void Ciphers::createNewKeys() noexcept(true)
	{
		for (auto& module : m_cipherModules)
			module->createNewKey();
	}

	void Ciphers::createNewKeys(int msgSize) noexcept(true)
	{
		for (auto& module : m_cipherModules)
			module->createNewKey(msgSize);
	}

	void Ciphers::encrypt(std::string& msg) noexcept(true)
	{
		for (auto& module : m_cipherModules)
			module->encrypt(msg);
	}

	void Ciphers::decrypt(std::string& msg) noexcept(true)
	{
		for (auto& module : m_cipherModules)
			module->decrypt(msg);
	}

	void Ciphers::encrypt(Shellcode& shellcode) noexcept(true)
	{
		for (auto& module : m_cipherModules)
			module->encrypt(shellcode);
	}

	void Ciphers::decrypt(Shellcode& shellcode) noexcept(true)
	{
		for (auto& module : m_cipherModules)
			module->decrypt(shellcode);
	}
}