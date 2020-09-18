#pragma once
#include <ntifs.h>
#include <intrin.h>
#include <windef.h>


namespace WinDefs {
	EXTERN_C_START
        typedef struct _LSA_UNICODE_STRING {
        WORD        wLength;
        WORD        wMaximumLength;
        LPWSTR    pszBuffer;
    } LSA_UNICODE_STRING, * PLSA_UNICODE_STRING, UNICODE_STRING, * PUNICODE_STRING;

    typedef struct _LDR_MODULE {
        LIST_ENTRY        InLoadOrderModuleList;
        LIST_ENTRY        InMemoryOrderModuleList;
        LIST_ENTRY        InInitializationOrderModuleList;
        LPVOID            pBaseAddress;
        LPVOID            pEntryPoint;
        DWORD            dwSizeOfImage;
        UNICODE_STRING    FullDllName;
        UNICODE_STRING    BaseDllName;
        DWORD            dwFlags;
        short            sLoadCount;
        short            sTlsIndex;
        LIST_ENTRY        HashTableEntry;
        DWORD            dwTimeDateStamp;
    } LDR_MODULE, * PLDR_MODULE;

    typedef struct _PEB_LDR_DATA_DUMMY_ {
        DWORD        dwLength;
        BYTE        bInitialized;
        LPVOID        SsHandle;
        LIST_ENTRY    InLoadOrderModuleList;
        LIST_ENTRY    InMemoryOrderModuleList;
        LIST_ENTRY    InInitializationOrderModuleList;
        LPVOID        EntryInProgress;
    } PEB_LDR_DATA, * PPEB_LDR_DATA;

    typedef struct _RTL_USER_PROCESS_PARAMETERS
    {
        BYTE            Reserved1[16];
        LPVOID            Reserved2[10];
        UNICODE_STRING    ImagePathName;
        UNICODE_STRING    CommandLine;
    } RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

    typedef void(__stdcall* PPS_POST_PROCESS_INIT_ROUTINE)(void);

    typedef struct _PEB {
        BYTE                            Reserved1[2];
        BYTE                            BeingDebugged;
        BYTE                            Reserved2[1];
        LPVOID                            Reserved3[2];
        PPEB_LDR_DATA                    Ldr;
        PRTL_USER_PROCESS_PARAMETERS    ProcessParameters;
        BYTE                            Reserved4[104];
        LPVOID                            Reserved5[52];
        PPS_POST_PROCESS_INIT_ROUTINE    PostProcessInitRoutine;
        BYTE                            Reserved6[128];
        LPVOID                            Reserved7[1];
        DWORD                            SessionId;
    } PEB, * PPEB;


    typedef struct _LDR_DATA_TABLE_ENTRY
    {
        LIST_ENTRY InLoadOrderLinks;
        LIST_ENTRY InMemoryOrderLinks;
        LIST_ENTRY InInitializationOrderLinks;
        PVOID DllBase;
        PVOID EntryPoint;
        ULONG SizeOfImage;
        UNICODE_STRING FullDllName;
        UNICODE_STRING BaseDllName;
        ULONG Flags;
        WORD LoadCount;
        WORD TlsIndex;
        union
        {
            LIST_ENTRY HashLinks;
            struct
            {
                PVOID SectionPointer;
                ULONG CheckSum;
            };
        };
        union
        {
            ULONG TimeDateStamp;
            PVOID LoadedImports;
        };
        VOID* EntryPointActivationContext;
        PVOID PatchInformation;
        LIST_ENTRY ForwarderLinks;
        LIST_ENTRY ServiceTagLinks;
        LIST_ENTRY StaticLinks;
    } LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;


#pragma region 32Bits Defs

    typedef struct _PEB32
    {
        UCHAR InheritedAddressSpace;
        UCHAR ReadImageFileExecOptions;
        UCHAR BeingDebugged;
        UCHAR BitField;
        ULONG Mutant;
        ULONG ImageBaseAddress;
        ULONG Ldr;
        ULONG ProcessParameters;
        ULONG SubSystemData;
        ULONG ProcessHeap;
        ULONG FastPebLock;
        ULONG AtlThunkSListPtr;
        ULONG IFEOKey;
        ULONG CrossProcessFlags;
        ULONG UserSharedInfoPtr;
        ULONG SystemReserved;
        ULONG AtlThunkSListPtr32;
        ULONG ApiSetMap;
    } PEB32, * PPEB32;

    typedef struct _PEB_LDR_DATA32
    {
        ULONG Length;
        UCHAR Initialized;
        ULONG SsHandle;
        LIST_ENTRY32 InLoadOrderModuleList;
        LIST_ENTRY32 InMemoryOrderModuleList;
        LIST_ENTRY32 InInitializationOrderModuleList;
    } PEB_LDR_DATA32, * PPEB_LDR_DATA32;

    typedef struct _LDR_DATA_TABLE_ENTRY32
    {
        LIST_ENTRY32 InLoadOrderLinks;
        LIST_ENTRY32 InMemoryOrderLinks;
        LIST_ENTRY32 InInitializationOrderLinks;
        ULONG DllBase;
        ULONG EntryPoint;
        ULONG SizeOfImage;
        UNICODE_STRING32 FullDllName;
        UNICODE_STRING32 BaseDllName;
        ULONG Flags;
        USHORT LoadCount;
        USHORT TlsIndex;
        LIST_ENTRY32 HashLinks;
        ULONG TimeDateStamp;
    } LDR_DATA_TABLE_ENTRY32, * PLDR_DATA_TABLE_ENTRY32;


#pragma endregion




	
		NTSTATUS NTAPI MmCopyVirtualMemory
		(
			PEPROCESS SourceProcess,
			PVOID SourceAddress,
			PEPROCESS TargetProcess,
			PVOID TargetAddress,
			SIZE_T BufferSize,
			KPROCESSOR_MODE PreviousMode,
			PSIZE_T ReturnSize
		);

	
		NTKERNELAPI
		PPEB
		PsGetProcessPeb(
			IN PEPROCESS Process
		);

        NTKERNELAPI
        PVOID
            PsGetProcessWow64Process(
                PEPROCESS Process
            );

    EXTERN_C_END
}
