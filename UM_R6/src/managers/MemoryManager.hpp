#pragma once
#include <Windows.h>
#include <iostream>
#include "../ProcessHelper.hpp"
#include <future>
#include <string>
#ifndef _DEBUG
#include "DriverCommunication.hpp"
#else
#include <TlHelp32.h>
#endif

class MemoryManager {
protected:
	std::wstring fileName;
	std::wstring windowTitle;
	std::wstring windowClass;
	HWND windowHandle = 0;
#ifdef _DEBUG
	HANDLE process_handle = INVALID_HANDLE_VALUE;
#endif // _DEBUG
public:
	DWORD pid = -1;
	explicit MemoryManager(std::wstring fileName) : fileName(fileName) {

	}

	explicit MemoryManager(std::wstring windowTitle, std::wstring windowClass) : windowTitle(windowTitle), windowClass(windowClass) {

	}

	bool TryToAttach() {
		if (!windowTitle.empty() || !windowClass.empty()) {
			windowHandle = FindWindowW(windowClass.empty() ? NULL : windowClass.c_str(), windowTitle.empty() ? NULL : windowTitle.c_str());
			if (!windowHandle) return false;
			GetWindowThreadProcessId(windowHandle, &pid);
		}
		else pid = ProcessHelper::FindProcessByName(fileName);

		if (!pid) return false;

#ifdef _DEBUG
		process_handle = OpenProcess(PROCESS_ALL_ACCESS, false, static_cast<DWORD>(pid));
		if (!process_handle) throw "Error on create handle to process";
		return true;
#endif // _DEBUG

	}

	template <typename T>
	T Read(uintptr_t address) {
		struct Deleter { void operator() (void* x) { free(x); }; };

		std::unique_ptr<T, Deleter> ptr(reinterpret_cast<T*>(malloc(sizeof(T))));

#ifndef _DEBUG
		auto ret_size = DriverCommunication::CopyVirtualMemory(static_cast<ULONGLONG>(pid), address, GetCurrentProcessId(), (uintptr_t)ptr.get(), sizeof(T));
		if (!ret_size) throw "Read Error!";
		return *ptr;
#else
		size_t readedBytes = 0;
		if (!ReadProcessMemory(process_handle, (void*)address, ptr.get(), sizeof(T), &readedBytes)) {
			throw "Read Error: !";
		}
		if (!readedBytes) throw "Read Error!";
		return *ptr;
#endif // !_DEBUG
	}

	template <typename T>
	size_t Write(uintptr_t address, T val) {
#ifndef _DEBUG
		auto ret_size = DriverCommunication::CopyVirtualMemory(GetCurrentProcessId(), (uintptr_t)&val, static_cast<ULONGLONG>(pid), address, sizeof(T));
		if (!ret_size) throw "Write Error!";
		return ret_size;
#else
		size_t writedBytes = 0;
		if (!WriteProcessMemory(process_handle, (void*)address, &val, sizeof(T), &writedBytes)) {
			std::string str = "Write Error: "; str += std::to_string(GetLastError());
			throw str.c_str();
		}
		if (!writedBytes) throw "Write Error!";
		return writedBytes;
#endif // !_DEBUG
	}

	std::tuple<uintptr_t, size_t> GetModule(std::wstring moduleName) {
#ifndef _DEBUG
		return DriverCommunication::GetModuleBaseAndSize(static_cast<ULONGLONG>(pid), moduleName);
#else
		auto tlhandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, static_cast<DWORD>(pid));
		if (!tlhandle) throw "Error on create handle";
		MODULEENTRY32W modulew{};
		modulew.dwSize = sizeof(MODULEENTRY32W);

		if (!Module32FirstW(tlhandle, &modulew)) throw "No first module";

		do
		{
			if (moduleName.compare(modulew.szModule) == 0) {
				CloseHandle(tlhandle);
				return { reinterpret_cast<uintptr_t>(modulew.modBaseAddr), modulew.modBaseSize };
			}
		} while (Module32NextW(tlhandle, &modulew));

		throw "Module not found";
#endif // !_DEBUG
	}

	bool IsWindowValid() {
		return IsWindow(windowHandle);
	}

	template<typename T>
	T Read(std::vector<uintptr_t> address) {
		uintptr_t last = NULL;
		for (size_t i = 0; i < address.size() - 1; i++)
		{
			last = this->Read<uintptr_t>(last + address.at(i));
		}
		return this->Read<T>(last + address.at(address.size() - 1));
	}

	template<typename T>
	bool Write(std::vector<uintptr_t> address, T data) {
		uintptr_t last = NULL;
		for (size_t i = 0; i < address.size() - 1; i++)
		{
			last = this->Read<uintptr_t>(last + address.at(i));
		}
		return this->Write<T>(last + address.at(address.size() - 1), data);
	}
};

