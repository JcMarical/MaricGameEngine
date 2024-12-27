#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "CryDust/Core/Base.h"
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


namespace CryDust {

	class  Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}



template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}
template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}
template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternio)
{
	return os << glm::to_string(quaternio);
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
