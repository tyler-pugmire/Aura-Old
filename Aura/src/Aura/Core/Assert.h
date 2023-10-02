#pragma once

#include "Logger.h"

#ifdef AURA_PLATFORM_WINDOWS
	#define AURA_DEBUG_BREAK __debugbreak()
#else
	#define AURA_DEBUG_BREAK
#endif

#ifdef AURA_DEBUG
	#define AURA_ENABLE_ASSERTS
#endif

#define AURA_ENABLE_VERIFY

#ifdef AURA_ENABLE_ASSERTS
	#define AURA_CORE_ASSERT_MESSAGE_INTERNAL(...)  ::Aura::Log::PrintAssertMessage(::Aura::Log::Type::Core, "Assertion Failed", __VA_ARGS__)
	#define AURA_ASSERT_MESSAGE_INTERNAL(...)  ::Aura::Log::PrintAssertMessage(::Aura::Log::Type::Client, "Assertion Failed", __VA_ARGS__)

	#define AURA_CORE_ASSERT(condition, ...) { if(!(condition)) { AURA_CORE_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); AURA_DEBUG_BREAK; } }
	#define AURA_ASSERT(condition, ...) { if(!(condition)) { AURA_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); AURA_DEBUG_BREAK; } }
#else
	#define AURA_CORE_ASSERT(condition, ...)
	#define AURA_ASSERT(condition, ...)
#endif

#ifdef AURA_ENABLE_VERIFY
	#define AURA_CORE_VERIFY_MESSAGE_INTERNAL(...)  ::Aura::Log::PrintAssertMessage(::Aura::Log::Type::Core, "Verify Failed", __VA_ARGS__)
	#define AURA_VERIFY_MESSAGE_INTERNAL(...)  ::Aura::Log::PrintAssertMessage(::Aura::Log::Type::Client, "Verify Failed", __VA_ARGS__)

	#define AURA_CORE_VERIFY(condition, ...) { if(!(condition)) { AURA_CORE_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); AURA_DEBUG_BREAK; } }
	#define AURA_VERIFY(condition, ...) { if(!(condition)) { AURA_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); AURA_DEBUG_BREAK; } }
#else
	#define AURA_CORE_VERIFY(condition, ...)
	#define AURA_VERIFY(condition, ...)
#endif
