#include <windows.h>
#include <Dbgeng.h>
#include <stdio.h>
#include <string>
#include <psapi.h>

#include "handleex.h"

#define EXT_MAJOR_VER  1
#define EXT_MINOR_VER  0

extern "C" __declspec(dllexport) HRESULT CALLBACK
handleex(IDebugClient* pDebugClient, PCSTR args) {
    Handleex windbgExt(pDebugClient);
    HANDLE haimedHandle = (HANDLE)windbgExt.EvaluateArgAsINT64(args);
    std::vector<SYSTEM_HANDLE> handles = windbgExt.GetDebuggeeHandles();

    for(const auto& h : handles) {
        if((HANDLE)h.Handle == haimedHandle) {
            HANDLE handle = (HANDLE)h.Handle;
            windbgExt.DumpObjectInfo(handle);
        }
    }

    return S_OK;
}

extern "C" __declspec(dllexport) HRESULT CALLBACK
DebugExtensionInitialize(PULONG Version, PULONG Flags) {
    *Version = DEBUG_EXTENSION_VERSION(EXT_MAJOR_VER, EXT_MINOR_VER);
    *Flags = 0;
    return S_OK;
}
