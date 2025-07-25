#pragma once

#define YAML_CPP_STATIC_DEFINE
#include "CryDust/Core/PlatformDetection.h"
#ifdef CD_PLATFORM_WINDOWS
	#ifndef NOMINMAX
	// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif



#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>



#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "CryDust/Core/Base.h"

#include "CryDust/Core/Log.h"

#include "CryDust/Debug/Instrumentor.h"


#ifdef CD_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
