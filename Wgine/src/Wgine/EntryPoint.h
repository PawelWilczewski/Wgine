#pragma once

#ifdef WGINE_PLATFORM_WINDOWS

extern Wgine::Application* Wgine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Wgine::CreateApplication();
	app->Run();
	delete app;
}

#else
	#error Unsupported platform!
#endif
