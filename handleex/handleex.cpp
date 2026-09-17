#pragma warning(disable: 4244)

/*++

Copyright (c) Taha Draidia. All rights reserved.

Module Name:

        handleex.cpp

Abstract:

	Windbg extension that allows retrieval of the name of an object associated with a handle from userspace, a capability not offered by existing extensions such as `!handle`

Environment:

        User mode, Windows.

Author:

        Taha Draidia (@tahadraidia)

Copyright:

        Use of this source code is governed by a MIT-style license that
        can be found in the LICENSE file.

--*/

// Leave this as it is.
// Workaround duplication redefinition compiler warning.
#ifndef WIN32_NO_STATUS

#define WIN32_NO_STATUS
#include <Windows.h>
#undef WIN32_NO_STATUS

#include <ntstatus.h>

#endif

#include "windbgext.hpp"
#include "ntstuff.h"

NTSTATUS
DumpObjName(
	IN HANDLE hSource,
	IN HANDLE hObject,
	OUT PUNICODE_STRING* ppObjectName
	)
/*++

Routine Description:

	The function retrieves the object name of a object.

	The object needs to have a name.

Arguments:

	hSource - HANDLE of debuggee process.
	hObject - HANDLE of the object we want retrieve its name.
	ppObjectName - if successfull will hold object name otherwise it will hold NULL.

Return value:

	STATUS_SUCCESS on success, otherwise the appropriate NTSTATUS error code.

--*/
{
	if (!ppObjectName || !hSource || !hObject)
		return STATUS_INVALID_PARAMETER;

	*ppObjectName = nullptr;

	NTSTATUS status{STATUS_SUCCESS};
	HANDLE hDup{nullptr};
	ULONG size{};
	PVOID buffer{};

	status = NtDuplicateObject(
			hSource,
			hObject,
			::GetCurrentProcess(), // Dbgengine process.
			&hDup,
			0,
			0,
			DUPLICATE_SAME_ACCESS
		);

	if (!NT_SUCCESS(status))
		return status;

	status = NtQueryObject(hDup,
			(OBJECT_INFORMATION_CLASS)ObjectNameInformation,
			nullptr,
			0,
			&size
		);

	if (status != STATUS_INFO_LENGTH_MISMATCH &&
		status != STATUS_BUFFER_TOO_SMALL &&
		status != STATUS_BUFFER_OVERFLOW
		)
	{
		goto cleanup;
	}

	buffer = std::malloc(size);

	if (!buffer)
		goto cleanup;

	status = NtQueryObject(
			hDup,
			(OBJECT_INFORMATION_CLASS)ObjectNameInformation,
			buffer,
			size,
			nullptr
		);

	if (NT_SUCCESS(status)) {
		PNTSTUFF_OBJECT_NAME_INFORMATION pObjInfo = (PNTSTUFF_OBJECT_NAME_INFORMATION)buffer;

		*ppObjectName = static_cast<PUNICODE_STRING>(malloc(sizeof(UNICODE_STRING)));

		std::memcpy(*ppObjectName, &pObjInfo->Name, sizeof(UNICODE_STRING));

		goto cleanup;
	}

cleanup:

	if (hDup) {
		NtClose(hDup);
		hDup = nullptr;
	}

	if (buffer) {
		free(buffer);
		buffer = nullptr;
	}

	return status;
}

#define EXT_MAJOR_VER  1
#define EXT_MINOR_VER  0

WDBG_CMD(handleex)
{
	const auto pArgument = pExt->GetRawArgs();
	const auto hSource = pExt->GetDebuggeeHandle();
	PUNICODE_STRING pObjName{};

	if (!pArgument || !*pArgument) {
		pExt->Err() << "no argument passed";
		return E_FAIL;
	}

	const auto hObject = pExt->EvaluateU64(pArgument);

	if (!hObject) {
		pExt->Err() << "parsing handle value failed";
		return E_FAIL;
	}

	if (!hSource) {
		pExt->Err() << "Failed retrieving debuggee handle";
		return E_FAIL;
	}

	auto status = DumpObjName(
			(HANDLE)*hSource,
			(HANDLE)*hObject,
			&pObjName
			);

	if (!NT_SUCCESS(status) || !pObjName) {
		pExt->Err() << "Failed retrieving name object, NTSTATUS 0x " << std::hex << status;
		return E_FAIL;
	}


	if (pObjName->Length) {

		auto wideName = std::wstring(
					pObjName->Buffer,
					pObjName->Length / sizeof(WCHAR)
				);

		auto objectName = std::string(wideName.begin(), wideName.end());

		pExt->Out() << "Handle: " << std::hex << *hObject << "\nObject name: " << objectName;
	}

	return S_OK;
}

WDBG_CMD_VERSION(EXT_MAJOR_VER, EXT_MINOR_VER)
