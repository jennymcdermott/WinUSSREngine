#pragma once

#include "Event.h"
#include "LayerStack.h"
#include "WinUSSREngine/Windows/Keyboard.h"
#include "WinUSSREngine/Windows/Mouse.h"

namespace WinUSSREngine
{
	class Application
	{
	private:
		static Application* s_Instance;
	public:
		Application();
		virtual ~Application();

		void PushLayer(Layer* layer);

		void OnEvent(Event& e);

		int run() noexcept(true);
		void close() noexcept(true);

		Keyboard& keyboard() noexcept(true);
		//Mouse& mouse() noexcept(true);

		static Application& GetApp() noexcept(true);
	private:
		LayerStack m_layerStack;
		bool m_isClosed = false;
		Keyboard m_keyboard;
	};

	Application* CreateApp(); // TODO: Define somewhere else.
}