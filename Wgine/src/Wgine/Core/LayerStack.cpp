#include "WginePCH.h"
#include "LayerStack.h"

Wgine::LayerStack::LayerStack()
{
	
}

Wgine::LayerStack::~LayerStack()
{
	for (auto layer : m_Layers)
		delete layer;
}

void Wgine::LayerStack::PushLayer(Layer *layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex++, layer);
	layer->OnAttach();
}

void Wgine::LayerStack::PushOverlay(Layer *overlay)
{
	m_Layers.emplace_back(overlay);
	overlay->OnAttach();
}

void Wgine::LayerStack::PopLayer(Layer *layer)
{
	auto i = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (i != m_Layers.end())
	{
		m_Layers.erase(i);
		m_LayerInsertIndex--;
		layer->OnDetach();
	}
}

void Wgine::LayerStack::PopOverlay(Layer *overlay)
{
	auto i = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (i != m_Layers.end())
	{
		m_Layers.erase(i);
		overlay->OnDetach();
	}
}
