#pragma once
#include "Windefs.h"
#include <ntifs.h>


namespace Helper {

	auto CopyVirtualMemory(ULONGLONG sourcePid, uintptr_t sourceAddress, ULONGLONG destinyPid, uintptr_t destinyAddress, SIZE_T size) {
		PEPROCESS sourcePeProcess;
		PEPROCESS destinyPeProcess;
		struct 
		{
			NTSTATUS status;
			SIZE_T size;
		} retur;

		retur.status = PsLookupProcessByProcessId((HANDLE)sourcePid, &sourcePeProcess);
		if (NT_SUCCESS(retur.status)) {
			retur.status = PsLookupProcessByProcessId((HANDLE)destinyPid, &destinyPeProcess);
			if (NT_SUCCESS(retur.status)) {
				retur.status = WinDefs::MmCopyVirtualMemory(sourcePeProcess, (void*)sourceAddress, destinyPeProcess, (void*)destinyAddress, size, MODE::KernelMode, &retur.size);
				ObDereferenceObject(destinyPeProcess);
			} 
			ObDereferenceObject(sourcePeProcess);
		} 

		return retur;
	}

	auto GetModuleInProcess(ULONGLONG pid, LPCWSTR name) {

		PEPROCESS peProcess;

		NTSTATUS status;
		
		struct
		{
			uintptr_t base = 0;
			SIZE_T size = 0;
		} retur;

		status = PsLookupProcessByProcessId((HANDLE)pid, &peProcess);
		if (NT_SUCCESS(status)) {
			KeAttachProcess((PKPROCESS)peProcess);

			PLIST_ENTRY32 pListEntry;

			WinDefs::PPEB32 pPeb32 = (WinDefs::PPEB32)WinDefs::PsGetProcessWow64Process(peProcess);
			
			if (pPeb32) {
				for (pListEntry = (PLIST_ENTRY32)((WinDefs::PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList.Flink;
					pListEntry != &((WinDefs::PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList;
					pListEntry = (PLIST_ENTRY32)pListEntry->Flink)
				{
					WinDefs::PLDR_DATA_TABLE_ENTRY32 pEntry = CONTAINING_RECORD(pListEntry, WinDefs::LDR_DATA_TABLE_ENTRY32, WinDefs::LDR_DATA_TABLE_ENTRY32::InLoadOrderLinks);

					if (_wcsicmp((PWCH)pEntry->BaseDllName.Buffer, name) == 0) {
						retur.base = pEntry->DllBase;
						retur.size = pEntry->SizeOfImage;
						break;
					}
				}
			} else {
				WinDefs::PPEB pPeb = WinDefs::PsGetProcessPeb(peProcess);
				PLIST_ENTRY pListEntry;
				for (pListEntry = (PLIST_ENTRY)((WinDefs::PPEB_LDR_DATA)pPeb->Ldr)->InLoadOrderModuleList.Flink;
					pListEntry != &((WinDefs::PPEB_LDR_DATA)pPeb->Ldr)->InLoadOrderModuleList;
					pListEntry = (PLIST_ENTRY)pListEntry->Flink)
				{
					WinDefs::PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, WinDefs::LDR_DATA_TABLE_ENTRY, WinDefs::LDR_DATA_TABLE_ENTRY::InLoadOrderLinks);

					if (_wcsicmp((PWCH)pEntry->BaseDllName.pszBuffer, name) == 0) {
						retur.base = (uintptr_t)pEntry->DllBase;
						retur.size = pEntry->SizeOfImage;
						break;
					}
				}
			}
			KeDetachProcess();
			ObDereferenceObject(peProcess);
		}
		return retur;
	}
}
