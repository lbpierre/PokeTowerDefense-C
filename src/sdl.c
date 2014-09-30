#define M_PI 3.14159265358979323846

#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "sdl.h"

static unsigned int WINDOW_WIDTH = 1000;
static unsigned int WINDOW_HEIGHT = 800;
static unsigned int PIXEL_WIDTH = 1000;
static unsigned int PIXEL_HEIGHT = 800;


static const unsigned int BIT_PER_PIXEL = 32;

/////////////////
//////SDL///////
/////////////////

/*--------------------------------------------------------------
                    FONCTIONS DE LA SDL
 --------------------------------------------------------------*/

void reshape() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0., PIXEL_WIDTH, -0., PIXEL_HEIGHT);
}

void setVideoMode() {
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE | SDL_INIT_TIMER)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        exit(EXIT_FAILURE);
    }
    reshape();
    glClear (GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND); //enable the blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //set the blend function
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapBuffers();
}
