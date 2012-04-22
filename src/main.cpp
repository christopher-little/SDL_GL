//============================================================================
// Name        : SDL_GL.cpp
// Author      : Chris Little
// Version     :
// Copyright   : Copyright Chris Little 2012
// Description :
//============================================================================

#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
using namespace std;

#include "Tile.h"

// Variables to control game update (tick) rate
const static int MAXIMUM_TICK_RATE = 120; // The frequency of game state updates
const static int MINIMUM_FRAME_RATE = 15; // Constraint on number of updates per frame (if frame-rate gets too slow, don't update the game state as frequently)
const static double UPDATE_INTERVAL = 1000.0/MAXIMUM_TICK_RATE; // Time (in Seconds) that passes between game state updates
const static double MAX_CYCLES_PER_FRAME = MAXIMUM_TICK_RATE/MINIMUM_FRAME_RATE; // The maximum number of game state updates per frame before update frequency must slow


// SDL display and input controls
SDL_Surface *screen = NULL;


// OpenGL drawing control values
static GLuint listIndex; // Index of the display list I'm using

static GLuint tileListIndex; // Display list for a tile sprite
static GLuint tileTexID; // Texture index identifier for the texture I'll be rendering

static GLfloat mouseX = 0;
static GLfloat mouseY = 0;

static GLfloat squareX = 0.0f; // Position
static GLfloat squareY = 100.0f;
static GLfloat squareV = 50.0f; // Velocity (pixels per second)

static GLfloat moveSpeed = 100.0f; // Defined keyboard movement speed
static GLfloat moveVel = 0; // Current movement speed based on keyboard input state
static GLfloat playerPos = 15.0f; // Current player position




// Identifier for each type of tile
enum TileType {
    air,
    grass,
    dirt
};

const static int tileTypesListSize=64;
static Tile tileTypesList[tileTypesListSize]; // Array of tile type objects

// Width and height of the tilesheet image in pixels
const static int tilesheet_w = 512;
const static int tilesheet_h = 512;
// Width and height of each tile in pixels within the tilesheet image
const static int tilesheet_icon_w = 64;
const static int tilesheet_icon_h = 64;

const static GLuint tileSize = 32; // Width and height of a displayed tile in pixels




// Bezier curve control points
GLfloat controlPoints[4][3]={
    {100.0f, 100.0f, 0},
    {500.0f, 100.0f, 0},
    {-100.0f, 400.0f, 0},
    {200.0f, 300.0f, 0}
};



//==============================================================================
// State update and graphics redraw declarations
//==============================================================================

// Update the game state
void tick();

// Initialize and generate a GL texture; parameters are set to alpha blend overlaying textures. Returns a texture ID
GLuint generateTileTexture();

// Load an SDL surface and store it as an opengl texture referenced by texID (NOTE, must generate a opengl texture before loading the surface)
void loadSDLSurfaceToTexture(GLuint texID);

// Draw a tile at the given grid location with a given GL texture ID (tileset texture) and tile type (NOTE: assumes the tilesheet texture has already been enabled/bound)
void drawTile(int grid_x, int grid_y, TileType type);

// Redraws the screen given the amount of time since the last frame
void redraw();

// Creates an SDL display surface with an OpenGL rendering context
int gfxInit();





void tick(){
    squareX += squareV*(UPDATE_INTERVAL/1000.0);

    playerPos += moveVel*(UPDATE_INTERVAL/1000.0);
}




GLuint generateTileTexture(){
    GLuint texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Pixel colour is determined directly from texture (no lighting/modulation)
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // Pixels are read in byte-order
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear interpolation of 4 nearest texels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return texID;
}




void loadSDLSurfaceToTexture(GLuint texID){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);

    GLenum texFormat;
    GLint nColors;

    SDL_Surface *texSurface = IMG_Load("assets/tiles.png");
    if(texSurface == NULL){
        cout << "Couldn't load image" << endl;
    }

    nColors = texSurface->format->BytesPerPixel;
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
        // This isn't a true color image format...
    }

    // Load the SDL surface pixel data into opengl video/texture memory
    glTexImage2D(GL_TEXTURE_2D, 0, nColors, texSurface->w, texSurface->h, 0, texFormat, GL_UNSIGNED_BYTE, texSurface->pixels);

    // Clean up the SDL surface data
    SDL_FreeSurface(texSurface);

    glDisable(GL_TEXTURE_2D);
}




