#include "Logger.h"

log_utils::Logger::Logger()
{
	if (pInstance)
		pInstance->error("Logger instance already exists!");
	else
		pInstance = this;
}

void log_utils::Logger::message(PCSTR sFlag, PCSTR sLevel, PCSTR sMessage)
{
#ifndef DEBUG
	UNREFERENCED_PARAMETER(sFlag);
	UNREFERENCED_PARAMETER(sLevel);
	UNREFERENCED_PARAMETER(sMessage);
#endif // !DEBUG

	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, sFlag));
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, " "));
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, sLevel));
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, " "));
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, sMessage));
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "\n"));
}