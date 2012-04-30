#include "Texture.h"

#include <iostream>
using namespace std;

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


Texture::Texture(char *filename){
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glDisable(GL_TEXTURE_2D);

    if(filename != NULL)
        loadTex(filename);
}

void Texture::loadTex(char *filename){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id);

    SDL_Surface *texSurface = IMG_Load(filename);
    if(texSurface == NULL){
        cout << "Texture::loadTex() couldn't load image: " << filename << endl;
    }

    w=texSurface->w;
    h=texSurface->h;

    // Determine pixel storage format
    GLenum texFormat = GL_RGBA;
    GLint nColors = texSurface->format->BytesPerPixel;
    if(nColors == 4){
        if(texSurface->format->Rmask == 0x000000ff){
            texFormat = GL_RGBA;
        }
        else{
            texFormat = GL_BGRA;
        }
    }
    else if(nColors == 3){
        if(texSurface->format->Rmask == 0x000000ff){
            texFormat = GL_RGB;
        }
        else{
            texFormat = GL_BGR;
        }
    }
    else{
        //***This isn't a true color image format...
    }

    // Load the SDL surface pixel data into opengl video/texture memory
    glTexImage2D(GL_TEXTURE_2D, 0, nColors, texSurface->w, texSurface->h, 0, texFormat, GL_UNSIGNED_BYTE, texSurface->pixels);

    // Clean up the SDL surface data
    SDL_FreeSurface(texSurface);

    glDisable(GL_TEXTURE_2D);
}
