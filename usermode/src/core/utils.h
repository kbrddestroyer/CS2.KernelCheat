#pragma once
#include <Windows.h>
#include <TlHelp32.h>

#include "../../../kernelmode/src/kutils.h"

namespace driver
{
	inline bool attach(HANDLE hDriverHandle, const DWORD dPid)
	{
		USERMODE_REQUEST request;

		request.hPID = reinterpret_cast<HANDLE>(dPid);

		return DeviceIoControl(hDriverHandle, codes::attach, &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
	}

	template<class T>
	inline T read(HANDLE hDriverHandle, const uintptr_t uAddress)
	{
		T temp = {};
		USERMODE_REQUEST request;
		request.pTarget = reinterpret_cast<PVOID>(uAddress);
		request.pBuffer = &temp;
		request.uSize = sizeof(temp);

		DeviceIoControl(hDriverHandle, codes::read, &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
		if (!temp)
			DWORD err = GetLastError();

		return temp;
	}

	template<class T>
	inline void write(HANDLE hDriverHandle, const uintptr_t uAddress, const T& value)
	{
		USERMODE_REQUEST request;
		request.pTarget = reinterpret_cast<PVOID> (uAddress);
		request.pBuffer = (PVOID) & value;
		
		request.uSize = sizeof(T);

		DeviceIoControl(hDriverHandle, codes::write, &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
	}
}

namespace uapp
{
	static inline DWORD getPID(const wchar_t* pName)
	{
		DWORD dPid = 0;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (hSnapshot == INVALID_HANDLE_VALUE)
			return dPid;

		PROCESSENTRY32W entry = {};

		entry.dwSize = sizeof(decltype(entry));

		if (Process32FirstW(hSnapshot, &entry) == TRUE)
		{
			do
			{
				if (_wcsicmp(pName, entry.szExeFile) == 0)
				{
					dPid = entry.th32ProcessID;
					break;
				}
			} while (Process32NextW(hSnapshot, &entry) == TRUE);
		}

		CloseHandle(hSnapshot);
		return dPid;
	}

	static inline uintptr_t getModuleBase(const DWORD dPid, const wchar_t* pModuleName)
	{
		uintptr_t uModuleBase = 0;

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dPid);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return uModuleBase;

		MODULEENTRY32 entry;
		entry.dwSize = sizeof(decltype(entry));

		if (Module32First(hSnapshot, &entry) == TRUE)
		{
			do
			{
				if (wcsstr(pModuleName, entry.szModule) != nullptr)
				{
					uModuleBase = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
					break;
				}
			} while (Module32Next(hSnapshot, &entry));
		}

		CloseHandle(hSnapshot);
		return uModuleBase;
	}
}