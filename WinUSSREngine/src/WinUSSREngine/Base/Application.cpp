#include "Application.h"
#include <assert.h>

namespace WinUSSREngine
{
	Application* Application::s_Instance = nullptr;
	Application& Application::GetApp() noexcept(true) { return *s_Instance; }
	
	std::optional<int> Application::ProcessMessages() noexcept(true)
	{
		MSG msg = { 0 };
		while (GetMessage(&msg, NULL, 0, 0) != 0)
		{
			if (msg.message == WM_QUIT)
				msg.wParam;

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return {};
	}

	Application::Application() 
	{ 
		assert("App already running" && s_Instance != nullptr);
		s_Instance = this; 
		m_keyboard.eventCallbackFn(BIND_EVENT_FN(OnEvent));
		m_keyboard.hook();
	}

	Application::~Application() 
	{
		m_keyboard.isDisabled() ?
			m_keyboard.enableInput() : m_keyboard.enableInput();

		if (m_keyboard.isHooked())
			m_keyboard.unhook();

		s_Instance = nullptr;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) 
	{
		for (auto IT = m_layerStack.rbegin(); IT != m_layerStack.rend(); ++IT)
		{
			(*IT)->OnEvent(e);
			if (e.Handled)break;
		}
	}

	Keyboard& Application::keyboard() noexcept(true) { return m_keyboard; }

	void Application::close() noexcept(true) { m_isClosed = true; }

	int Application::run() noexcept(true) {
		while (!m_isClosed)
		{
			if (const auto ecode = ProcessMessages())
				return *ecode;

			for (auto& layer : m_layerStack)
				layer->OnUpdate();
		}
		return 0x45; // lol nice
	}
}