#pragma once

#include <memory>

#ifdef CD_PLATFORM_WINDOWS


#if CD_DYNAMIC_LINK
	#ifdef CD_BUILD_DLL
		#define CRYDUST_API __declspec(dllexport)
	#else
		#define CRYDUST_API __declspec(dllimport)
	#endif
#else
    #define CRYDUST_API
#endif

#else
	#error CryDust only support Window!

#endif // CD_PLATFORM_WINDOWS


#ifdef CD_ENABLE_ASSERTS
	#define DEBUG_ASSERT(x,...) {if(!(x)){ DEBUG_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
	#define CORE_DEBUG_ASSERT(x,...) {if(!(x)){ CORE_DEBUG_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}

#else
	#define DEBUG_ASSERT(x,...)
	#define CORE_DEBUG_ASSERT(x,...)
#endif

#ifdef CD_DEBUG
#define CD_ENABLE_ASSERTS
#endif


#define BIT(x)  (1 << x)

#define CD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)


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