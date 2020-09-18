#pragma once
#include <intsafe.h>
#include <intrin.h>
#include <string.h>
#include <ntddk.h>

namespace HookHelper {

	BYTE* MakeDetourHook(void* targetFunction, void* callbackFunction) {
		//BYTE shellCode[sizeof(void*) + 2 + 4]{
		//	0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, // mov rax, [xxx]
		//	0xFF, 0xE0,							// jmp rax
		//	0xCC								// int 3
		//};

		BYTE shellCode[sizeof(void*) + 2 + 4]{
			0xFF, 0x25, 0, 0, 0, 0 // JMP QWORD PTR [address]
		};

		*reinterpret_cast<uintptr_t*>(shellCode + 6) = (uintptr_t)callbackFunction;

		BYTE* oldBytes = (BYTE*)MmAllocateNonCachedMemory(sizeof(shellCode));
		if (!oldBytes) return false;

		const auto oldcr0 = __readcr0();

		auto newcr0 = oldcr0 & -0b10000000000000001;

		__writecr0(newcr0);

		memcpy(oldBytes, targetFunction, sizeof(shellCode));
		memcpy(targetFunction, shellCode, sizeof(shellCode));

		__writecr0(oldcr0);

		return oldBytes;
	}


	BYTE* MakeTrampolineHook(void* targetFunction, void* callbackFunction) {
		BYTE* gateway = reinterpret_cast<BYTE*>(ExAllocatePool(POOL_TYPE::PagedPool, 0x100));

		if (!gateway) return nullptr;

		SIZE_T len = sizeof(DWORD) + 1;

		BYTE* oldBytes = MakeDetourHook(targetFunction, callbackFunction);

		memcpy(gateway, oldBytes, len);

		BYTE shellCode[sizeof(DWORD) + 1]{ 0xE9 };

		DWORD relativeAddress = reinterpret_cast<uintptr_t>(targetFunction)
			- reinterpret_cast<uintptr_t>(callbackFunction)
			- sizeof(shellCode);

		*reinterpret_cast<DWORD*>(shellCode + 1) = relativeAddress;

		memcpy(gateway + len, shellCode, sizeof(shellCode));

		return gateway;
	}

};

