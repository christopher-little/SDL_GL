#ifndef TILEGRID_H
#define TILEGRID_H

#include <SDL/SDL_opengl.h>

class Tilegrid
{
public:
    Tilegrid(int width, int height);

private:
    int width;
    int height;
    int *tiles;
};

#endif // TILEGRID_H
