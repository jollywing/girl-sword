# This is makefile for Linux. 
# It is also can be used in mingw (with sdl installed) on  Windows.

cflags = -I/usr/include `sdl-config --cflags`
lflags = `sdl-config --libs` -lSDL_ttf

cc = g++

bin = girl-sword
objs = Girl.o Map.o Role.o Fighter.o Menu.o Record.o Dialog.o assist.o

# on mingw, $(lflags) must be placed after $(objs)
$(bin): $(objs)
	$(cc) -o $@ $(objs) $(lflags)

%.o:%.cpp
	$(cc) $(cflags) -c $<

.PHONY: clean

clean:
	-rm -f *.o $(bin)

