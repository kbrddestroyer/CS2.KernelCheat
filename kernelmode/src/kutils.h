#pragma once
#include "constants.h"

namespace driver
{
	namespace codes
	{
		constexpr ULONG attach	= CTL_CODE(FILE_DEVICE_UNKNOWN, ATTACH, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		constexpr ULONG read	= CTL_CODE(FILE_DEVICE_UNKNOWN, READ, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
		constexpr ULONG write	= CTL_CODE(FILE_DEVICE_UNKNOWN, WRITE, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
	}

	struct UMRequest
	{
		HANDLE	hPID;
		PVOID	pTarget;
		PVOID	pBuffer;

		SIZE_T	uSize;
		SIZE_T	uReturnSize;
	};

	typedef UMRequest USERMODE_REQUEST;
	typedef UMRequest* PUSERMODE_REQUEST;
}