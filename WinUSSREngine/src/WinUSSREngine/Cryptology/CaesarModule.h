#pragma once

#include "CipherModule.h"
#include <string>
#include <mutex>
#include <thread>

namespace WinUSSREngine
{
	class CaesarModule : public CipherModule
	{
	public:
		CaesarModule() noexcept(true);
		virtual ~CaesarModule() override = default;

		virtual void createNewKey() noexcept(true) override;
		virtual void createNewKey(int msgsize) noexcept(true) override;

		virtual void encrypt(std::string& msg) noexcept(true) override; // TCP/Reverse TCP Connections
		virtual void decrypt(std::string& msg) noexcept(true) override; // TCP/Reverse TCP Connections

		virtual void encrypt(Shellcode& shellcode) noexcept(true) override; // Shellcode Usage
		virtual void decrypt(Shellcode& shellcode) noexcept(true) override; // Shellcode Usage

		virtual void toFile(const std::string& fname) noexcept(true) override;
		virtual void fromFile(const std::string& fname) noexcept(true) override;

		virtual std::string& currentKey() noexcept override;
	private:
		std::string m_caesarkey;
		std::mutex m_mutex;
	};
}