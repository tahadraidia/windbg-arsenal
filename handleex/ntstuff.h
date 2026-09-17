#ifndef _NTSTUFF_H
#define _NTSTUFF_H

// This needs to be inserted prior a windows header.
#ifndef UNICODE
#define UNICODE
#endif

#include <ntstatus.h>
#include <winternl.h>

#pragma comment(lib, "ntdll.lib")

#define SystemHandleInformation 16
#define ObjectBasicInformation 0
#define ObjectNameInformation 1
#define ObjectTypeInformation 2

typedef struct _NTSTUFF_SYSTEM_HANDLE {
    ULONG ProcessId;
    BYTE ObjectTypeNumber;
    BYTE Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
} NTSTUFF_SYSTEM_HANDLE, *PNTSTUFF_SYSTEM_HANDLE;
 
typedef struct _NTSTUFF_SYSTEM_HANDLE_INFORMATION {
    ULONG HandleCount;
    NTSTUFF_SYSTEM_HANDLE Handles[1];
} NTSTUFF_SYSTEM_HANDLE_INFORMATION, *PNTSTUFF_SYSTEM_HANDLE_INFORMATION;
 
typedef enum _POOL_TYPE {
    NonPagedPool,
    PagedPool,
    NonPagedPoolMustSucceed,
    DontUseThisType,
    NonPagedPoolCacheAligned,
    PagedPoolCacheAligned,
    NonPagedPoolCacheAlignedMustS
} POOL_TYPE, *PPOOL_TYPE;
 
typedef struct _NTSTUFF_OBJECT_TYPE_INFORMATION {
    UNICODE_STRING Name;
    ULONG TotalNumberOfObjects;
    ULONG TotalNumberOfHandles;
    ULONG TotalPagedPoolUsage;
    ULONG TotalNonPagedPoolUsage;
    ULONG TotalNamePoolUsage;
    ULONG TotalHandleTableUsage;
    ULONG HighWaterNumberOfObjects;
    ULONG HighWaterNumberOfHandles;
    ULONG HighWaterPagedPoolUsage;
    ULONG HighWaterNonPagedPoolUsage;
    ULONG HighWaterNamePoolUsage;
    ULONG HighWaterHandleTableUsage;
    ULONG InvalidAttributes;
    GENERIC_MAPPING GenericMapping;
    ULONG ValidAccess;
    BOOLEAN SecurityRequired;
    BOOLEAN MaintainHandleCount;
    USHORT MaintainTypeList;
    POOL_TYPE PoolType;
    ULONG PagedPoolUsage;
    ULONG NonPagedPoolUsage;
} NTSTUFF_OBJECT_TYPE_INFORMATION, *PNTSTUFF_OBJECT_TYPE_INFORMATION;

typedef struct _NTSTUFF_OBJECT_NAME_INFORMATION {
    UNICODE_STRING Name;
} NTSTUFF_OBJECT_NAME_INFORMATION, *PNTSTUFF_OBJECT_NAME_INFORMATION;


extern "C"
__declspec(dllimport)
NTSTATUS
NTAPI
NtDuplicateObject(
    _In_ HANDLE SourceProcessHandle,
    _In_ HANDLE SourceHandle,
    _In_opt_ HANDLE TargetProcessHandle,
    _Out_opt_ PHANDLE TargetHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ ULONG HandleAttributes,
    _In_ ULONG Options
    );

extern "C"
__declspec(dllimport)
NTSTATUS
NTAPI
NtQueryObject(
    _In_opt_ HANDLE Handle,
    _In_ OBJECT_INFORMATION_CLASS ObjectInformationClass,
    _Out_writes_bytes_opt_(ObjectInformationLength) PVOID ObjectInformation,
    _In_ ULONG ObjectInformationLength,
    _Out_opt_ PULONG ReturnLength
    );

#endif
