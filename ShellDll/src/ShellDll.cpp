#include "Entrypoint.h"
#include <WinUSSREngine/Base/Application.h>

namespace WinUSSREngine
{
	class ShellDll : public Application 
	{
	public:
		ShellDll() : Application() {

		}

		virtual ~ShellDll() = default;
	};

	Application* CreateApp() {
		return new ShellDll();
	}
}