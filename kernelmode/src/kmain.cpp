#define KERNELMODE

#include <ntifs.h>

#include "kutils.h"
#include "classes/Logger.h"

extern "C"
{
	NTKERNELAPI NTSTATUS IoCreateDriver(PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitializationFunction);

	NTKERNELAPI NTSTATUS MmCopyVirtualMemory(
		PEPROCESS SourceProcess, PVOID SourceAddress,
		PEPROCESS TargetProcess, PVOID TargetAddress,
		SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize
	);
}

namespace driver
{
	NTSTATUS create(PDEVICE_OBJECT pDevice, PIRP pIrp)
	{
		UNREFERENCED_PARAMETER(pDevice);

		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return pIrp->IoStatus.Status;
	}

	NTSTATUS close(PDEVICE_OBJECT pDevice, PIRP pIrp)
	{
		return create(pDevice, pIrp);
	}

	NTSTATUS deviceControl(PDEVICE_OBJECT pDevice, PIRP pIrp)
	{
		UNREFERENCED_PARAMETER(pDevice);

		LOG("Device control call");

		NTSTATUS status = STATUS_UNSUCCESSFUL;
		PIO_STACK_LOCATION pStackLocation = IoGetCurrentIrpStackLocation(pIrp);
		PUSERMODE_REQUEST pRequest = reinterpret_cast<PUSERMODE_REQUEST>(pIrp->AssociatedIrp.SystemBuffer);
		
		if (!pStackLocation && !pRequest)
		{
			IoCompleteRequest(pIrp, IO_NO_INCREMENT);

			return status;
		}

		static PEPROCESS pTargetProc = nullptr;

		ASSERT(pStackLocation);
		const ULONG uControlCode = pStackLocation->Parameters.DeviceIoControl.IoControlCode;

		switch (uControlCode)
		{
		case codes::attach:
			status = PsLookupProcessByProcessId(pRequest->hPID, &pTargetProc);
			break;
		case codes::read:
			if (pTargetProc)
			{
				status = MmCopyVirtualMemory(
					pTargetProc, pRequest->pTarget, PsGetCurrentProcess(), pRequest->pBuffer, pRequest->uSize, KernelMode, &pRequest->uReturnSize
				);
			}
			break;
		case codes::write:
						if (pTargetProc)
			{
				status = MmCopyVirtualMemory(
					PsGetCurrentProcess(), pRequest->pBuffer, pTargetProc, pRequest->pTarget, pRequest->uSize, KernelMode, &pRequest->uReturnSize
				);
			}
			break;

		default:
			break;
		}

		pIrp->IoStatus.Status = status;
		pIrp->IoStatus.Information = sizeof(pRequest);

		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return pIrp->IoStatus.Status;
	}
}

NTSTATUS DriverMain(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING sRegistryPath)
{
	UNREFERENCED_PARAMETER(sRegistryPath);

	log_utils::Logger();

	UNICODE_STRING sDeviceName = {};
	RtlInitUnicodeString(&sDeviceName, L"\\Device\\KM_KBRDCS2RWAPP");

	PDEVICE_OBJECT pDeviceObject = nullptr;

	NTSTATUS status = IoCreateDevice(
		pDriverObject, 0, &sDeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject
	);

	if (status != STATUS_SUCCESS)
	{
		ERR("Device creation failure");
		
		return status;
	}

	LOG("Device created");

	UNICODE_STRING sSymbolicLink = {};
	RtlInitUnicodeString(&sSymbolicLink, L"\\DosDevices\\KM_KBRDCS2RWAPP");

	status = IoCreateSymbolicLink(&sSymbolicLink, &sDeviceName);

	if (status != STATUS_SUCCESS)
	{
		ERR("Symbolic link creation failure");

		return status;
	}

	LOG("Symbolic link established");

	SetFlag(pDeviceObject->Flags, DO_BUFFERED_IO);

	pDriverObject->MajorFunction[IRP_MJ_CREATE]			= driver::create;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE]			= driver::close;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = driver::deviceControl;

	ClearFlag(pDeviceObject->Flags, DO_DEVICE_INITIALIZING);

	return status;
}

NTSTATUS DriverEntry() 
{
	WARN("DriverEntry call");
	WARN("Assuming mapping process initiated");

	// Initialization
	UNICODE_STRING sDriverName = {};
	RtlInitUnicodeString(&sDriverName, L"\\Driver\\KM_KBRDCS2RWAPP");

	LOG("Driver mapping process completed");

	return IoCreateDriver(&sDriverName, &DriverMain);
}