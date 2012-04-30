#ifndef TILE_H
#define TILE_H


#include "Texture.h"

//==============================================================================
// Tile class
//
// Contains parameters for a tile type
//==============================================================================

class Tile{
public:
    UVSection uv_coords; // UV section of tilesheet texture

    bool passable; // True if the player can move through (cannot walk on) the tile
};

#endif // TILE_H
