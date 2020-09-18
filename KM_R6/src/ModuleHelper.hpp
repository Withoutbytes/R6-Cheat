#pragma once
#include <string.h>
#include "Undocumented.hpp"


namespace ModuleHelper {

	PVOID GetModuleBaseByName(LPSTR moduleName) {
		ULONG len;
		NTSTATUS status;
		status = ZwQuerySystemInformation(SystemModuleInformation, nullptr, NULL, &len);

		if (!len) return nullptr;

		PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)MmAllocateNonCachedMemory(len);

		if (!modules) return nullptr;

		status = ZwQuerySystemInformation(SystemModuleInformation, modules, len, &len);

		if (!NT_SUCCESS(status)) return nullptr;
		
		for (size_t i = 0; i < modules->NumberOfModules; i++)
		{
			auto module = modules->Modules[i];
			if (!module.FullPathName)
				continue;

			if (strcmp((char*)module.FullPathName + module.OffsetToFileName, moduleName) == 0)
				return module.ImageBase;
		}

		return nullptr;
	}

	PVOID GetExportedRoutineByModuleAndName(LPSTR moduleName, LPSTR funcName) {
		auto baseAddress = GetModuleBaseByName(moduleName);
		if (!baseAddress) return nullptr;

		return RtlFindExportedRoutineByName(baseAddress, funcName);
	}
}
