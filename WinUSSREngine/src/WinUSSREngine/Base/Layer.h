#pragma once

#include "Event.h"

namespace WinUSSREngine
{
	class Layer
	{
	public:
		Layer();
		virtual ~Layer() = default;
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) { }
	};
}