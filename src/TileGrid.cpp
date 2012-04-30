#include "TileGrid.h"


TileGrid::TileGrid(){
    tilesheet_tex = NULL;
}

void TileGrid::parseTilesheet(Texture *tilesheet_texture, unsigned int tile_width, unsigned int tile_height){
    tilesheet_tex = tilesheet_texture;


    // Number of tiles in x and y directions contained in the tilesheet
    unsigned int tile_count_x = tilesheet_tex->w/tile_width;
    unsigned int tile_count_y = tilesheet_tex->h/tile_height;

    unsigned int tile_count = tile_count_x*tile_count_y;

    // Width and height of each tile in u,v coordinates
    float tile_size_x = 1.0/tile_count_x;
    float tile_size_y = 1.0/tile_count_y;

    // Initialize list of tile types
    tile_types_list = new Tile[tile_count];

    UVSection *uv_coords = NULL;
    for(unsigned int y=0; y<tile_count_y; y++) for(unsigned int x=0; x<tile_count_x; x++){
        unsigned int i=y*tile_count_x + x;
        uv_coords = &tile_types_list[i].uv_coords;

        uv_coords->umin = x*tile_size_x;
        uv_coords->umax = uv_coords->umin + tile_size_x;
        uv_coords->vmin = y*tile_size_y;
        uv_coords->vmax = uv_coords->vmin + tile_size_y;
    }
}


void TileGrid::draw(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tilesheet_tex->id);

    int tile_id = -1;
    UVSection *uv_coords = NULL;


    int x=0;
    list<TileColumn>::iterator i;
    for(i=columns.begin(); i != columns.end(); i++){
        for(int y=0; y<TileColumn::height; y++){
            // Retrieve the tile type id for the current tile
            tile_id = i->tiles[y];

            // Skip the "air" tile (id = -1)
            if(tile_id < 0)
                continue;

            // Retreive UVSection for this tile
            uv_coords = &tile_types_list[tile_id].uv_coords;

            // Draw the tile
            glPushMatrix();
            // ***Translate the tiles "inversely" since screen origin is top left
            glTranslatef(x*TileGrid::tile_size, (TileColumn::height-1-y)*TileGrid::tile_size, 0);
            glBegin(GL_QUADS);
                glTexCoord2f(uv_coords->umin, uv_coords->vmin);
                glVertex2f(0.0,0.0);

                glTexCoord2f(uv_coords->umax, uv_coords->vmin);
                glVertex2f(TileGrid::tile_size, 0.0);

                glTexCoord2f(uv_coords->umax, uv_coords->vmax);
                glVertex2f(TileGrid::tile_size, TileGrid::tile_size);

                glTexCoord2f(uv_coords->umin, uv_coords->vmax);
                glVertex2f(0.0, TileGrid::tile_size);
            glEnd();
            glPopMatrix();
        }

        x++;
    }

    glDisable(GL_TEXTURE_2D);
}

