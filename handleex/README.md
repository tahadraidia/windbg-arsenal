# Handleex

Handleex is a Windbg extension written in C++ that allows retrieval of the name of an object associated with a handle from userspace, a capability not offered by existing extensions such as `!handle`.

# Compile

```cmd
PS C:\Users\tahai\Code\windbg-arsenal\handleex> nmake -f .\Makefile

Microsoft (R) Program Maintenance Utility Version 14.29.30147.0
Copyright (C) Microsoft Corporation.  All rights reserved.

        cl.exe ext.cpp handleex.cpp ../windbgext/windbgext.cpp /I ../windbgext /nologo /MT /LD /std:c++17 /EHsc /Fe:handleex.dll
ext.cpp
handleex.cpp
windbgext.cpp
Generating Code...
   Creating library handleex.lib and object handleex.exp
PS C:\Users\tahai\Code\windbg-arsenal\handleex>
```

# Usage

```sh
0:007> .load C:\Users\tahai\Code\windbg-arsenal\handleex\handleex.dll
0:007> !handle 0 1 file
Handle 40
  Type         	File
Handle 84
  Type         	File
Handle b0
  Type         	File
Handle 130
  Type         	File
Handle 1a8
  Type         	File
Handle 258
  Type         	File
Handle 30c
  Type         	File
Handle 314
  Type         	File
8 handles of type File
0:007> !handleex 84
Object Type: File

Handle Name: \Device\HarddiskVolume3\Windows\WinSxS\amd64_microsoft.windows.common-controls_6595b64144ccf1df_6.0.19041.1110_none_60b5254171f9507e

0:007> .unload
Unloading C:\Users\tahai\Code\windbg-arsenal\handleex\handleex.dll extension DLL
```
