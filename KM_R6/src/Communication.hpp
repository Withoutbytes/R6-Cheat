#pragma once
#include "Heper.hpp"
#include <CommunicationDefs.hpp>
#include "HookHelper.hpp"
#include <PIDDBAndMmUnloadedDrivers.hpp>


using namespace CommunicationDefs;
class Communication {
public:
	LPSTR functionName;
	LPSTR moduleName;
	BYTE* oldBytes = nullptr;
	const SIZE_T bytes_length = sizeof(void*) + 6;

	Communication(LPSTR moduleName, LPSTR functionName) 
		: moduleName(moduleName), functionName(functionName) {}


	static BOOL CallBack(PACKET_BASE& packet) {
		using namespace Helper;
		DbgBreakPoint();
		if (packet.op == NONE) return false;
		if (packet.side != SIDE::SERVER) return false;

		auto server_p = packet.server;

		switch (packet.op)
		{
			case COPY_VIRTUAL_MEMORY: {
				auto [targetPid, targetAddress, sourcePid, sourceAddress, size] = server_p.copy_virtual_memory;
				auto ret = CopyVirtualMemory(sourcePid, sourceAddress, targetPid, targetAddress, size);
				RtlSecureZeroMemory(&packet.client, sizeof(packet.client));
				packet.side = SIDE::CLIENT;
				packet.client.copy_virtual_memory.size = ret.size;
				return NT_SUCCESS(ret.status);
			}

			case GET_MODULE_BASE_SIZE: {
				auto [pid, name] = server_p.get_module;
				auto [base, size] = GetModuleInProcess(pid, name);
				RtlSecureZeroMemory(&packet.client, sizeof(packet.client));
				packet.side = SIDE::CLIENT;
				packet.client.get_module.baseAddress = base;
				packet.client.get_module.module_size = size;
				return true;
			}

			case REMOVE_FROM_MM_UNLOADED_DRIVERS: {
				auto [name] = packet.server.remove_from_mm_unloaded_drivers;

				UNICODE_STRING name_unicode;
				RtlInitUnicodeString(&name_unicode, name);

				SIZE_T count = PIDDBAndMmUnloadedDrivers::RemoveFromMmUnloadedDrivers(name_unicode);

				RtlSecureZeroMemory(&packet.client, sizeof(packet.client));
				packet.side = SIDE::CLIENT;
				packet.client.remove_from_mm_unloaded_drivers.count = count;

				return true;
			}

			case REMOVE_FROM_PiDDB_TABLE: {
				auto [name] = packet.server.remove_from_piddb_table;

				UNICODE_STRING name_unicode;
				RtlInitUnicodeString(&name_unicode, name);

				SIZE_T count = PIDDBAndMmUnloadedDrivers::RemoveFromPiDDBCacheTable(name_unicode);

				RtlSecureZeroMemory(&packet.client, sizeof(packet.client));
				packet.side = SIDE::CLIENT;
				packet.client.remove_from_piddb_table.count = count;
				return true;
			}

			case TEST: {
				RtlSecureZeroMemory(&packet.client, sizeof(packet.client));
				packet.side = SIDE::CLIENT;
				packet.client.test.valid = true;
				return true;
			}
			default:
				return false;
		}
	}


	bool MakeHook() {
		using namespace ModuleHelper;
		using namespace HookHelper;

		auto target = GetExportedRoutineByModuleAndName(moduleName, functionName);

		if (!target) return false;

		oldBytes = MakeDetourHook(target, &CallBack);

		if (!oldBytes) return false;

		return true;
	}

	bool UndoHook() {
		using namespace ModuleHelper;

		if (!oldBytes) return false;

		auto target = GetExportedRoutineByModuleAndName(moduleName, functionName);

		if (!target) return false;

		__writecr0(__readcr0());

		const auto oldcr0 = __readcr0();

		auto newcr0 = oldcr0 & -0b10000000000000001;

		__writecr0(newcr0);

		memcpy(target, oldBytes, bytes_length);

		__writecr0(oldcr0);

		return true;
	}

	void* operator new(size_t size) {
		return ExAllocatePool(NonPagedPool, size);
	}

	void operator delete(void* addr) {
		ExFreePool(addr);
	}
	
};
