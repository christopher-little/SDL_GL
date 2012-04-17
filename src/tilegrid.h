#ifndef TILEGRID_H
#define TILEGRID_H

#include <SDL/SDL_opengl.h>

static int tileSize = 32;

class Tilegrid
{
public:
    Tilegrid();

private:
    int tiles[(640/tileSize) * (480/tileSize)];
};

#endif // TILEGRID_H
