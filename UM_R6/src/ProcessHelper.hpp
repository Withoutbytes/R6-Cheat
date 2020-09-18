#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

namespace ProcessHelper {
	ULONGLONG FindProcessByName(std::wstring name) {
		auto helper = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (!helper) throw;
		PROCESSENTRY32W procEntry{sizeof(PROCESSENTRY32W)};
		if (!Process32FirstW(helper, &procEntry)) throw;

		ULONGLONG pid = 0;

		do
		{
			if (name.compare(procEntry.szExeFile) == 0) {
				pid = procEntry.th32ProcessID;
				break;
			}
		} while (Process32NextW(helper, &procEntry));


		CloseHandle(helper);
		return pid;
	}
};
