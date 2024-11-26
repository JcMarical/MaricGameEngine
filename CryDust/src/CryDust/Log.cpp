
#include "cdpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
namespace CryDust
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	//logger基础设置，初始化调用
	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("CryDust");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}



}