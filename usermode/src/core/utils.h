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

	inline void* readUnsafe(HANDLE hDriverHandle, const uintptr_t uAddress, const size_t size)
	{
#ifndef GUI_DEBUG_MODE
		void* temp = malloc(size);
		if (!temp)
			return nullptr;

		USERMODE_REQUEST request;
		request.pTarget = reinterpret_cast<PVOID>(uAddress);
		request.pBuffer = temp;
		request.uSize = size;

		DeviceIoControl(hDriverHandle, codes::read, &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
		return temp;
#elif
		return nullptr;
#endif
	}

	template<class T>
	inline T read(HANDLE hDriverHandle, const uintptr_t uAddress)
	{
#ifndef GUI_DEBUG_MODE
		T temp = {};
		USERMODE_REQUEST request;
		request.pTarget = reinterpret_cast<PVOID>(uAddress);
		request.pBuffer = &temp;
		request.uSize = sizeof(temp);

		DeviceIoControl(hDriverHandle, codes::read, &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
		return temp;
#else
		return T();
#endif
	}

	inline void writeUnsafe(HANDLE hDriverHandle, const uintptr_t uAddress, void* value, const size_t uSize)
	{
#ifndef GUI_DEBUG_MODE
		USERMODE_REQUEST request;
		request.pTarget = reinterpret_cast<PVOID>(uAddress);
		request.pBuffer = value;
		request.uSize = uSize;

		DeviceIoControl(hDriverHandle, codes::write, &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
#endif
	}

	template<class T>
	inline void write(HANDLE hDriverHandle, const uintptr_t uAddress, const T& value)
	{
#ifndef GUI_DEBUG_MODE
		USERMODE_REQUEST request;
		request.pTarget = reinterpret_cast<PVOID> (uAddress);
		request.pBuffer = (PVOID) & value;
		
		request.uSize = sizeof(T);

		DeviceIoControl(hDriverHandle, codes::write, &request, sizeof(request), &request, sizeof(request), nullptr, nullptr);
#endif
	}
}

namespace uapp
{
	static inline DWORD getPID(const wchar_t* pName)
	{
#ifndef GUI_DEBUG_MODE
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
#else
		return 0;
#endif
	}

	static inline uintptr_t getModuleBase(const DWORD dPid, const wchar_t* pModuleName)
	{
#ifndef GUI_DEBUG_MODE
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
#else
		return 0;
#endif
	}
}