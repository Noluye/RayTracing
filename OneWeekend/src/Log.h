#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace ow
{
	//enum class LogLevel { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<class spdlog::logger>& GetCoreLogger();
	private:
		static std::shared_ptr<class spdlog::logger> s_CoreLogger;
	};
}

// Core log macros
#define OW_CORE_TRACE(...)     ::ow::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OW_CORE_DEBUG(...)     ::ow::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define OW_CORE_INFO(...)      ::ow::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OW_CORE_WARN(...)      ::ow::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OW_CORE_ERROR(...)     ::ow::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OW_CORE_FATAL(...)     ::ow::Log::GetCoreLogger()->critical(__VA_ARGS__)

// TODO
//#define PP_Log(...)            ::dl::Log::Logging(__VA_ARGS__)