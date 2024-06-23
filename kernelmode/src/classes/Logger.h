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

#define LOG(str) log_utils::Logger::Instance()->info(str);
#define WARN(str) log_utils::Logger::Instance()->warning(str);
#define ERR(str) log_utils::Logger::Instance()->error(str);

namespace log_utils
{
	class Logger
	{
	private:
		static inline Logger* pInstance;
		void message(PCSTR sFlag, PCSTR sLevel, PCSTR sMessage);
	public:
		Logger();
		~Logger()
		{
			pInstance = nullptr;
		}

		static Logger* Instance() { return pInstance; }

		void info(PCSTR sMessage) { message(FLAG_OK, LEVEL_OK, sMessage); }
		void warning(PCSTR sMessage) { message(FLAG_WARNING, LEVEL_WARNING, sMessage); }
		void error(PCSTR sMessage) { message(FLAG_ERROR, LEVEL_ERROR, sMessage); }
	};
}