CC=g++
exe=bezier
libs=-lSDL -lGL

all: bezier.o
	$(CC) -o $(exe) bezier.o $(libs)

bezier.o: bezier.cpp
	$(CC) -c bezier.cpp

clean:
	rm -f $(exe)
	rm -f *.o

