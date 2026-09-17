CC=cl.exe
CFLAGS=/nologo /MT /LD /std:c++23preview /EHsc /W4
TARGET=handleex.dll
SRC=handleex.cpp
OBJ=$(SRC:.cpp=.obj)


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(SRC) $(CFLAGS) /Fe:$@

.cpp.obj:
	$(CC) $(CFLAGS) /c $< /Fo$@

clean:
	del *.dll
	del *.lib
	del *.exp
	del *.obj