void drawTile(int grid_x, int grid_y, TileType type){
    Tile tileType = tileTypesList[type];

    glPushMatrix();
    glTranslated(grid_x*tileSize, grid_y*tileSize, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(tileType.u,tileType.v);
        glVertex2f(0.0,0.0);
        glTexCoord2f(tileType.u+tileType.d_u,tileType.v);
        glVertex2f(tileSize, 0.0);
        glTexCoord2f(tileType.u+tileType.d_u,tileType.v+tileType.d_v);
        glVertex2f(tileSize, tileSize);
        glTexCoord2f(tileType.u,tileType.v+tileType.d_v);
        glVertex2f(0.0, tileSize);

        /*
        glTexCoord2f(0.0,0.0);
        glVertex2f(0.0,0.0);
        glTexCoord2f(64.0/512.0,0.0);
        glVertex2f(tileSize, 0.0);
        glTexCoord2f(64.0/512.0,64.0/512.0);
        glVertex2f(tileSize, tileSize);
        glTexCoord2f(0.0,64.0/512.0);
        glVertex2f(0.0, tileSize);
        */
    glEnd();
    glPopMatrix();
}



void redraw(){
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a triangle
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0,1.0f);
        glVertex2f(300.0f, 400.0f);

        glVertex2f(450.0f, 400.0f);

        glVertex2f(375.0f, 500.0f);
    glEnd();


    // Draw the square at its new position
    glPushMatrix();
    glTranslatef(squareX, squareY, 0);
    glBegin(GL_QUADS);
        glColor3f(0.0f,1.0f,1.0f);
        glVertex2f(0.0f, 0.0f);

        glVertex2f(150.0f, 0.0f);

        glVertex2f(150.0f, 150.0f);

        glVertex2f(0.0f, 150.0f);
    glEnd();
    glPopMatrix();

    // Draw a square centered at the mouse pointer
    glPushMatrix();
    glTranslatef(mouseX-25.0f,mouseY-25.0f, 0);
    glBegin(GL_QUADS);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex2f(0, 0);

        glVertex2f(0, 50.0f);

        glVertex2f(50.0f, 50.0f);

        glVertex2f(50.0f, 0);
    glEnd();
    glPopMatrix();


    // Draw the display list in a couple different colours and locations
    glColor3f(0,0,1.0f);
    glCallList(listIndex);
    glColor3f(0,1.0f,1.0f);
    glPushMatrix();
    glTranslatef(150.0f, 150.0f, 0.0f);
    glCallList(listIndex);
    glPopMatrix();


    // Draw a bezier curve
    glBegin(GL_LINE_STRIP);
        glColor3f(1.0f,0,0);
        float t=0.0f;
        float t_=1.0f;
        for(int i=0; i<30; i++){
            glVertex3f(t_*t_*t_*controlPoints[0][0] + 3*t*t_*t_*controlPoints[1][0] + 3*t*t*t_*controlPoints[2][0] + t*t*t*controlPoints[3][0],
                t_*t_*t_*controlPoints[0][1] + 3*t*t_*t_*controlPoints[1][1] + 3*t*t*t_*controlPoints[2][1] + t*t*t*controlPoints[3][1],
                t_*t_*t_*controlPoints[0][2] + 3*t*t_*t_*controlPoints[1][2] + 3*t*t*t_*controlPoints[2][2] + t*t*t*controlPoints[3][2]);

            t += 1.0f/30;
            t_ = 1-t;
        }
    glEnd();



    glColor4f(1.0,1.0,1.0,1.0);
    // Draw the textured tiles
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tileTexID);

    for(unsigned int i=0; i<640/tileSize; i++){
        glPushMatrix();
        glTranslated(i*tileSize, 416.0, 0);
        glCallList(tileListIndex);
        glPopMatrix();
    }
    for(unsigned int i=0; i<640/tileSize; i++){
        for(unsigned int j=0; j<2; j++){
            glPushMatrix();
            glTranslated(i*tileSize, 448.0 + j*tileSize, 0);
            glCallList(tileListIndex);
            glPopMatrix();
        }
    }

    for(unsigned int i=0; i<480/tileSize; i++){
        glPushMatrix();
        glTranslated(576.0, i*tileSize, 0);
        glCallList(tileListIndex);
        glPopMatrix();
    }


    //***Draw a tile given a type parameter!
    drawTile(10,3, dirt);


    glDisable(GL_TEXTURE_2D);



    // Draw a place holder for the player
    glPushMatrix();
    glTranslatef(playerPos, 300.0f, 0);
    glBegin(GL_QUADS);
        glColor3f(0.5f,0.1f,0.1f);
        glVertex2f(0.0f, 0.0f);

        glVertex2f(32.0f, 0.0f);

        glVertex2f(32.0f, 32.0f);

        glVertex2f(0.0f, 32.0f);
    glEnd();
    glPopMatrix();
}



