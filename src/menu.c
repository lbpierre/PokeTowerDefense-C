#define M_PI 3.14159265358979323846

#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include <SDL/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <SDL_image/SDL_image.h>
#include <SDL_mixer/SDL_mixer.h>
#include <SDL_ttf/SDL_ttf.h>

#include "menu.h"
#include "monster.h"
#include "tower.h"
#include "texture.h"
#include "texte.h"



////////////////
//////MENU//////
////////////////

/*--------------------------------------------------------------
                    AFFICHAGE DE LA PAUSE
 --------------------------------------------------------------*/

void afficherMenuPause(int * pause){
    if(*pause == 1){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 6);
        glColor4f(1,1,1,1);
        glLoadIdentity();
        glBegin(GL_QUADS);
        glTexCoord2f(1, 0);
        glVertex2f(1000, 800);
        glTexCoord2f(0, 0);
        glVertex2f(0,800);
        glTexCoord2f(0, 1);
        glVertex2f(0, 0);
        glTexCoord2f(1, 1);
        glVertex2f(1000, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

        }
}

/*--------------------------------------------------------------
                    AFFICHAGE DU MENU D'AIDE
 --------------------------------------------------------------*/

void afficherMenuHelp(GLuint* textureId){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 10);
        glColor4f(1,1,1,1);
        glLoadIdentity();
        glBegin(GL_QUADS);
        glTexCoord2f(1, 0);
        glVertex2f(1000, 300);
        glTexCoord2f(0, 0);
        glVertex2f(0,300);
        glTexCoord2f(0, 1);
        glVertex2f(0, 0);
        glTexCoord2f(1, 1);
        glVertex2f(1000, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        afficheTexte(textureId, 40, 130, "[a] : TOWER Rocket", 100, 100, 100, 20);
        afficheTexte(textureId, 40, 100, "[z] : TOWER Laser", 100, 100, 100, 20);
        afficheTexte(textureId, 40, 70, "[e] : TOWER Mitraillette", 100, 100, 100, 20);
        afficheTexte(textureId, 40, 40, "[r] : TOWER Hybride", 100, 100, 100, 20);
        afficheTexte(textureId, 400, 130, "[t] : Way + Ray ", 100, 100, 100, 20);
        afficheTexte(textureId, 400, 100, "[s] : Vendre ", 100, 100, 100, 20);
        afficheTexte(textureId, 400, 70, "[u] : Evoluer ", 100, 100, 100, 20);
        afficheTexte(textureId, 400, 40, "[q] : EXIT ", 100, 100, 100, 20);
}

/*--------------------------------------------------------------
            AFFICHAGE DE L'IMAGE DE DÉPART DU JEU
 --------------------------------------------------------------*/

void afficherMenuStart(int * pause){
    if(*pause == 1){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 5);
        glColor4f(1,1,1,1);
        glLoadIdentity();
        glBegin(GL_QUADS);
        glTexCoord2f(1, 0);
        glVertex2f(1000, 800);
        glTexCoord2f(0, 0);
        glVertex2f(0,800);
        glTexCoord2f(0, 1);
        glVertex2f(0, 0);
        glTexCoord2f(1, 1);
        glVertex2f(1000, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

    }
}

/*--------------------------------------------------------------
            AFFICHAGE DE L'IMAGE DE VICTOIRE
 --------------------------------------------------------------*/

void afficherMenuWin(int * pause){
    if(*pause == 1){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 7);
        glColor4f(1,1,1,1);
        glLoadIdentity();
        glBegin(GL_QUADS);
        glTexCoord2f(1, 0);
        glVertex2f(1000, 800);
        glTexCoord2f(0, 0);
        glVertex2f(0,800);
        glTexCoord2f(0, 1);
        glVertex2f(0, 0);
        glTexCoord2f(1, 1);
        glVertex2f(1000, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

    }
}

/*--------------------------------------------------------------
            AFFICHAGE DE L'IMAGE DE DÉFAITE
 --------------------------------------------------------------*/

void afficherMenuLose(int * pause){
    if(*pause == 1){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 8);
        glColor4f(1,1,1,1);
        glLoadIdentity();
        glBegin(GL_QUADS);
        glTexCoord2f(1, 0);
        glVertex2f(1000, 800);
        glTexCoord2f(0, 0);
        glVertex2f(0,800);
        glTexCoord2f(0, 1);
        glVertex2f(0, 0);
        glTexCoord2f(1, 1);
        glVertex2f(1000, 0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);

    }
}

