#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <string_view>

namespace Aura
{
	namespace Log
	{
		enum class Type : uint8_t
		{
			Core = 0, Client = 1
		};
		enum class Level : uint8_t
		{
			Trace = 0, Info, Warn, Error, Fatal
		};
		struct TagDetails
		{
			bool Enabled = true;
			Level LevelFilter = Level::Trace;
		};


		void Init();
		void Shutdown();

		const char* LevelToString(Level level);
		Level LevelFromString(std::string_view string);

		std::shared_ptr<spdlog::logger>& GetCoreLogger();
		std::shared_ptr<spdlog::logger>& GetClientLogger();
		std::shared_ptr<spdlog::logger>& GetEditorConsoleLogger();

		bool HasTag(const std::string& tag);
		std::unordered_map<std::string, TagDetails>& EnabledTags();

		template<typename... Args>
		static void PrintMessage(Log::Type type, Log::Level level, std::string_view tag, Args&&... args);

		template<typename... Args>
		static void PrintAssertMessage(Log::Type type, std::string_view prefix, Args&&... args);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tagged logs (prefer these!)                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Core logging
#define AURA_CORE_TRACE_TAG(tag, ...) ::Aura::Log::PrintMessage(::Aura::Log::Type::Core, ::Aura::Log::Level::Trace, tag, __VA_ARGS__)
#define AURA_CORE_INFO_TAG(tag, ...)  ::Aura::Log::PrintMessage(::Aura::Log::Type::Core, ::Aura::Log::Level::Info, tag, __VA_ARGS__)
#define AURA_CORE_WARN_TAG(tag, ...)  ::Aura::Log::PrintMessage(::Aura::Log::Type::Core, ::Aura::Log::Level::Warn, tag, __VA_ARGS__)
#define AURA_CORE_ERROR_TAG(tag, ...) ::Aura::Log::PrintMessage(::Aura::Log::Type::Core, ::Aura::Log::Level::Error, tag, __VA_ARGS__)
#define AURA_CORE_FATAL_TAG(tag, ...) ::Aura::Log::PrintMessage(::Aura::Log::Type::Core, ::Aura::Log::Level::Fatal, tag, __VA_ARGS__)

// Client logging
#define AURA_TRACE_TAG(tag, ...) ::Aura::Log::PrintMessage(::Aura::Log::Type::Client, ::Aura::Log::Level::Trace, tag, __VA_ARGS__)
#define AURA_INFO_TAG(tag, ...)  ::Aura::Log::PrintMessage(::Aura::Log::Type::Client, ::Aura::Log::Level::Info, tag, __VA_ARGS__)
#define AURA_WARN_TAG(tag, ...)  ::Aura::Log::PrintMessage(::Aura::Log::Type::Client, ::Aura::Log::Level::Warn, tag, __VA_ARGS__)
#define AURA_ERROR_TAG(tag, ...) ::Aura::Log::PrintMessage(::Aura::Log::Type::Client, ::Aura::Log::Level::Error, tag, __VA_ARGS__)
#define AURA_FATAL_TAG(tag, ...) ::Aura::Log::PrintMessage(::Aura::Log::Type::Client, ::Aura::Log::Level::Fatal, tag, __VA_ARGS__)

// Core Logging
#define AURA_CORE_TRACE(...)  ::Aura::Log::PrintMessage(::Aura::Log::Type::Core, ::Aura::Log::Level::Trace, "", __VA_ARGS__)
#define AURA_CORE_INFO(...)   ::Aura::Log::PrintMessage(::Aura::Log::Type::Core, ::Aura::Log::Level::Info, "", __VA_ARGS__)
#define AURA_CORE_WARN(...)   ::Aura::Log::PrintMessage(::Aura::Log::Type::Core, ::Aura::Log::Level::Warn, "", __VA_ARGS__)
#define AURA_CORE_ERROR(...)  ::Aura::Log::PrintMessage(::Aura::Log::Type::Core, ::Aura::Log::Level::Error, "", __VA_ARGS__)
#define AURA_CORE_FATAL(...)  ::Aura::Log::PrintMessage(::Aura::Log::Type::Core, ::Aura::Log::Level::Fatal, "", __VA_ARGS__)

// Client Logging
#define AURA_TRACE(...)   ::Aura::Log::PrintMessage(::Aura::Log::Type::Client, ::Aura::Log::Level::Trace, "", __VA_ARGS__)
#define AURA_INFO(...)    ::Aura::Log::PrintMessage(::Aura::Log::Type::Client, ::Aura::Log::Level::Info, "", __VA_ARGS__)
#define AURA_WARN(...)    ::Aura::Log::PrintMessage(::Aura::Log::Type::Client, ::Aura::Log::Level::Warn, "", __VA_ARGS__)
#define AURA_ERROR(...)   ::Aura::Log::PrintMessage(::Aura::Log::Type::Client, ::Aura::Log::Level::Error, "", __VA_ARGS__)
#define AURA_FATAL(...)   ::Aura::Log::PrintMessage(::Aura::Log::Type::Client, ::Aura::Log::Level::Fatal, "", __VA_ARGS__)

// Editor Console Logging Macros
#define AURA_CONSOLE_LOG_TRACE(...)   Aura::Log::GetEditorConsoleLogger()->trace(__VA_ARGS__)
#define AURA_CONSOLE_LOG_INFO(...)    Aura::Log::GetEditorConsoleLogger()->info(__VA_ARGS__)
#define AURA_CONSOLE_LOG_WARN(...)    Aura::Log::GetEditorConsoleLogger()->warn(__VA_ARGS__)
#define AURA_CONSOLE_LOG_ERROR(...)   Aura::Log::GetEditorConsoleLogger()->error(__VA_ARGS__)
#define AURA_CONSOLE_LOG_FATAL(...)   Aura::Log::GetEditorConsoleLogger()->critical(__VA_ARGS__)

namespace Aura::Log
{
	template<typename... Args>
	void PrintMessage(Log::Type type, Log::Level level, std::string_view tag, Args&&... args)
	{
		auto detail = EnabledTags()[std::string(tag)];
		if (detail.Enabled && detail.LevelFilter <= level)
		{
			auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
			std::string logString = tag.empty() ? "{0}{1}" : "[{0}] {1}";
			switch (level)
			{
			case Level::Trace:
				logger->trace(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Info:
				logger->info(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Warn:
				logger->warn(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Error:
				logger->error(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Fatal:
				logger->critical(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			}
		}
	}

	template<typename... Args>
	void PrintAssertMessage(Log::Type type, std::string_view prefix, Args&&... args)
	{
		auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}: {1}", prefix, fmt::format(std::forward<Args>(args)...));
	}

	template<>
	inline void PrintAssertMessage(Log::Type type, std::string_view prefix)
	{
		auto logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}", prefix);
	}
}