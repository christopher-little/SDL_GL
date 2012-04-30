#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL/SDL_opengl.h>


class Texture{
public:
    Texture(char *filename = NULL);

    // Load texture at the given directory/filename
    void loadTex(char *filename);


    GLuint id; // OpenGL texture reference ID

    unsigned int w, h; // Width and height of the texture in pixels
};



class UVSection{
public:
    float umin,umax;
    float vmin,vmax;
};


#endif // TEXTURE_H
