#include "TileGrid.h"


TileGrid::TileGrid(){

}

void TileGrid::parseTilesheet(unsigned int tile_width, unsigned int tile_height){
    // Number of tiles in x and y directions contained in the tilesheet
    unsigned int tile_count_x = tilesheet_tex->w/tile_width;
    unsigned int tile_count_y = tilesheet_tex->h/tile_height;

    unsigned int tile_count = tile_count_x*tile_count_y;

    // Width and height of each tile in pixels
    float tile_size_x = 1.0/tile_count_x;
    float tile_size_y = 1.0/tile_count_y;

    tile_uv_coords = new UVSection[tile_count];

    for(unsigned int y=0; y<tile_count_y; y++) for(unsigned int x=0; x<tile_count_x; x++){
        unsigned int i=y*tile_count_x + x;

        tile_uv_coords[i].umin = x*tile_size_x;
        tile_uv_coords[i].umax = tile_uv_coords[i].umin + tile_size_x;
        tile_uv_coords[i].vmin = y*tile_size_y;
        tile_uv_coords[i].vmax = tile_uv_coords[i].vmin + tile_size_y;
    }
}


void TileGrid::draw(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tilesheet_tex->id);

    int tile_id = -1;
    UVSection *uv_coords;


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
            uv_coords = &tile_uv_coords[tile_id];

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

