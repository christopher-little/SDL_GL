//============================================================================
// Name        : SDL_GL.cpp
// Author      : Chris Little
// Version     :
// Copyright   : Copyright Chris Little 2012
// Description : 
//============================================================================

#include <iostream>
#include <ctime>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
using namespace std;


// Bezier curve control points
GLfloat controlPoints[4][3]={
	{100.0f, 100.0f, 0},
	{500.0f, 100.0f, 0},
	{-100.0f, 400.0f, 0},
	{200.0f, 300.0f, 0}
};


void drawSquare(GLfloat x, GLfloat y, GLfloat w=50.0f, GLfloat h=50.0f){
	GLfloat _w = w/2;
	GLfloat _h = h/2;
	glBegin(GL_QUADS);
		glColor3f(1.0f,0.5f,0);
		glVertex3f(x-_w, y-_h, 0);

		glVertex3f(x+_w, y-_h, 0);

		glVertex3f(x+_w, y+_h, 0);

		glVertex3f(x-_w, y+_h, 0);
	glEnd();
}


int main(int argc, char **argv) {
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
		cout << "Couldn't initialize SDL: " << SDL_GetError() << endl;
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Surface *screen = SDL_SetVideoMode(640,480,32,SDL_OPENGL);
	if(screen == NULL){
		cout << "Couldn't create video screen surface: " << SDL_GetError() << endl;
		return 1;
	}

	//SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF,0xFF,0));
	//SDL_Flip(screen);

	// Create an orthographic projection such that origin (0,0) is top-left
	glClearColor(1.0f, 1.0f, 0, 0);
	glViewport(0,0,640,480);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable texture mapping
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);
		glColor3f(1.0f,0,1.0f);
		glVertex3f(300.0f, 300.0f, 0);

		glVertex3f(450.0f, 300.0f, 0);

		glVertex3f(375.0f, 400.0f, 0);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.0f,1.0f,1.0f);
		glVertex3f(300.0f, 100.0f, 0);

		glVertex3f(450.0f, 100.0f, 0);

		glVertex3f(450.0f, 250.0f, 0);

		glVertex3f(300.0f, 250.0f, 0);
	glEnd();

	drawSquare(320,240);


	// Create a display list
	GLuint listIndex = glGenLists(1);
	if(listIndex == 0){
		cout << "List failed" << endl;
		return 1;
	}

	glNewList(listIndex, GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);
		/*
		glBegin(GL_TRIANGLES);
			glVertex2f(50.0f, 0.0f);
			glVertex2f(0.0f, 100.0f);
			glVertex2f(60.0f, 10.0f);

			glVertex2f(60.0f, 10.0f);
			glVertex2f(0.0f, 100.0f);
			glVertex2f(80.0f, 50.0f);

			glVertex2f(80.0f, 50.0f);
			glVertex2f(0.0f, 100.0f);
			glVertex2f(60.0f, 90.0f);

			glVertex2f(80.0f, 50.0f);
			glVertex2f(60.0f, 90.0f);
			glVertex2f(90.0f, 110.0f);

			glVertex2f(80.0f, 50.0f);
			glVertex2f(90.0f, 110.0f);
			glVertex2f(120.0f, 90.0f);
		*/
			glVertex2f(50.0f, 0.0f); // A
			glVertex2f(100.0f, 15.0f); // B
			glVertex2f(0.0f, 100.0f); // C

			glVertex2f(140.0f, 70.0f); // D

			glVertex2f(120.0f, 120.0f); // E

			glVertex2f(200.0f, 110.0f); // F

			glVertex2f(150.0f, 150.0f); // G
		glEnd();
	glEndList();

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



	// Lets try texture mapping something...
	srand(time(NULL));
	int texWidth = 64;
	int texHeight = 64;
	// Generate random texture data
	GLubyte imageData[texWidth*texHeight*3];
	for(int i=0; i<texWidth*texHeight*3; i++){
		imageData[i] = GLubyte(rand() % 256);
	}

	GLint texID = 13;
	glBindTexture(GL_TEXTURE_2D, texID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex2f(250.0f,400.0f);
		glTexCoord2f(1.0,0.0);
		glVertex2f(314.0f, 400.0f);
		glTexCoord2f(1.0,1.0);
		glVertex2f(314.0f, 464.0f);
		glTexCoord2f(0.0,1.0);
		glVertex2f(250.0f, 464.0f);
	glEnd();



	SDL_GL_SwapBuffers();



	bool quit=false;
	SDL_Event event;

	while(quit == false){
		if(SDL_PollEvent(&event)){
			if(event.type == SDL_MOUSEBUTTONDOWN){
				glClear(GL_COLOR_BUFFER_BIT);

				drawSquare(event.button.x, event.button.y);

				SDL_GL_SwapBuffers();
			}
			else if(event.type == SDL_QUIT)
				quit = true;
		}
	}

	SDL_Quit();
	return 0;
}
