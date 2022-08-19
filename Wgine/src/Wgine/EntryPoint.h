#pragma once

#ifdef WGINE_PLATFORM_WINDOWS

extern Wgine::Application* Wgine::CreateApplication();

int main(int argc, char** argv)
{
	Wgine::Log::Init();
	int i = 5;
	WGINE_TRACE("variable value x={0}", i);
	WGINE_INFO("whatever");

	auto app = Wgine::CreateApplication();
	app->Run();
	delete app;
}

#else
	#error Unsupported platform!
#endif
