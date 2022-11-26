#include "CipherModule.h"
#include "CaesarModule.h"
#include "VigenereModule.h"
#include "XorModule.h"

namespace WinUSSREngine
{
	Scope<CipherModule> CipherModule::CreateCaesar() noexcept(true) {
		return CreateScope<CaesarModule>();
	}

	Scope<CipherModule> CipherModule::CreateVigenere() noexcept(true) {
		return CreateScope<VigenereModule>();
	}

	Scope<CipherModule> CipherModule::CreateXOR() noexcept(true) {
		return CreateScope<XorModule>();
	}
}