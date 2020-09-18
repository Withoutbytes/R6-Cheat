#pragma once
#include <Windows.h>
#include <CommunicationDefs.hpp>
#include <exception>
#include <excpt.h>
#include <signal.h>

namespace DriverCommunication {

	using namespace CommunicationDefs;

	FunctionPrototype callfunc = nullptr;

	bool Init(std::string modName, std::string funcName) {

		auto libAddr = LoadLibraryA(modName.c_str());
		if (!libAddr) return false;

		callfunc = (FunctionPrototype)GetProcAddress(libAddr, funcName.c_str());
		return callfunc ? true : false;
	}

	bool Test() {

		PACKET_BASE packet{};
		packet.op = TEST;
		packet.side = SIDE::SERVER;

		constexpr ULONG CALL_FIRST = 1;
		constexpr ULONG CALL_LAST = 0;

		auto handlerExeception = AddVectoredExceptionHandler(CALL_FIRST, [](PEXCEPTION_POINTERS exceptionInfo) -> LONG {
			auto ctx = exceptionInfo->ContextRecord;
			ctx->Rip += 8; 
			
			return EXCEPTION_CONTINUE_EXECUTION;
		});

		if (!handlerExeception) return false;

		if (!callfunc(packet)) return false;		

		if (!RemoveVectoredExceptionHandler(handlerExeception)) return false;

		auto [valid] = packet.client.test;
		return valid;
	}

	SIZE_T CopyVirtualMemory(ULONGLONG srcPid, uintptr_t srcAddr, ULONGLONG targetPid, uintptr_t targetAddr, SIZE_T size) {
		PACKET_BASE packet{};
		packet.op = COPY_VIRTUAL_MEMORY;
		packet.side = SIDE::SERVER;

		auto& server_cpvm = packet.server.copy_virtual_memory;
		server_cpvm.sourcePid = srcPid;
		server_cpvm.sourceAddress = srcAddr;
		server_cpvm.targetPid = targetPid;
		server_cpvm.targetAddress = targetAddr;
		server_cpvm.size = size;

		if (!callfunc(packet)) throw "Request Error";
		
		auto client_cpvm = packet.client.copy_virtual_memory;

		return client_cpvm.size;
	}

	std::tuple<uintptr_t, size_t> GetModuleBaseAndSize(ULONGLONG pid, std::wstring modName) {
		if (sizeof(GET_MODULE_S::name) < modName.length())  throw "Module name size is too big";

		PACKET_BASE packet{};
		packet.op = GET_MODULE_BASE_SIZE;
		packet.side = SIDE::SERVER;

		auto& server = packet.server;
		modName.copy(server.get_module.name, modName.length());
		server.get_module.pid = pid;

		if (!callfunc(packet)) throw "Request Error";

		auto [baseAddress, module_size] = packet.client.get_module;

		return { baseAddress, module_size };
	}

	SIZE_T RemoveFromMMUnloadedDrivers(std::wstring driverName) {
		if (sizeof(REMOVE_FROM_MM_UNLOADED_DRIVERS_S::name) < driverName.length())  throw "Driver name size is too big";

		PACKET_BASE packet{};
		packet.op = REMOVE_FROM_MM_UNLOADED_DRIVERS;
		packet.side = SIDE::SERVER;

		auto& server = packet.server;

		driverName.copy(server.remove_from_mm_unloaded_drivers.name, driverName.length());

		if (!callfunc(packet)) throw "Request Error";

		auto [count] = packet.client.remove_from_mm_unloaded_drivers;

		return count;
	}

	SIZE_T RemoveFromPiDDBTable(std::wstring driverName) {
		if (sizeof(REMOVE_FROM_PiDDB_TABLE_S::name) < driverName.length())  throw "Driver name size is too big";

		PACKET_BASE packet{};
		packet.op = REMOVE_FROM_PiDDB_TABLE;
		packet.side = SIDE::SERVER;

		auto& server = packet.server;

		driverName.copy(server.remove_from_piddb_table.name, driverName.length());

		if (!callfunc(packet)) throw "Request Error";

		auto [count] = packet.client.remove_from_piddb_table;

		return count;
	}
}

