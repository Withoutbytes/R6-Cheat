#pragma once
#include <intrin.h>
#include <string.h>
#include <intsafe.h>

namespace CommunicationDefs {
	enum OP_CODE : BYTE
	{
		NONE,
		TEST,
		COPY_VIRTUAL_MEMORY,
		GET_MODULE_BASE_SIZE,
		REMOVE_FROM_MM_UNLOADED_DRIVERS,
		REMOVE_FROM_PiDDB_TABLE
	};

	enum class SIDE : BYTE
	{
		NONE,
		CLIENT,
		SERVER
	};

	struct TEST_S
	{

	};

	struct COPY_VIRTUAL_MEMORY_S
	{
		ULONGLONG targetPid;
		uintptr_t targetAddress;
		ULONGLONG sourcePid;
		uintptr_t sourceAddress;
		size_t size;
	};

	struct GET_MODULE_S
	{
		ULONGLONG pid;
		wchar_t name[32];
	};

	struct REMOVE_FROM_MM_UNLOADED_DRIVERS_S
	{
		wchar_t name[32];
	};

	struct REMOVE_FROM_PiDDB_TABLE_S
	{
		wchar_t name[32];
	};

	struct TEST_C
	{
		bool valid = true;
	};

	struct COPY_VIRTUAL_MEMORY_C
	{
		size_t size;
	};

	struct GET_MODULE_C
	{
		uintptr_t baseAddress;
		size_t module_size;
	};

	struct REMOVE_FROM_MM_UNLOADED_DRIVERS_C
	{
		SIZE_T count;
	};

	struct REMOVE_FROM_PiDDB_TABLE_C
	{
		SIZE_T count;
	};

	struct PACKET_BASE
	{
		OP_CODE op;
		SIDE side;
		union
		{
			union
			{
				TEST_S test;
				COPY_VIRTUAL_MEMORY_S copy_virtual_memory;
				GET_MODULE_S get_module;
				REMOVE_FROM_MM_UNLOADED_DRIVERS_S remove_from_mm_unloaded_drivers;
				REMOVE_FROM_PiDDB_TABLE_S remove_from_piddb_table;
			} server;

			union
			{
				TEST_C test;
				COPY_VIRTUAL_MEMORY_C copy_virtual_memory;
				GET_MODULE_C get_module;
				REMOVE_FROM_MM_UNLOADED_DRIVERS_C remove_from_mm_unloaded_drivers;
				REMOVE_FROM_PiDDB_TABLE_C remove_from_piddb_table;
			} client;
		};
	};

	typedef BOOL(*FunctionPrototype)(PACKET_BASE& packet);
}
