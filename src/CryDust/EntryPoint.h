#pragma once


#ifdef CD_PLATFORM_WINDOWS

extern CryDust::Application* CryDust::CreateApplication();

int main(int argc, char** argv)
{
	auto app = CryDust::CreateApplication();
	app->Run();
	delete app;
}

#endif
