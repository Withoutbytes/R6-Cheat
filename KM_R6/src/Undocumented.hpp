#pragma once
#include <ntddk.h>

extern "C" NTKERNELAPI
PVOID
NTAPI
RtlFindExportedRoutineByName(
	_In_ PVOID ImageBase,
	_In_ PCCH RoutineNam
);


extern "C"
NTKERNELAPI
PVOID NTAPI PsGetProcessWow64Process(PEPROCESS process);


typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemBasicInformation,
    SystemProcessorInformation,
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemPathInformation,
    SystemProcessInformation,
    SystemCallCountInformation,
    SystemDeviceInformation,
    SystemProcessorPerformanceInformation,
    SystemFlagsInformation,
    SystemCallTimeInformation,
    SystemModuleInformation
} SYSTEM_INFORMATION_CLASS,
* PSYSTEM_INFORMATION_CLASS;

extern "C" NTKERNELAPI
NTSTATUS ZwQuerySystemInformation(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

typedef struct _RTL_PROCESS_MODULE_INFORMATION
 {
         HANDLE Section;
         PVOID MappedBase;
         PVOID ImageBase;
         ULONG ImageSize;
         ULONG Flags;
         USHORT LoadOrderIndex;
         USHORT InitOrderIndex;
         USHORT LoadCount;
         USHORT OffsetToFileName;
         UCHAR FullPathName[256];
     } RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

 typedef struct _RTL_PROCESS_MODULES
 {
         ULONG NumberOfModules;
         RTL_PROCESS_MODULE_INFORMATION Modules[1];
     } RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;