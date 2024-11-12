#pragma once


#ifdef CD_PLATFORM_WINDOWS

extern CryDust::Application* CryDust::CreateApplication();

int main(int argc, char** argv)
{

	//logger日志系统
	CryDust::Log::Init();
	CORE_DEBUG_WARN("Initialized DebugLogger!!!");
	int a = 5;
	DEBUG_INFO("Hello! Var = {0}", a);

	//
	auto app = CryDust::CreateApplication();
	app->Run();
	delete app;
}

#endif
