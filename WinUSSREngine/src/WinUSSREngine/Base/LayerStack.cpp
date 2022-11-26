#include "LayerStack.h"

namespace WinUSSREngine
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayersInsertIndex, layer);
		m_LayersInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* Overlay)
	{
		m_Layers.emplace_back(Overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto IT = std::find(m_Layers.begin(), m_Layers.begin() + m_LayersInsertIndex, layer);
		if (IT != m_Layers.begin() + m_LayersInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(IT);
			m_LayersInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* Overlay)
	{
		auto IT = std::find(m_Layers.begin() + m_LayersInsertIndex, m_Layers.end(), Overlay);
		if (IT != m_Layers.end())
		{
			Overlay->OnDetach();
			m_Layers.erase(IT);
		}
	}
}