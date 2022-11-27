#pragma once

#include <WinUSSREngine/Base/Layer.h>

namespace WinUSSREngine
{
	class DllLayer : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& e) override;
	};
}