int gfxInit(){
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
        cout << "Couldn't initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

    screen = SDL_SetVideoMode(640,480,32,SDL_OPENGL);
    if(screen == NULL){
        cout << "Couldn't create video screen surface: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


    // Use an orthographic projection such that origin (0,0) is top-left
    glClearColor(1.0f, 1.0f, 0, 0);
    glViewport(0,0,640,480);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 640, 480, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    return 0;
}





#undef main// This is bizarre, but gets rid of that WinMain/qMain linker error nonsense

int main(int argc, char **argv) {

    if(gfxInit() > 0){
        cout << "gfxInit() failed!" << endl;
        return 1;
    }



    // Create a display list
    listIndex = glGenLists(1);
    if(listIndex == 0){
        cout << "List failed" << endl;
        return 1;
    }

    glNewList(listIndex, GL_COMPILE);
        glBegin(GL_TRIANGLE_STRIP);
            glVertex2f(50.0f, 0.0f); // A
            glVertex2f(100.0f, 15.0f); // B
            glVertex2f(0.0f, 100.0f); // C

            glVertex2f(140.0f, 70.0f); // D

            glVertex2f(120.0f, 120.0f); // E

            glVertex2f(200.0f, 110.0f); // F

            glVertex2f(150.0f, 150.0f); // G
        glEnd();
    glEndList();



    // Create a display list for the tile
    tileListIndex = glGenLists(1);
    if(listIndex == 0){
        cout << "List failed" << endl;
        return 1;
    }

    glNewList(tileListIndex, GL_COMPILE);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex2f(0.0,0.0);
        glTexCoord2f((1.0/8.0),0.0);
        glVertex2f(tileSize, 0.0);
        glTexCoord2f((1.0/8.0),(1.0/8.0));
        glVertex2f(tileSize, tileSize);
        glTexCoord2f(0.0,(1.0/8.0));
        glVertex2f(0.0, tileSize);
    glEnd();
    glEndList();



    // Lets try texture mapping something...
    glEnable(GL_TEXTURE_2D);

    // Generate a transparent tile for "air"
    GLuint airTex[16*16*4];
    for(int x=0; x<16*16; x++){
        airTex[x*4] = 0;
        airTex[x*4 +1] = 0;
        airTex[x*4 +2] = 0;
        airTex[x*4 +3] = 0;
    }
    // Generate the opengl texture ID for 'air'
    GLuint airTexID = 0;
    airTexID = generateTileTexture();
    // Copy the transparent array to video memory
    glTexImage2D(GL_TEXTURE_2D, 0, 4, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, airTex);
    // Assign the texture data to the air tile type
    tileTypesList[air].texID = airTexID;
    tileTypesList[air].u = 0.0;
    tileTypesList[air].v = 0.0;
    tileTypesList[air].d_u = 1.0;
    tileTypesList[air].d_v = 1.0;


    // Load the tilesheet texture
    tileTexID = generateTileTexture();

    loadSDLSurfaceToTexture(tileTexID);

    // Determine tile width/height in tilesheet texture coordinates
    double u_width = (double)tilesheet_icon_w/(double)tilesheet_w;
    double v_height = (double)tilesheet_icon_h/(double)tilesheet_h;
    // Assign grass tile properties
    tileTypesList[grass].texID = tileTexID;
    tileTypesList[grass].u = 0.0;
    tileTypesList[grass].v = 0.0;
    tileTypesList[grass].d_u = u_width;
    tileTypesList[grass].d_v = v_height;
    // Assign dirt tile properties
    tileTypesList[dirt].texID = tileTexID;
    tileTypesList[dirt].u = u_width;
    tileTypesList[dirt].v = 0.0;
    tileTypesList[dirt].d_u = u_width;
    tileTypesList[dirt].d_v = v_height;


    // Disable texturing for now until it's needed again (if left enabled, any shapes drawn use texture data instead)
    glDisable(GL_TEXTURE_2D);




    // Draw the scene to start off
    redraw();

    SDL_GL_SwapBuffers();




    // Start the game loop
    bool quit=false;
    SDL_Event event;
    double lastFrameTime = 0.0;
    double cyclesLeftOver = 0.0;
    double currentTime;
    double updateIterations; // Actually an interval (rather than number of game state updates)

    while(quit == false){
        currentTime = SDL_GetTicks();
        updateIterations = ((currentTime-lastFrameTime) + cyclesLeftOver);

        // Cap the number of iterations if the time between frames is getting too large
        if(updateIterations > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)){ //... this is actually 1.0/MINIMUM_TICK_RATE, or, maximum number of seconds per update?
            updateIterations = MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL;
        }

        // If the time since the last frame redraw is small, don't update the game, otherwise update the game state one or more times
        while(updateIterations > UPDATE_INTERVAL){
            updateIterations -= UPDATE_INTERVAL;

            // Check event queue, collect user input
            if(SDL_PollEvent(&event)){
                if(event.type == SDL_MOUSEBUTTONDOWN){
                    mouseX = event.button.x;
                    mouseY = event.button.y;
                }
                else if(event.type == SDL_KEYDOWN){
                    if(event.key.keysym.sym == SDLK_RIGHT){
                        moveVel += moveSpeed;
                    }
                    else if(event.key.keysym.sym == SDLK_LEFT){
                        moveVel -= moveSpeed;
                    }
                }
                else if(event.type == SDL_KEYUP){
                    if(event.key.keysym.sym == SDLK_RIGHT){
                        moveVel -= moveSpeed;
                    }
                    else if(event.key.keysym.sym == SDLK_LEFT){
                        moveVel += moveSpeed;
                    }
                }
                else if(event.type == SDL_QUIT)
                    quit = true;
            }

            // Update the game state
            tick();
        }

        cyclesLeftOver = updateIterations;
        lastFrameTime = SDL_GetTicks();

        redraw();
        SDL_GL_SwapBuffers();
    }


    // Clean up and quit
    SDL_Quit();
    return 0;
}
