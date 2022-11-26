#pragma once

#include "CipherModule.h"
#include "WinUSSREngine/Shellcode/Shellcode.h"

namespace WinUSSREngine
{
	class VigenereModule : public CipherModule
	{
	public:
		VigenereModule() noexcept(true);
		virtual ~VigenereModule() override = default;

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
		std::string m_currentKey;
	};
}