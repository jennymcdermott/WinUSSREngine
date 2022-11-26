#pragma once

#include "WinUSSREngine/Base/Base.h"
#include "WinUSSREngine/Shellcode/Shellcode.h"
#include <string>

/*
*	Design Philosophy:
*	- Base Class
*	- Has the ability to work regardless of what you are using
*	- Has the ability to work with any cipher I use
*	- Can have easy to access keys from interface for sending
*	- None of the keys are hard coded
*	- Keys are sent over network, first with RCA key then other
*	  forms of encryption
*	- Can add onto the module rather easily
*	- Can create new modules with it
*/
namespace WinUSSREngine
{
	class CipherModule
	{
	public:
		virtual void createNewKey() noexcept(true) = 0;
		virtual void createNewKey(int msgsize) noexcept(true) = 0;

		virtual void encrypt(std::string& msg) noexcept(true) = 0; // TCP/Reverse TCP Connections
		virtual void decrypt(std::string& msg) noexcept(true) = 0; // TCP/Reverse TCP Connections

		virtual void encrypt(Shellcode& shellcode) noexcept(true) = 0; // Shellcode Usage
		virtual void decrypt(Shellcode& shellcode) noexcept(true) = 0; // Shellcode Usage

		virtual void toFile(const std::string& fname) noexcept(true) = 0;
		virtual void fromFile(const std::string& fname) noexcept(true) = 0;

		virtual std::string& currentKey() noexcept(true) = 0;
		virtual ~CipherModule() = default;

		static Scope<CipherModule> CreateCaesar() noexcept(true);
		static Scope<CipherModule> CreateVigenere() noexcept(true);
		static Scope<CipherModule> CreateXOR() noexcept(true);
	};
}