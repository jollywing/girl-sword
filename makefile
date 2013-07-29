# This is makefile for Linux. 
# It is also can be used in mingw (with sdl installed) on  Windows.

cflags = -I/usr/include `sdl-config --cflags` -g
lflags = `sdl-config --libs` -lSDL_ttf -g

cc = g++

bin = girl-sword
objs = Girl.o Map.o Role.o Fighter.o Menu.o Record.o Dialog.o SdlSys.o\
	Variable.o Script.o

# on mingw, $(lflags) must be placed after $(objs)
$(bin): $(objs)
	$(cc) -o $@ $(objs) $(lflags)

SdlSys.o: SdlSys.h
Dialog.o: Dialog.h SdlSys.h
Menu.o: Menu.h SdlSys.h Girl.h
Girl.o: Dialog.h Menu.h Girl.h Script.h Variable.h
Script.o: Script.h Variable.h

%.o:%.cpp
	$(cc) $(cflags) -c $<

.PHONY: clean

clean:
	-rm -f *.o $(bin)