/*--------------------------------------------------------------
                    AFFICHAGE DU MENU
 --------------------------------------------------------------*/

void afficherMenu(int selected){

    glEnable(GL_TEXTURE_2D);
    glColor3ub(255,255,255);
    glBindTexture(GL_TEXTURE_2D, 3);
    glLoadIdentity();
    glBegin(GL_QUADS);
        glTexCoord2f(1, 0);
        glVertex2f(1000, 800);
        glTexCoord2f(0, 0);
        glVertex2f(0,800);
        glTexCoord2f(0, 0.99);
        glVertex2f(0, 600);
        glTexCoord2f(1, 0.99);
        glVertex2f(1000, 600);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);

    //tower1
    glTranslatef(727,776,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 4);
    glBegin(GL_QUADS);
    if(selected == 1){
        glColor4f(1.0, 1.0, 1.0, 1.0); //color + alpha

    }else{
        glColor4f(1.0, 1.0, 1.0, 0.5); //color + alpha

    }
    glTexCoord2f(0.25, 0);          glVertex2f(20, 20);
    glTexCoord2f(0, 0);             glVertex2f(-20, 20);
    glTexCoord2f(0, 0.1);           glVertex2f(-20,-20);
    glTexCoord2f(0.25, 0.1);        glVertex2f(20, -20);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glLoadIdentity();

    //tower2
    glTranslatef(761,776,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 4);
    glBegin(GL_QUADS);
    if(selected == 2){
        glColor4f(1.0, 1.0, 1.0, 1.0); //color + alpha

    }else{
        glColor4f(1.0, 1.0, 1.0, 0.5); //color + alpha

    }
    glTexCoord2f(0.50, 0);          glVertex2f(20, 20);
    glTexCoord2f(0.25, 0);             glVertex2f(-20, 20);
    glTexCoord2f(0.25, 0.1);           glVertex2f(-20,-20);
    glTexCoord2f(0.50, 0.1);        glVertex2f(20, -20);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glLoadIdentity();

    //tower3
    glTranslatef(725,743,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 4);
    glBegin(GL_QUADS);
    if(selected == 3){
        glColor4f(1.0, 1.0, 1.0, 1.0); //color + alpha

    }else{
        glColor4f(1.0, 1.0, 1.0, 0.5); //color + alpha

    }
    glTexCoord2f(0.75, 0);          glVertex2f(20, 20);
    glTexCoord2f(0.50, 0);             glVertex2f(-20, 20);
    glTexCoord2f(0.50, 0.1);           glVertex2f(-20,-20);
    glTexCoord2f(0.75, 0.1);        glVertex2f(20, -20);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glLoadIdentity();

    //tower4
    glTranslatef(759,743,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 4);
    glBegin(GL_QUADS);
    if(selected == 4){
        glColor4f(1.0, 1.0, 1.0, 1.0); //color + alpha

    }else{
        glColor4f(1.0, 1.0, 1.0, 0.5); //color + alpha

    }
    glTexCoord2f(1, 0);          glVertex2f(20, 20);
    glTexCoord2f(0.75, 0);             glVertex2f(-20, 20);
    glTexCoord2f(0.75, 0.1);           glVertex2f(-20,-20);
    glTexCoord2f(1, 0.1);        glVertex2f(20, -20);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glLoadIdentity();

}

/*--------------------------------------------------------------
                AFFICHAGE DU CURSEUR
 --------------------------------------------------------------*/

void afficherMouse(int xMove, int yMove, int PIXEL_HEIGHT){
    //affichage du cursor
    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
    glTranslatef(xMove, PIXEL_HEIGHT-yMove,1);
    glColor3ub(255, 255, 255);
    glBindTexture(GL_TEXTURE_2D, 12);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0);
    glVertex2f(17, 0);
    glTexCoord2f(0, 0);
    glVertex2f(-12, 0);
    glTexCoord2f(0, 1);
    glVertex2f(-12,-30);
    glTexCoord2f(1, 1);
    glVertex2f(17, -30);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
