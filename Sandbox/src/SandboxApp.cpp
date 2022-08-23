#include "Wgine.h"

class ExampleLayer : public Wgine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		WGINE_INFO("ExampleLayer::Update");
	}

	void OnEvent(Wgine::Event &event) override
	{
		WGINE_TRACE("Example layer update: {0}", event);
	}
};

class Sandbox : public Wgine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Wgine::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Wgine::Application* Wgine::CreateApplication()
{
	return new Sandbox();
}
