#pragma once

#include <memory>



#include "CryDust/Core/PlatformDetection.h"


#ifdef CD_DEBUG
	#if defined(CD_PLATFORM_WINDOWS)
		#define CD_DEBUGBREAK() __debugbreak()
		#elif defined(CD_PLATFORM_LINUX)
			#include <signal.h>
			#define CD_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
		#define CD_ENABLE_ASSERTS
#else
	#define CD_DEBUGBREAK()
#endif


#define CD_EXPAND_MACRO(x) x
#define CD_STRINGIFY_MACRO(x) #x






#ifdef CD_DEBUG
#define CD_ENABLE_ASSERTS
#endif


#define BIT(x)  (1 << x)

//#define CD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define CD_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace CryDust
{

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "CryDust/Core/Log.h"
#include "CryDust/Core/Assert.h"
