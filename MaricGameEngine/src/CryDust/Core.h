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


#define BIT(x) (1 << x)