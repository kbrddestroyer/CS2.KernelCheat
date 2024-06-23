#pragma once

#ifdef DEBUG
#include <iostream>
#endif
#include <fstream>
#include <string>

#define LOG_FILE			"usermode.log"

#pragma region INTERNAL_CONSTANTS
#define LOG_INFO_SCOPE		" [INFO] "
#define LOG_WARN_SCOPE		" [WARN] "
#define LOG_ERROR_SCOPE 	" [ERROR]"

#define LOG_INFO_LABEL		'+'
#define LOG_WARN_LABEL		'!'
#define LOG_ERROR_LABEL		'-'
#pragma endregion

#define INFO(s) log_utils::UMLogger::info(s)
#define WARN(s) log_utils::UMLogger::warn(s)
#define ERR(s)	log_utils::UMLogger::error(s)

namespace log_utils {
	class UMLogger
	{
	private:
		static void message(char cLablel, std::string sScope, std::string sMessage);
	public:
		static void info(std::string sMessage) { message(LOG_INFO_LABEL, LOG_INFO_SCOPE, sMessage); }
		static void warn(std::string sMessage) { message(LOG_WARN_LABEL, LOG_WARN_SCOPE, sMessage); }
		static void error(std::string sMessage) { message(LOG_ERROR_LABEL, LOG_ERROR_SCOPE, sMessage); }
	};
}