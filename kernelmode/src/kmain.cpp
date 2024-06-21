#include <ntifs.h>

void debug_print(PCSTR str)
{
#ifndef DEBUG
	UNREFERENCED_PARAMETER(str);
#endif
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, str));
}

NTSTATUS DriverEntry() {
	debug_print("Hello from kernel mode");

	return STATUS_SUCCESS;
}