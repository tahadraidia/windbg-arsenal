#ifndef _HANDLEEX_H
#define _HANDLEEX_H

#include "windbgext.h"

class Handleex : public WinDBGExt {
public:
    Handleex(IDebugClient* pDebugClient);
    void DumpObjectInfo(HANDLE const handle);

};

#endif
