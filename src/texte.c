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

#include "monster.h"
#include "tower.h"
#include "texture.h"
#include "texte.h"

////////////////
////TEXTE///////
////////////////

/*--------------------------------------------------------------
        AFFICHAGE DE LA DESCRIPTION DES TOURS DU MENU
 ---------------------------------------------------------------*/

GLuint* description(GLuint *textureId, int type){
    int ray = 0;
    int puissance = 0;
    int cadence = 0.0;
    int cost = 0;
    int timeConst;
    //en fonction du type de tower
    switch(type) {
        case 1:
            ray = 200;
            puissance = 700;
            cadence = 9;
            cost = 15;
            timeConst = 2;
            break;
        case 2:
            ray = 80;
            puissance = 100;
            cadence = 1;
            cost = 20;
            timeConst = 10;
            break;
        case 3:
            ray = 100;
            puissance = 70;
            cadence = 2;
            cost = 30;
            timeConst = 1;
            break;
        case 4:
            ray = 200;
            puissance = 200;
            cadence = 5;
            cost = 10;
            timeConst = 2;
            break;
        default:
            break;
    }
    
    char valeurRay[25] = "Rayon : ";
    char valeurPui[25] = "Puissance : ";
    char valeurCad[25] = "Cadence : 0.";
    char valeurCou[25] = "Cout : ";
    char valeurCons[30] = "Construction : ";
    sprintf(valeurRay+8, "%d", ray);
    sprintf(valeurPui+12, "%d", puissance);
    sprintf(valeurCad+12, "%d", cadence);
    sprintf(valeurCou+7, "%d", cost);
    sprintf(valeurCons+15, "%d", timeConst);
    //printf("%s", valeurRay);
    //printf("%s", valeurCons);

    //strcat(valeur," Cout : ");
    
    TTF_Font *police = NULL;
    SDL_Surface *texte = NULL;
    SDL_Color couleurBlanche = {255, 255, 255};
    /* Chargement de la police */
    police = TTF_OpenFont("images/hoog0656.ttf", 12);
    
    // Bind the texture object
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, textureId[10]);
    // Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glLoadIdentity();
    glTranslatef(850,660,1);
    //////////////
    /////TYPE////
    //////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    switch(type) {
        case 1:
            texte = TTF_RenderText_Blended(police, "Rocket", couleurBlanche);
            break;
        case 2:
            texte = TTF_RenderText_Blended(police, "Laser", couleurBlanche);
            break;
        case 3:
            texte = TTF_RenderText_Blended(police, "Mitraillette", couleurBlanche);
            break;
        case 4:
            texte = TTF_RenderText_Blended(police, "Hybride", couleurBlanche);
            break;
        default:
            texte = TTF_RenderText_Blended(police, "TYPE", couleurBlanche);
            break;
    }

    
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(0+texte->w, 100+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 100+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,100);
    glTexCoord2f(1, 1);
    glVertex2f(0+texte->w, 100);
    glEnd();
    
    //////////////
    /////RAYON////
    //////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeurRay, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(0+texte->w, 80+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 80+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,80);
    glTexCoord2f(1, 1);
    glVertex2f(0+texte->w, 80);
    glEnd();
    
    //////////////////
    /////PUISSANCE////
    //////////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeurPui, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(texte->w, 60+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 60+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,60);
    glTexCoord2f(1, 1);
    glVertex2f(0+texte->w, 60);
    glEnd();
    
    ////////////////
    /////CADENCE////
    ////////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeurCad, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(texte->w, 40+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 40+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,40);
    glTexCoord2f(1, 1);
    glVertex2f(texte->w, 40);
    glEnd();
    
    /////////////
    /////Cout////
    /////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeurCou, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(texte->w, 20+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 20+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,20);
    glTexCoord2f(1, 1);
    glVertex2f(texte->w, 20);
    glEnd();
    
    /////////////
    /////Construction////
    /////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeurCons, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(texte->w, texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,0);
    glTexCoord2f(1, 1);
    glVertex2f(texte->w, 0);
    glEnd();
    
    
    
    
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable(GL_TEXTURE_2D);
    TTF_CloseFont(police);
    SDL_FreeSurface(texte);
    
       return textureId;
}

