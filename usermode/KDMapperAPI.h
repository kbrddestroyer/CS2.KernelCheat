#pragma once
#include "src/core/constants.h"

#include <string>
#include <filesystem>
#include <Windows.h>

#ifdef USE_NATIVE_KDMAPPER
#include <kdmapper.hpp>
#include <service.hpp>
#endif

class KDMapperAPI
{
private:
	std::wstring wPath = L"";
	HANDLE iqvw64e_device_handle = nullptr;
public:
	KDMapperAPI() {}

	bool callbackExample(ULONG64* param1, ULONG64* param2, ULONG64 allocationPtr, ULONG64 allocationSize, ULONG64 mdlptr) {
		UNREFERENCED_PARAMETER(param1);
		UNREFERENCED_PARAMETER(param2);
		UNREFERENCED_PARAMETER(allocationPtr);
		UNREFERENCED_PARAMETER(allocationSize);
		UNREFERENCED_PARAMETER(mdlptr);
		/*
		This callback occurs before call driver entry and
		can be usefull to pass more customized params in
		the last step of the mapping procedure since you
		know now the mapping address and other things
		*/
		return true;
	}

	bool load()
	{
#ifdef USE_NATIVE_KDMAPPER
		const std::wstring driver_path = std::filesystem::current_path().native() + L"\\kernelmode.sys";

		if (!std::filesystem::exists(driver_path)) {
			MessageBoxA(NULL, "Nonexistent driver", "Error", NULL);
			return false;
		}

		iqvw64e_device_handle = intel_driver::Load();

		if (iqvw64e_device_handle == INVALID_HANDLE_VALUE)
		{
			MessageBoxA(NULL, "Invalid handle", "Error", NULL);
			return false;
		}
		std::vector<uint8_t> raw_image = { 0 };
		if (!utils::ReadFileToMemory(driver_path, &raw_image)) {
			MessageBoxA(NULL, "Image reading failed", "Error", NULL);
			intel_driver::Unload(iqvw64e_device_handle);
			return false;
		}

		kdmapper::AllocationMode mode = kdmapper::AllocationMode::AllocatePool;

		NTSTATUS exitCode = 0;
		if (!kdmapper::MapDriver(iqvw64e_device_handle, raw_image.data(), 0, 0, free, true, mode, false)) {
			MessageBoxA(NULL, "Driver mapping error", "Error", NULL);
			intel_driver::Unload(iqvw64e_device_handle);
			return false;
		}

		if (!intel_driver::Unload(iqvw64e_device_handle)) {
			MessageBoxA(NULL, "Failed to unload vulnerable driver", "Warning", NULL);
		}
		MessageBoxA(NULL, "OK!", "Success", NULL);
		return true;
#else
		return false;
#endif
	}

	bool unload()
	{
#ifdef USE_NATIVE_KDMAPPER
		return service::StopAndRemove(wPath);
#endif
	}
};

