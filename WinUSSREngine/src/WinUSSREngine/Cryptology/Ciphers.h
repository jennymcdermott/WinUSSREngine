#pragma once

#include "CipherModule.h"
#include "WinUSSREngine/Base/Base.h"
#include "WinUSSREngine/Shellcode/Shellcode.h"
#include <vector>
#include <string>

namespace WinUSSREngine
{
	class Ciphers
	{
	public:
		Ciphers() noexcept(true);
		virtual ~Ciphers() = default;

		void addmodule(Ref<CipherModule> module) noexcept(true);

		void createNewKeys() noexcept(true);
		void createNewKeys(int msgSize) noexcept(true);

		void encrypt(std::string& msg) noexcept(true); // TCP/Reverse TCP Connections
		void decrypt(std::string& msg) noexcept(true); // TCP/Reverse TCP Connections

		void encrypt(Shellcode& shellcode) noexcept(true); // Shellcode Usage
		void decrypt(Shellcode& shellcode) noexcept(true); // Shellcode Usage
	private:
		std::vector<Ref<CipherModule>> m_cipherModules;
	};
}