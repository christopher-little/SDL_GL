CC=g++
exe=game.exe
libs=-lSDL -lGL

all: SDL_GL.o
	$(CC) -o $(exe) SDL_GL.o $(libs)

SDL_GL.o: SDL_GL.cpp
	$(CC) -c SDL_GL.cpp

clean:
	rm -f $(exe)
	rm -f *.o

