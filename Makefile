CC=g++
exe=engine
src=src
bin=bin

#Detect a mingw32 (windows 32 bit) environment, otherwise assume linux for now
UNAME := $(shell uname)

ifeq ($(UNAME),MINGW32_NT-6.1)
	exe:=$(exe).exe
	include=-Ic:/SDL-1.2.15/include
	libs=-Lc:/SDL-1.2.15/lib -lmingw32 -lSDLmain -lSDL -lSDL_image -lopengl32
else
	include=
	libs=-lSDL -lSDL_image -lGL
endif

all: $(bin)/main.o
	$(CC) -o $(exe) $(bin)/main.o $(libs)

$(bin)/main.o: $(src)/main.cpp
	$(CC) -o $(bin)/main.o -c $(src)/main.cpp $(include)

clean:
	rm -f $(exe)
	rm -f $(bin)/*.o

