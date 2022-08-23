#include "WginePCH.h"
#include "LayerStack.h"

Wgine::LayerStack::LayerStack()
{
	m_LayerInsert = m_Layers.begin();
}

Wgine::LayerStack::~LayerStack()
{
	for (auto layer : m_Layers)
		delete layer;
}

void Wgine::LayerStack::PushLayer(Layer *layer)
{
	m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
}

void Wgine::LayerStack::PushOverlay(Layer *overlay)
{
	m_Layers.emplace_back(overlay);
}

void Wgine::LayerStack::PopLayer(Layer *layer)
{
	auto i = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (i != m_Layers.end())
	{
		m_Layers.erase(i);
		m_LayerInsert--;
	}
}

void Wgine::LayerStack::PopOverlay(Layer *overlay)
{
	auto i = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (i != m_Layers.end())
		m_Layers.erase(i);
}