/*--------------------------------------------------------------
        DESCRIPTION DE LA TOUR SELECTIONNÉ SUR LA MAP
 ---------------------------------------------------------------*/

GLuint* descriptionTowerSelected(GLuint *textureId, l_tower towerSelected){
    char valeurRay[25] = "Rayon : ";
    char valeurPui[25] = "Puissance : ";
    char valeurCad[25] = "Cadence : 0.";
    char valeurCou[25] = "Cout : ";
    char valeurCons[30] = "Construction : ";
    sprintf(valeurRay+8, "%d", towerSelected->rayon);
    sprintf(valeurPui+12, "%d", towerSelected->puissance);
    sprintf(valeurCad+12, "%d", (int)(towerSelected->cadence*10));
    sprintf(valeurCou+7, "%d", towerSelected->cost);
    sprintf(valeurCons+15, "%d", (int)towerSelected->timeConst);
    //printf("%s", valeurRay);
    //printf("%s", valeurCons);
    
    //strcat(valeur," Cout : ");
    
    TTF_Font *police = NULL;
    SDL_Surface *texte = NULL;
    SDL_Color couleurBlanche = {255, 255, 255};
    SDL_Color couleurRouge = {0, 0, 255};
    SDL_Color couleurVerte = {0, 250, 0};
    /* Chargement de la police */
    police = TTF_OpenFont("images/hoog0656.ttf", 12);
    
    // Bind the texture object
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, textureId[10]);
    // Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glLoadIdentity();
    glTranslatef(850,660,1);
    //////////////
    /////TYPE////
    //////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    switch(towerSelected->type) {
        case 1:
            texte = TTF_RenderText_Blended(police, "Rocket", couleurBlanche);
            break;
        case 2:
            texte = TTF_RenderText_Blended(police, "Laser", couleurBlanche);
            break;
        case 3:
            texte = TTF_RenderText_Blended(police, "Mitraillette", couleurBlanche);
            break;
        case 4:
            texte = TTF_RenderText_Blended(police, "Hybride", couleurBlanche);
            break;
        default:
            texte = TTF_RenderText_Blended(police, "TYPE", couleurBlanche);
            break;
    }
    
    
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(0+texte->w, 100+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 100+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,100);
    glTexCoord2f(1, 1);
    glVertex2f(0+texte->w, 100);
    glEnd();
    

    //////////////
    /////RAYON////
    //////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeurRay, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(0+texte->w, 80+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 80+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,80);
    glTexCoord2f(1, 1);
    glVertex2f(0+texte->w, 80);
    glEnd();
    
    //////////////////
    /////PUISSANCE////
    //////////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeurPui, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(texte->w, 60+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 60+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,60);
    glTexCoord2f(1, 1);
    glVertex2f(0+texte->w, 60);
    glEnd();
    
    ////////////////
    /////CADENCE////
    ////////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeurCad, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(texte->w, 40+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 40+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,40);
    glTexCoord2f(1, 1);
    glVertex2f(texte->w, 40);
    glEnd();
    
    /////////////
    /////Cout////
    /////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeurCou, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(texte->w, 20+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 20+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,20);
    glTexCoord2f(1, 1);
    glVertex2f(texte->w, 20);
    glEnd();
    
    /////////////
    /////Construction////
    /////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeurCons, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(texte->w, texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,0);
    glTexCoord2f(1, 1);
    glVertex2f(texte->w, 0);
    glEnd();

    /////////////
    /////VENDRE////
    /////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, "Vendre", couleurRouge);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(texte->w, texte->h-15);
    glTexCoord2f(0, 0);
    glVertex2f(0, texte->h-15);
    glTexCoord2f(0, 1);
    glVertex2f(0,-15);
    glTexCoord2f(1, 1);
    glVertex2f(texte->w, -15);
    glEnd();
    
    /////////////
    /////EVOLUER////
    /////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, "Evoluer", couleurVerte);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(texte->w+65, texte->h-15);
    glTexCoord2f(0, 0);
    glVertex2f(65, texte->h-15);
    glTexCoord2f(0, 1);
    glVertex2f(65,-15);
    glTexCoord2f(1, 1);
    glVertex2f(texte->w+65, -15);
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable(GL_TEXTURE_2D);
    TTF_CloseFont(police);
    SDL_FreeSurface(texte);
    
    return textureId;
}

