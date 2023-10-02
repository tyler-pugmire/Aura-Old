#include "pch.h"
#include "Logger.h"
#include <filesystem>
#include <unordered_map>
//#include <string_view>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace
{
	std::shared_ptr<spdlog::logger> s_CoreLogger;
	std::shared_ptr<spdlog::logger> s_ClientLogger;
	std::shared_ptr<spdlog::logger> s_EditorConsoleLogger;
	std::unordered_map<std::string, Aura::Log::TagDetails> s_EnabledTags;
}

#define AURA_HAS_CONSOLE !AURA_DIST

namespace Aura::Log
{
	void Init()
	{
		// Create "logs" directory if doesn't exist
		std::string logsDirectory = "logs";
		if (!std::filesystem::exists(logsDirectory))
			std::filesystem::create_directories(logsDirectory);

		std::vector<spdlog::sink_ptr> hazelSinks =
		{
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/AURA.log", true),
#if AURA_HAS_CONSOLE
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
#endif
		};

		std::vector<spdlog::sink_ptr> appSinks =
		{
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/APP.log", true),
#if AURA_HAS_CONSOLE
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
#endif
		};

		std::vector<spdlog::sink_ptr> editorConsoleSinks =
		{
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/APP.log", true),
#if AURA_HAS_CONSOLE
			//std::make_shared<EditorConsoleSink>(1)
#endif
		};

		hazelSinks[0]->set_pattern("[%T] [%l] %n: %v");
		appSinks[0]->set_pattern("[%T] [%l] %n: %v");

#if AURA_HAS_CONSOLE
		hazelSinks[1]->set_pattern("%^[%T] %n: %v%$");
		appSinks[1]->set_pattern("%^[%T] %n: %v%$");
		for (auto sink : editorConsoleSinks)
			sink->set_pattern("%^%v%$");
#endif

		s_CoreLogger = std::make_shared<spdlog::logger>("AURA", hazelSinks.begin(), hazelSinks.end());
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", appSinks.begin(), appSinks.end());
		s_ClientLogger->set_level(spdlog::level::trace);

		s_EditorConsoleLogger = std::make_shared<spdlog::logger>("Console", editorConsoleSinks.begin(), editorConsoleSinks.end());
		s_EditorConsoleLogger->set_level(spdlog::level::trace);
	}

	void Shutdown()
	{
		s_EditorConsoleLogger.reset();
		s_ClientLogger.reset();
		s_CoreLogger.reset();
		spdlog::drop_all();
	}

	const char* LevelToString(Level level)
	{
		switch (level)
		{
		case Level::Trace: return "Trace";
		case Level::Info:  return "Info";
		case Level::Warn:  return "Warn";
		case Level::Error: return "Error";
		case Level::Fatal: return "Fatal";
		}
		return "";
	}
	Level LevelFromString(std::string_view string)
	{
		if (string == "Trace") return Level::Trace;
		if (string == "Info")  return Level::Info;
		if (string == "Warn")  return Level::Warn;
		if (string == "Error") return Level::Error;
		if (string == "Fatal") return Level::Fatal;

		return Level::Trace;
	}

	std::shared_ptr<spdlog::logger>& GetCoreLogger() 
	{ 
		return s_CoreLogger; 
	}

	std::shared_ptr<spdlog::logger>& GetClientLogger() 
	{ 
		return s_ClientLogger; 
	}

	std::shared_ptr<spdlog::logger>& GetEditorConsoleLogger() 
	{ 
		return s_EditorConsoleLogger; 
	}

	bool HasTag(const std::string& tag) 
	{ 
		return s_EnabledTags.find(tag) != s_EnabledTags.end(); 
	}

	std::unordered_map<std::string, TagDetails>& EnabledTags() 
	{ 
		return s_EnabledTags; 
	}
}