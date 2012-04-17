#include "tilegrid.h"

Tilegrid::Tilegrid(int width, int height){
    this->width = width;
    this->height = height;

    tiles = new int[width*height];
    for(int i=0; i<width*height; i++){
        tiles[i]=0;
    }
}
