#include "Entrypoint.h"
#include <WinUSSREngine/Base/Application.h>
#include "DllLayer/DllLayer.h"

namespace WinUSSREngine
{
	class ShellDll : public Application 
	{
	public:
		ShellDll() : Application() {
			PushLayer(new DllLayer()); // Forgot to push a dll layer again
		}

		virtual ~ShellDll() = default;
	};

	Application* CreateApp() {
		return new ShellDll();
	}
}