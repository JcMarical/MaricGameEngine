#pragma once
#include "CryDust/Core/Base.h"
#include "CryDust/Core/Application.h"
#ifdef CD_PLATFORM_WINDOWS

extern CryDust::Application* CryDust::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{

	//logger日志系统
	CryDust::Log::Init();

	//
	CD_PROFILE_BEGIN_SESSION("Startup", "CryDustProfile - Startup.json");
	auto app = CryDust::CreateApplication({argc,argv});
	CD_PROFILE_END_SESSION();

	CD_PROFILE_BEGIN_SESSION("Runtime", "CryDustProfile-Runtime.json");
	app->Run();
	CD_PROFILE_END_SESSION();

	CD_PROFILE_BEGIN_SESSION("Startup", "CryDustProfile-Shutdown.json");
	delete app;
	CD_PROFILE_END_SESSION();
}

#endif
