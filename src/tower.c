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

#include "monster.h"
#include "tower.h"

////////////////
////TOWER///////
////////////////

/*--------------------------------------------------------------
                    AJOUT D'UNE TOWER
 --------------------------------------------------------------*/

l_tower ajouterTower(l_tower liste, int* coins, int x, int y,int type, char** message){
    int error = 0;
    int ray;
    int puissance;
    float cadence;
    int cost;
    float timeConst;

    //Le son de la construction d'une tower
    Mix_Chunk *towerSound;
    towerSound = Mix_LoadWAV("son/Notification.ogg");
    Mix_VolumeChunk(towerSound, MIX_MAX_VOLUME/2);

    //Le son d'une erreur lors de la construction
    Mix_Chunk *errorTowerSound;
    errorTowerSound = Mix_LoadWAV("son/PP Zero.ogg");
    Mix_VolumeChunk(errorTowerSound, MIX_MAX_VOLUME/2);

    //en fonction du type de tower
    switch(type) {
        case 1: //rocket
            ray = 200;
            puissance = 700;
            cadence = 0.9;
            cost = 15;
            timeConst = 2.f;
            break;
        case 2: //laser
            ray = 80;
            puissance = 100;
            cadence = 0.1;
            cost = 20;
            timeConst = 10.f;
            break;
        case 3: //mitrallette
            ray = 100;
            puissance = 70;
            cadence = 0.2;
            cost = 30;
            timeConst = 1.f;
            break;
        case 4: //hybride
            ray = 200;
            puissance = 200;
            cadence = 0.5;
            cost = 10;
            timeConst = 2.f;
            break;
        default:
            break;
    }


    // test de superposition de tower // n'est plus util car on dessine la zone de non construiction (donc la couleur change)

    tower *tmpTower = liste;
    while(tmpTower != NULL){
        if(fabs(x-tmpTower->x)<=30 && fabs(y-tmpTower->y)<=30){
            Mix_PlayChannel(2, errorTowerSound, 0);
            error = 1;
            *message = "On ne peut construire sur une tour";
        }
        tmpTower = tmpTower->next;
    }
    if(*coins-cost < 0){
        *message = "Manque de coins !!!";
        Mix_PlayChannel(2, errorTowerSound, 0);
        error = 1;
    }
    if(error == 0){
        //lancement du bruitage
        Mix_PlayChannel(2, towerSound, 0);

        tower* nouvelElement = (tower*)(malloc(sizeof(tower)));
        if(nouvelElement == NULL){
            fprintf(stderr, "Erreur allocation : TOWER\n");
            exit(EXIT_FAILURE);
        }
        nouvelElement->x = x;
        nouvelElement->y = y;
        nouvelElement->rayon = ray;
        nouvelElement->cost = cost;
        nouvelElement->type = type;
        nouvelElement->puissance = puissance; //puissance ( cout infligé )
        nouvelElement->cadence = cadence; //intervalle de temps entre deux tirs
        nouvelElement->active = 0; //1 quand elel a fini d etre construite
        nouvelElement->timeCrea = (float)SDL_GetTicks(); //1 quand elel a fini d etre construite
        nouvelElement->timeConst = timeConst; //(sec) temps de construction pour cette tower
        nouvelElement->timeDernierTir = 0.0;
        // on paie
        *coins -=cost;
        nouvelElement->next = NULL;
        if(liste == NULL){
            *message = "NEW TOWER";
            return nouvelElement;
        }
        else{
            tower* temp=liste;
            while(temp->next != NULL){
                temp = temp->next;
            }
            temp->next = nouvelElement;
            *message = "TOWER ADDED";
            return liste;
        }
    }else{
        return liste;
    }
    Mix_FreeChunk(towerSound);//Libération du son de la tower
    Mix_FreeChunk(errorTowerSound);//Libération du son de la tower
}

/*--------------------------------------------------------------
                GESTION DU GLISSÉ DÉPOSÉ
 --------------------------------------------------------------*/

