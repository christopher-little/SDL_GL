#ifndef TILE_H
#define TILE_H

#include <SDL/SDL_opengl.h>

class Tile{
public:
    GLuint texID; //OpenGL generated texture identifier

    float u,v; //Texture coordinates
    float d_u,d_v; //Width and height in u,v space
};

#endif // TILE_H
