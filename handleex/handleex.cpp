#include "handleex.h"

Handleex::Handleex(IDebugClient* pDebugClient) : WinDBGExt(pDebugClient) { }

void Handleex::DumpObjectInfo(HANDLE const handle) {
    _NtDuplicateObject NtDuplicateObject = (_NtDuplicateObject)GetProcAddress(LoadLibrary("ntdll.dll"), "NtDuplicateObject");
    _NtQueryObject NtQueryObject = (_NtQueryObject)GetProcAddress(LoadLibrary("ntdll.dll"), "NtQueryObject");
    POBJECT_TYPE_INFORMATION pObjectTypeInfo = { 0 };
    PVOID pObjectNameInfo = { 0 };
    UNICODE_STRING usObjectName = { 0 };
    ULONG ulSize = 0;
    NTSTATUS status = 0;
    HANDLE dupHandle = nullptr;

    // Duplicate the handle so we can query it.
    if(!NT_SUCCESS(NtDuplicateObject(
	GetDebuggeeHandle(), // This handle has all what we need!
	(void*) handle,
	GetCurrentProcess(),
	&dupHandle,
	0,
	0,
	0
	))) {

	PrintOut("Error Duplicating handle 0x%x\n", handle);
	return;	
    }

    status = NtQueryObject(dupHandle, ObjectTypeInformation, nullptr, 0, &ulSize);

    if (status == STATUS_INFO_LENGTH_MISMATCH) {
	PVOID buffer = malloc(ulSize);
	status = NtQueryObject(dupHandle, ObjectTypeInformation, buffer, ulSize, nullptr);
	if (NT_SUCCESS(status)) 
	{
            POBJECT_NAME_INFORMATION objectNameInfo = (POBJECT_NAME_INFORMATION)buffer;
            PrintOut("Object Type: %ws\n", objectNameInfo->Name.Buffer);
        }

	status = NtQueryObject(dupHandle, ObjectNameInformation, nullptr, 0, &ulSize);
	if (status == STATUS_INFO_LENGTH_MISMATCH) {
	    buffer = malloc(ulSize);
	    status = NtQueryObject(dupHandle, ObjectNameInformation, buffer, ulSize, nullptr);
	    if (NT_SUCCESS(status)) 
	    {
		POBJECT_NAME_INFORMATION objectNameInfo = (POBJECT_NAME_INFORMATION)buffer;
		PrintOut("Handle Name: %ws\n", objectNameInfo->Name.Buffer);
	    }
	}
        // Free the buffer.
        free(buffer);
	buffer = nullptr;
    }
}
