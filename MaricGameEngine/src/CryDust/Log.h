#pragma once



#include "Core.h"
#include <spdlog/spdlog.h>



namespace CryDust {

	class CRYDUST_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core Logger define
#define CORE_DEBUG_TRACE(...)	::CryDust::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define CORE_DEBUG_INFO(...)	::CryDust::Log::GetCoreLogger()->info(__VA_ARGS__);
#define CORE_DEBUG_WARN(...)	::CryDust::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define CORE_DEBUG_ERROR(...)	::CryDust::Log::GetCoreLogger()->error(__VA_ARGS__);
#define CORE_DEBUG_FATAL(...)	::CryDust::Log::GetCoreLogger()->fatal(__VA_ARGS__);


//Client Logger define
#define DEBUG_TRACE(...)	::CryDust::Log::GetClientLogger()->trace(__VA_ARGS__);
#define DEBUG_INFO(...)	::CryDust::Log::GetClientLogger()->info(__VA_ARGS__);
#define DEBUG_WARN(...)	::CryDust::Log::GetClientLogger()->warn(__VA_ARGS__);
#define DEBUG_ERROR(...)	::CryDust::Log::GetClientLogger()->error(__VA_ARGS__);
#define DEBUG_FATAL(...)	::CryDust::Log::GetClientLogger()->fatal(__VA_ARGS__);