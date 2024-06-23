#pragma once
#include <ntifs.h>

#include "../kutils.h"

#pragma region INTERNAL_CONSTANTS
#define FLAG_OK			"[+]"
#define FLAG_ERROR		"[-]"
#define FLAG_WARNING	"[!]"

#define LEVEL_OK		"[INFO]"
#define LEVEL_ERROR		"[ERROR]"
#define LEVEL_WARNING	"[WARNING]"
#pragma endregion

#define LOG(str) log_utils::Logger::info(str)
#define WARN(str) log_utils::Logger::warning(str)
#define ERR(str) log_utils::Logger::error(str)

namespace log_utils
{
	class Logger
	{
	private:
		static void message(PCSTR sFlag, PCSTR sLevel, PCSTR sMessage);
	public:
		static void info(PCSTR sMessage) { message(FLAG_OK, LEVEL_OK, sMessage); }
		static void warning(PCSTR sMessage) { message(FLAG_WARNING, LEVEL_WARNING, sMessage); }
		static void error(PCSTR sMessage) { message(FLAG_ERROR, LEVEL_ERROR, sMessage); }
	};
}