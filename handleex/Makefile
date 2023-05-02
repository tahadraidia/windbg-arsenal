CC=cl.exe
CFLAGS=/nologo /MT /LD /std:c++17 /EHsc
WINDBGEXT_INCLUDES=/I ../windbgext
WINDBGEXT_SOURCE=../windbgext/windbgext.cpp
WINDBGEXT_FILENAME=handleex.dll

handleex.dll:
	$(CC) ext.cpp handleex.cpp $(WINDBGEXT_SOURCE) $(WINDBGEXT_INCLUDES) $(CFLAGS) /Fe:$(WINDBGEXT_FILENAME)

clean:
	del *.dll
	del *.lib
	del *.exp
	del *.obj
