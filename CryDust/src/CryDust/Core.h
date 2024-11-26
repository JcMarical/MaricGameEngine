#pragma once



#ifdef CD_PLATFORM_WINDOWS
	#ifdef CD_BUILD_DLL
		#define CRYDUST_API __declspec(dllexport)
	#else
		#define CRYDUST_API __declspec(dllimport)
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



#define BIT(x)  (1 << x)

#define CD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)