void glisserTower(int x, int y, int type){
    int ray = 0;
    switch(type) {
        case 1:
            ray = 200;
            break;
        case 2:
            ray = 80;
            break;
        case 3:
            ray = 100;
            break;
        case 4:
            ray = 200;
            break;
        default:
            break;
    }
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(x,y,1);
    //cercle d action
    glColor3ub(220, 250, 255);
        int i;
        glBegin(GL_LINE_LOOP);
        for(i=0; i<30; i++){
            //angle = 2*M_PI*i/30;
            glVertex2f( cos(2*M_PI*i/30)*(ray), sin(2*M_PI*i/30)*(ray));
        }
        glEnd();

    //zone de construction impossible
    glColor4f(0.0, 0.0, 0.0, 0.1); //color + alpha
    glBegin(GL_POLYGON);
    for(i=0; i<30; i++){
        //angle = 2*M_PI*i/30;
        glVertex2f( cos(2*M_PI*i/30)*(30), sin(2*M_PI*i/30)*(30));
    }
    glEnd();

    //tower
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 4);
    glColor4f(1, 1, 1, 0.5);
    glBegin(GL_QUADS);
    //HAUT GAUCHE
    glTexCoord2f(0.25*(type), 0);      glVertex2f(20,20);
    //HAUT DROITE
    glTexCoord2f(0.25*(type-1), 0);   glVertex2f(-20, 20);
    //BAS GAUCHE
    glTexCoord2f(0.25*(type-1), 0.1);         glVertex2f(-20, -20);
    //BAS DROIT
    glTexCoord2f(0.25*(type), 0.1);      glVertex2f(20, -20);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glLoadIdentity();

}

/*--------------------------------------------------------------
            SUPPRESSION DE TOUTE LES TOWERS
 --------------------------------------------------------------*/

l_tower supprimerAllTower(l_tower liste){
    tower* tmp = liste;
    while(tmp!=NULL){
        tmp = tmp->next;
        free(tmp);
    }
    return NULL;
}

/*--------------------------------------------------------------
            GESTION DE LA SELECTION DES TOWERS
 --------------------------------------------------------------*/

l_tower selectTower(l_tower liste, int x, int y, char** message){
    tower *tmpTower = liste;
    while(tmpTower != NULL){
        if(fabs(x-tmpTower->x)<=20 && fabs(y-tmpTower->y)<=20){
	    *message = "Tour selectionnee";
            return tmpTower;
        }
        tmpTower = tmpTower->next;
    }
    return NULL;
}

/*--------------------------------------------------------------
                    SUPPRIME UNE TOWER
 --------------------------------------------------------------*/

l_tower supprimerTower(l_tower liste, l_tower aSuppr){
    /* Liste vide, il n'y a plus rien à supprimer */
    if(liste == NULL)
        return NULL;

    /* Si l'élément en cours de traitement doit être supprimé */
    if(liste == aSuppr)
    {
        /* On le supprime en prenant soin de mémoriser
         l'adresse de l'élément suivant */
        tower* tmp = liste->next;
        free(liste);
        /* L'élément ayant été supprimé, la liste commencera à l'élément suivant
         pointant sur une liste qui ne contient plus aucun élément ayant la valeur recherchée */
        tmp = supprimerTower(tmp, aSuppr);
        return tmp;
    }
    else
    {
        /* Si l'élement en cours de traitement ne doit pas être supprimé,
         alors la liste finale commencera par cet élément et suivra une liste ne contenant
         plus d'élément ayant la valeur recherchée */
        liste->next = supprimerTower(liste->next, aSuppr);
        return liste;
    }
}

/*--------------------------------------------------------------
            AFFICHAGE DE TOWER
 --------------------------------------------------------------*/