/*--------------------------------------------------------------
                    AFFICHAGE DE L'ARGENT
 ---------------------------------------------------------------*/

GLuint* afficheCoins(GLuint *textureId, int coins){
       
    char valeur[25] = "Coins : ";
    sprintf(valeur+8, "%d", coins);
    
    TTF_Font *police = NULL;
    SDL_Surface *texte = NULL;
    SDL_Color couleurBlanche = {255, 255, 255};
    /* Chargement de la police */
    police = TTF_OpenFont("images/hoog0656.ttf", 20);
    
    // Bind the texture object
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, textureId[10]);
    // Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glLoadIdentity();
    glTranslatef(500,680,1);
    //////////////
    /////COINS////
    //////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeur, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(0+texte->w, 80+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 80+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,80);
    glTexCoord2f(1, 1);
    glVertex2f(0+texte->w, 80);
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable(GL_TEXTURE_2D);
    TTF_CloseFont(police);
    SDL_FreeSurface(texte);
    
    return textureId;
}

/*--------------------------------------------------------------
                    AFFICHAGE DES VAGUES
 ---------------------------------------------------------------*/

GLuint* afficheVagues(GLuint *textureId, int vague){
    
    char valeur[25] = "Vague : ";
    sprintf(valeur+8, "%d", vague);
    
    TTF_Font *police = NULL;
    SDL_Surface *texte = NULL;
    SDL_Color couleurBlanche = {255, 255, 255};
    /* Chargement de la police */
    police = TTF_OpenFont("images/hoog0656.ttf", 20);
    
    // Bind the texture object
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, textureId[10]);
    // Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glLoadIdentity();
    glTranslatef(350,680,1);
    //////////////
    /////VAGUE////
    //////////////
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, valeur, couleurBlanche);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glColor3ub(255,255,255);
    glTexCoord2f(1, 0);
    glVertex2f(0+texte->w, 80+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 80+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,80);
    glTexCoord2f(1, 1);
    glVertex2f(0+texte->w, 80);
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable(GL_TEXTURE_2D);
    TTF_CloseFont(police);
    SDL_FreeSurface(texte);
    
    return textureId;
}


/*--------------------------------------------------------------
                    AFFICHAGE DU TEXTE
 ---------------------------------------------------------------*/

GLuint* afficheTexte(GLuint *textureId, int x, int y, char * Texte, int r, int g, int b, int size){
    TTF_Font *police = NULL;
    SDL_Surface *texte = NULL;
    SDL_Color couleur = {r, g, b};
    /* Chargement de la police */
    police = TTF_OpenFont("images/hoog0656.ttf", size);
    
    // Bind the texture object
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, textureId[10]);
    // Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glLoadIdentity();
    glTranslatef(x,y,1);
    
    /* Écriture du texte dans la SDL_Surface texte en mode Blended (optimal) */
    texte = TTF_RenderText_Blended(police, Texte, couleur);
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texte->w, texte->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texte->pixels );
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0);
    glVertex2f(0+texte->w, 80+texte->h);
    glTexCoord2f(0, 0);
    glVertex2f(0, 80+texte->h);
    glTexCoord2f(0, 1);
    glVertex2f(0,80);
    glTexCoord2f(1, 1);
    glVertex2f(0+texte->w, 80);
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable(GL_TEXTURE_2D);
    TTF_CloseFont(police);
    SDL_FreeSurface(texte);
    
    return textureId;
}

/*--------------------------------------------------------------
            AFFICHAGE DU MESSAGE D'ERREUR
 ---------------------------------------------------------------*/

void afficheMessageErreur(void){
    glLoadIdentity();
    glColor4f(0, 0, 0, 0.5);
    glBegin(GL_QUADS);
    glVertex2f(1000, 715);
    glVertex2f(0, 715);
    glVertex2f(0, 693);
    glVertex2f(1000, 693);
    glEnd();
    glColor4f(1, 1, 1, 1);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

