#define M_PI 3.14159265358979323846

#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "map.h"


/////////////////
////Map//////
/////////////////

/*--------------------------------------------------------------
                    AFFICHAGE DE LA MAP
 --------------------------------------------------------------*/

void DessinMap(void) {
    //map
    glLoadIdentity();
    glClearColor (0.0, 0.0, 0.0, 0.0);
    
    glEnable(GL_TEXTURE_2D);
    glColor3ub(255, 255, 255);
    glBindTexture(GL_TEXTURE_2D, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0);
    glVertex2f(1000, 730);
    glTexCoord2f(0, 0);
    glVertex2f(0, 730);
    glTexCoord2f(0, 1);
    glVertex2f(0,0);
    glTexCoord2f(1, 1);
    glVertex2f(1000, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}
