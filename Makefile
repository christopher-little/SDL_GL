CC=g++
exe=bezier

#Detect a mingw32 (windows 32 bit) environment, otherwise assume linux for now
UNAME := $(shell uname)

ifeq ($(UNAME),MINGW32_NT-6.1)
	exe:=$(exe).exe
	include=-Ic:/SDL-1.2.15/include
	libs=-Lc:/SDL-1.2.15/lib -lmingw32 -lSDLmain -lSDL -lopengl32
else
	include=
	libs=-lSDL -lGL
endif

all: bezier.o
	$(CC) -o $(exe) bezier.o $(libs)

bezier.o: bezier.cpp
	$(CC) -c bezier.cpp $(include)

clean:
	rm -f $(exe)
	rm -f *.o

