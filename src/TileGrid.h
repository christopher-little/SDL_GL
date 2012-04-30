#ifndef TILEGRID_H
#define TILEGRID_H

#include <list>
using namespace std;

#include <SDL/SDL_opengl.h>

#include "Texture.h"
#include "Tile.h"



/* ***Tile type identifier acts as index to tilesheet array
  -1 = air (blank, passable)
  0 = first tile in tilesheet (ie grass)
  1..
  2..
*/


class TileColumn{
public:
    const static int height = 64; // ***Maximum height of tile grid

    int tiles[height]; // List of tile type identifiers
    // Note tile order is bottom to top

    TileColumn(){
        // Initialize all tiles to default ("air") tiles
        for(int i=0; i<height; i++){
            tiles[i] = -1;
        }
    }
};



class TileGrid
{
public:
    // Dimensions of a tile when displayed
    const static float tile_size = 32.0;


    // Image containing all tile graphics
    Texture *tilesheet_tex;

    // List of tile types
    Tile *tile_types_list;


    // Each column contains a list of tile type id's.
    list<TileColumn> columns;





    TileGrid();

    // Breakup tilesheet texture into component tiles
    void parseTilesheet(Texture *tilesheet_texture, unsigned int tile_width, unsigned int tile_height);

    void draw();



    // ***Create a default grid for testing purposes
    static TileGrid* createDefaultTileGrid(){
        TileGrid *tile_grid = new TileGrid();

        // Create a default tile column (bottom to top: dirt, grass, air)
        TileColumn column;

        for(int j=0; j<5; j++){
            column.tiles[j] = 1; // Dirt
        }
        column.tiles[5] = 0; // grass
        // ***Remaining tiles default to "air" empty tile

        // Insert 20 copies of the default column (enough to fill the default screen size)
        tile_grid->columns.insert(tile_grid->columns.begin(), 20, column);

        // Segregate the tile uv coordinates (assuming all tile images are 64x64 pixels)
        tile_grid->parseTilesheet(new Texture((char *)"assets/tiles.png"), 64,64);


        return tile_grid;
    }
};

#endif // TILEGRID_H
