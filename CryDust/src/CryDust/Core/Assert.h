#pragma once
#include "CryDust/Core/Base.h"
#include "CryDust/Core/Log.h"
#include <filesystem>
#ifdef CD_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define CD_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { type##DEBUG_ERROR(msg, __VA_ARGS__); CD_DEBUGBREAK(); } }
#define CD_INTERNAL_ASSERT_WITH_MSG(type, check, ...) CD_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define CD_INTERNAL_ASSERT_NO_MSG(type, check) CD_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", CD_STRINGIFY_MACRO(check),std::filesystem::path(__FILE__).filename().string(), __LINE__)
#define CD_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define CD_INTERNAL_ASSERT_GET_MACRO(...) CD_EXPAND_MACRO( CD_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, CD_INTERNAL_ASSERT_WITH_MSG, CD_INTERNAL_ASSERT_NO_MSG) )
// Currently accepts at least the condition and one additional parameter (the message) being optional
#define DEBUG_ASSERT(...) CD_EXPAND_MACRO( CD_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define CORE_DEBUG_ASSERT(...) CD_EXPAND_MACRO( CD_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(CORE_, __VA_ARGS__) )
#else
#define DEBUG_ASSERT(...)
#define CORE_DEBUG_ASSERT(...)
#endif