void afficherTower(l_tower liste, int ray, l_tower towerSelected){
    tower *tmp = liste;
    while(tmp != NULL){
        if(((float)SDL_GetTicks() - tmp->timeCrea)/1000 > tmp->timeConst){
            tmp->active = 1;
        }
        glMatrixMode(GL_MODELVIEW);
        glTranslatef(tmp->x,tmp->y,1);

        //cercle d action, si on appui sur t
        if(ray == 1){
            glColor3ub(220, 250, 255);
            int i;
            glBegin(GL_LINE_LOOP);
            for(i=0; i<30; i++){
                //angle = 2*M_PI*i/30;
                glVertex2f( cos(2*M_PI*i/30)*(tmp->rayon), sin(2*M_PI*i/30)*(tmp->rayon));
            }
            glEnd();

            //zone de construction impossible
            glColor4f(0.0, 0.0, 0.0, 0.1); //color + alpha
            glBegin(GL_POLYGON);
            for(i=0; i<30; i++){
                //angle = 2*M_PI*i/30;
                glVertex2f( cos(2*M_PI*i/30)*(30), sin(2*M_PI*i/30)*(30));
            }
            glEnd();
        }

        if(tmp == towerSelected){
            int j;
            glColor4f(0.7, 0.0, 0.0, 0.3); //color + alpha
            glBegin(GL_POLYGON);
            for(j=0; j<30; j++){
                glVertex2f( cos(2*M_PI*j/30)*(15), sin(2*M_PI*j/30)*(15));
            }
            glEnd();
        }

        if(tmp->active == 0){
            glBindTexture(GL_TEXTURE_2D, 4);
            glColor4f(1, 1, 1, 0.2);
            glBegin(GL_QUADS);
            //HAUT GAUCHE
            glTexCoord2f(0.25*(tmp->type), 0);      glVertex2f(20,20);
            //HAUT DROITE
            glTexCoord2f(0.25*(tmp->type-1), 0);   glVertex2f(-20, 20);
            //BAS GAUCHE
            glTexCoord2f(0.25*(tmp->type-1), 0.1);         glVertex2f(-20, -20);
            //BAS DROIT
            glTexCoord2f(0.25*(tmp->type), 0.1);      glVertex2f(20, -20);
            glEnd();
        }else{ //tower
            glBindTexture(GL_TEXTURE_2D, 4);
            glColor3ub(255, 255, 255);
            glBegin(GL_QUADS);
            //HAUT GAUCHE
            glTexCoord2f(0.25*(tmp->type), 0);      glVertex2f(20,20);
            //HAUT DROITE
            glTexCoord2f(0.25*(tmp->type-1), 0);   glVertex2f(-20, 20);
            //BAS GAUCHE
            glTexCoord2f(0.25*(tmp->type-1), 0.1);         glVertex2f(-20, -20);
            //BAS DROIT
            glTexCoord2f(0.25*(tmp->type), 0.1);      glVertex2f(20, -20);
            glEnd();

        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glLoadIdentity();
        tmp = tmp->next;
    }

}

/*--------------------------------------------------------------
                GESTION DES TOWERS
 --------------------------------------------------------------*/

void gestionTower(l_tower listeTower, l_monster listeMonster){

    tower *tmpTower = listeTower;
    monster *tmpMonster = listeMonster;
    monster *Cible;
    float distance;
    float distanceCible;
    float resistTower = 1;
    while(tmpTower != NULL){
        tmpMonster = listeMonster;
        if(tmpTower->active==1 && (SDL_GetTicks()/1000.f - tmpTower->timeDernierTir)>tmpTower->cadence){
            tmpTower->timeDernierTir = SDL_GetTicks()/1000.f;
            Cible = tmpMonster;
            while(tmpMonster != NULL){
                if(tmpMonster->go == 1){
                    //gestionde la resistance a chaque type de tourelle
                    switch (tmpTower->type) {
                        case 1:
                            resistTower = tmpMonster->resistTower1;
                            break;
                        case 2:
                            resistTower = tmpMonster->resistTower2;
                            break;
                        case 3:
                            resistTower = tmpMonster->resistTower3;
                            break;
                        case 4:
                            resistTower = tmpMonster->resistTower4;
                            break;
                        default:
                            resistTower = 1;
                            break;
                    }
                    //recherche du monster le plus proche
                    distanceCible = sqrt(((tmpTower->x)-(Cible->x))*((tmpTower->x)-(Cible->x))+((tmpTower->y)-(Cible->y))*((tmpTower->y)-(Cible->y)));
                    distance = sqrt(((tmpTower->x)-(tmpMonster->x))*((tmpTower->x)-(tmpMonster->x))+((tmpTower->y)-(tmpMonster->y))*((tmpTower->y)-(tmpMonster->y)));
                    //si la tower est de type 3 elle tir sur tout le monde en meme temps
                    if(tmpTower->type==3 && distance<tmpTower->rayon){

                        glBegin(GL_LINES);
                        glColor3f(0.0,1.0,0.0);
                        glVertex2f(tmpTower->x,tmpTower->y);
                        glVertex2f(tmpMonster->x,tmpMonster->y);
                        glEnd();
                        glLoadIdentity();
                        tmpMonster->vie=(tmpMonster->vie)-(tmpTower->puissance*resistTower);

                    }

                    if(distance<distanceCible){
                        Cible = tmpMonster;
                        distanceCible=distance;
                    }
                }
                tmpMonster = tmpMonster->next;
            }

            //on lui ote de la vie s il est dans le rayon d action de la tower
            //sauf si la tower est de type 3 car on lui a deja ote de la vie
            if(distanceCible<(tmpTower->rayon) && tmpTower->type!=3){
                Cible->vie=(Cible->vie)-(tmpTower->puissance*resistTower);
            }
            // si on est dans le rayon d action de la tower
            if(distanceCible<tmpTower->rayon && tmpTower->type!=3){
                // on dessin de tir
                glBegin(GL_LINES);
                glColor3f(1.0f,0.0,0.0);
                glVertex2f(tmpTower->x,tmpTower->y);
                glVertex2f(Cible->x,Cible->y);
                glEnd();
                glLoadIdentity();

            }
        }
        tmpTower = tmpTower->next;
    }

}
