#include <ntifs.h>
extern "C"
{
	DRIVER_INITIALIZE DriverEntry;
}

NTSTATUS DriverEntry() {

	return STATUS_SUCCESS;
}