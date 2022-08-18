#include "Wgine.h"

class Sandbox : public Wgine::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Wgine::Application* Wgine::CreateApplication()
{
	return new Sandbox();
}
