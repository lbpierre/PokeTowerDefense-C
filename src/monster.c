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

#include "vague.h"
#include "node.h"
#include "tower.h"
#include "texture.h"


////////////////
////MONSTRE/////
////////////////

/*--------------------------------------------------------------
                AJOUTE UN MONSTRE
 --------------------------------------------------------------*/

l_monster ajouterMonster(l_monster liste, l_node listeNode, int idMonster, int vie, int typeMonster, float resistTower1,float resistTower2,float resistTower3,float resistTower4){
    monster* nouvelElement = (monster*)(malloc(sizeof(monster)));
    if(nouvelElement == NULL){
        fprintf(stderr, "Erreur allocation : MONSTER\n");
        exit(EXIT_FAILURE);
    }
    nouvelElement->x = listeNode->x;
    nouvelElement->y = listeNode->y;
    nouvelElement->dest = listeNode->next;
    nouvelElement->oldDest = listeNode;
    nouvelElement->vie = vie;
    nouvelElement->idMonster = idMonster;
    nouvelElement->typeMonster = typeMonster;
    nouvelElement->resistTower1 = resistTower1;
    nouvelElement->resistTower2 = resistTower2;
    nouvelElement->resistTower3 = resistTower3;
    nouvelElement->resistTower4 = resistTower4;
    nouvelElement->go = 0;
    nouvelElement->next = NULL;
    if(liste == NULL){
        return nouvelElement;
    }
    else{
        monster* temp=liste;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = nouvelElement;
        return liste;
    }
}

/*--------------------------------------------------------------
            SUPPRESSION DE MONSTRE
 --------------------------------------------------------------*/

l_monster supprimerMonster(l_monster liste, int idMonster){


    /* Liste vide, il n'y a plus rien à supprimer */
    if(liste == NULL)
        return NULL;

    /* Si l'élément en cours de traitement doit être supprimé */
    if(liste->idMonster == idMonster)
    {

	glColor3ub(255,25,25);
	glTranslatef(liste->x,liste->y,1);
	glBegin(GL_POLYGON);
        glVertex2f(0,10);
     	glVertex2f(-0, 20);
        glVertex2f(-30, 0);
        glVertex2f(0, -20);
	glVertex2f(5,30);
     	glVertex2f(-20, 2);
        glVertex2f(-23, -17);
        glVertex2f(20, 0);
   	glEnd();
        /* On le supprime en prenant soin de mémoriser
         l'adresse de l'élément suivant */
        monster* tmp = liste->next;
        free(liste);
        /* L'élément ayant été supprimé, la liste commencera à l'élément suivant
         pointant sur une liste qui ne contient plus aucun élément ayant la valeur recherchée */
        tmp = supprimerMonster(tmp, idMonster);
        return tmp;
    }
    else
    {
        /* Si l'élement en cours de traitement ne doit pas être supprimé,
         alors la liste finale commencera par cet élément et suivra une liste ne contenant
         plus d'élément ayant la valeur recherchée */
        liste->next = supprimerMonster(liste->next, idMonster);
        return liste;
    }
}

/*--------------------------------------------------------------
            SUPPRIMER TOUT LES MONSTRES
 --------------------------------------------------------------*/

l_monster supprimerAllMonster(l_monster liste){
    monster* tmp = liste;
    while(tmp!=NULL){
        tmp = tmp->next;
        free(tmp);
    }
    return NULL;
}

/*--------------------------------------------------------------
                DÉPLACEMENT DES MONSTRES
 --------------------------------------------------------------*/

void deplaceMonster (l_monster liste, float timeVague, int vague, char** message, int * bouge, int * WinLose){
    *bouge = *bouge+1;
    if(*bouge == 4){
        *bouge = 1;
    }
    int vitesse = 1;
    if(vague == 1){
        if(*bouge == 1 || *bouge == 3){
            vitesse = 1;
        }else{
            vitesse = 0;
        }
    }

    while (liste!=NULL) {
        //permet de evoyer les monster 1 a 1, monster avec idMonster==1 demarre a la premiere seconde...
        if(liste->next == NULL || fabs(liste->next->x-liste->x)>50 || fabs(liste->next->y-liste->y)>50){
            liste->go = 1;
        }
        if(liste->go == 1){
            //si le monster est a destination, il prend en destination le prochain noeud
            if(liste->x>liste->dest->x-vitesse && liste->x<liste->dest->x+vitesse && liste->y>liste->dest->y-vitesse && liste->y<liste->dest->y+vitesse){
                //printf("super\n");
                liste->x = liste->dest->x;
                liste->y = liste->dest->y;
                if(liste->dest->next!=NULL){
                    liste->oldDest = liste->dest;
                    liste->dest=liste->dest->next;
                }else{
                    *message = "YOU LOSE";
                    *WinLose = 2;
                }
            }
            // gestion des deplacements, haut, bas, gauche, droite


            //gauche a droite
            if((liste->dest->x)-(liste->x)>0){
                liste->x=(liste->x+vitesse);
                float penteOldDestDest = ((float)((liste->dest->y)-(liste->oldDest->y))/(float)((liste->dest->x)-(liste->oldDest->x)));

                if(penteOldDestDest > 1 || penteOldDestDest < -1){
                    //bas en haut
                    if((liste->dest->y)-(liste->y)>0){
                        penteOldDestDest = ((float)((liste->dest->x)-(liste->oldDest->x))/(float)((liste->dest->y)-(liste->oldDest->y)));
                        //printf("bas en haut\n");
                        liste->y=(liste->y+vitesse);
                        liste->x=(liste->oldDest->x)-((float)((liste->oldDest->y)-(liste->y))*penteOldDestDest);
                        liste->direction = 4;
                    }
                    //haut en bas en haut
                    if((liste->dest->y)-(liste->y)<0){
                        penteOldDestDest = ((float)((liste->dest->x)-(liste->oldDest->x))/(float)((liste->dest->y)-(liste->oldDest->y)));
                        //printf("haut en bas : %f\n",(liste->oldDest->x)-(float)((liste->oldDest->y)-(liste->y))*penteOldDestDest);
                        liste->y=(liste->y-vitesse);
                        liste->x=(liste->oldDest->x)-((float)((liste->oldDest->y)-(liste->y))*penteOldDestDest);
                        liste->direction = 1;
                    }
                }else{
                    //gauche a droite
                    //printf("gauche a droite\n");
                    liste->y=(liste->oldDest->y)+((float)((liste->x)-(liste->oldDest->x))*penteOldDestDest);
                    liste->direction = 3;
                }


            }
            //droite a gauche
            if((liste->dest->x)-(liste->x)<0){
                liste->x=(liste->x-vitesse);
                float penteOldDestDest = ((float)((liste->oldDest->y)-(liste->dest->y))/(float)((liste->oldDest->x)-(liste->dest->x)));

                if(penteOldDestDest > 1 || penteOldDestDest < -1){
                    //bas en haut
                    if((liste->dest->y)-(liste->y)>0){
                        penteOldDestDest = ((float)((liste->dest->x)-(liste->oldDest->x))/(float)((liste->dest->y)-(liste->oldDest->y)));
                        //printf("bas en haut\n");
                        liste->y=(liste->y+vitesse);
                        liste->x=(liste->oldDest->x)-((float)((liste->oldDest->y)-(liste->y))*penteOldDestDest);
                        liste->direction = 4;
                    }
                    //haut en bas en haut
                    if((liste->dest->y)-(liste->y)<0){
                        penteOldDestDest = ((float)((liste->dest->x)-(liste->oldDest->x))/(float)((liste->dest->y)-(liste->oldDest->y)));
                        //printf("haut en bas : %f\n",(liste->oldDest->x)-(float)((liste->oldDest->y)-(liste->y))*penteOldDestDest);
                        liste->y=(liste->y-vitesse);
                        liste->x=(liste->oldDest->x)-((float)((liste->oldDest->y)-(liste->y))*penteOldDestDest);
                        liste->direction = 1;
                    }
                }else{
                    //droite a gauche
                    //printf("droite a gauche : \n");
                    liste->y=(liste->oldDest->y)-((float)((liste->oldDest->x)-(liste->x))*penteOldDestDest);
                    liste->direction = 2;
                }
            }
            //bas en haut
            if((liste->dest->x)-(liste->x)==0 && (liste->dest->y)-(liste->y)>0){
                liste->y=(liste->y+vitesse);
                liste->direction = 4;
            }
            //haut en bas
            if((liste->dest->x)-(liste->x)==0 && (liste->dest->y)-(liste->y)<0){
                liste->y=(liste->y-vitesse);
                liste->direction = 1;
            }

        }
        liste=liste->next;
    }
}

/*--------------------------------------------------------------
            AFFICHAGE DES MONSTRES
 --------------------------------------------------------------*/

void AfficherMonster (l_monster liste, int vague, int * bouge){
    int mouv = *bouge;
    glEnable(GL_TEXTURE_2D);
    while (liste!=NULL) {
        if(liste->go == 1){
            if(vague == 5 ){
                glBindTexture(GL_TEXTURE_2D, 9);
		DessinMonstre(liste, vague, mouv);
            }else if(vague == 10){
                glBindTexture(GL_TEXTURE_2D, 13);
		DessinMonstre(liste, vague, mouv);
            }else if(vague == 15){
                glBindTexture(GL_TEXTURE_2D, 13);
		DessinMonstre(liste, vague, mouv);
            }else if(vague == 20){
                glBindTexture(GL_TEXTURE_2D, 13);
		DessinMonstre(liste, vague, mouv);
            }else{
		if(liste->idMonster == 0){
               		 glBindTexture(GL_TEXTURE_2D, 14);
               		 DessinMonstre(liste, vague, mouv);
            	}
           	if(liste->idMonster == 1){
                	glBindTexture(GL_TEXTURE_2D, 17);
                	DessinMonstre(liste, vague, mouv);
            	}
            	if(liste->idMonster == 2){
                	glBindTexture(GL_TEXTURE_2D, 14);
                	DessinMonstre(liste, vague, mouv);
            	}
            	if(liste->idMonster == 3){
                	glBindTexture(GL_TEXTURE_2D, 15);
                	DessinMonstre(liste, vague, mouv);
            	}
            	if(liste->idMonster == 4){
                        glBindTexture(GL_TEXTURE_2D, 16);
                    	DessinMonstre(liste, vague, mouv);
            	}
            	if(liste->idMonster == 5){
                	glBindTexture(GL_TEXTURE_2D, 17);
                	DessinMonstre(liste, vague, mouv);
            	}
            	if(liste->idMonster == 6){
                	glBindTexture(GL_TEXTURE_2D, 18);
                	DessinMonstre(liste, vague, mouv);
            	}
            	if(liste->idMonster == 7){
                	glBindTexture(GL_TEXTURE_2D, 19);
                	DessinMonstre(liste, vague, mouv);
            	}
            	if(liste->idMonster == 8){
                	glBindTexture(GL_TEXTURE_2D, 17);
                	DessinMonstre(liste, vague, mouv);
            	}
            	if(liste->idMonster == 9){
            	    glBindTexture(GL_TEXTURE_2D, 15);
            	    DessinMonstre(liste, vague, mouv);
            	}
            }




        }

        liste=liste->next;
    }
    glDisable(GL_TEXTURE_2D);
}

/*--------------------------------------------------------------
                    GESTION DES SPRITES
 --------------------------------------------------------------*/

void DessinMonstre(l_monster liste, int vague, int mouv){
    glLoadIdentity();
    glTranslatef(liste->x,liste->y,1);
    glColor3ub(255, 255, 255);

    glBegin(GL_QUADS);
    //HAUT GAUCHE
    glTexCoord2f(0.25+mouv*0.25, 0.25*(liste->direction-1));      glVertex2f(20,20);
    //HAUT DROITE
    glTexCoord2f(0+mouv*0.25, 0.25*(liste->direction-1));   glVertex2f(-20, 20);
    //BAS GAUCHE
    glTexCoord2f(0+mouv*0.25, 0.25+0.25*(liste->direction-1));         glVertex2f(-20, -20);
    //BAS DROIT
    glTexCoord2f(0.25+mouv*0.25, 0.25+0.25*(liste->direction-1));      glVertex2f(20, -20);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    //vie = vague*10000
    glColor3ub(50, 255, 20);
    glBegin(GL_QUADS);
    glVertex2f((liste->vie)*40/(vague*10000)-20,-25);
    glVertex2f(-20,-25);
    glVertex2f(-20,-20);
    glVertex2f((liste->vie)*40/(vague*10000)-20,-20);
    glEnd();

    glColor3ub(230, 50, 20);
    glBegin(GL_QUADS);
    glVertex2f(20,-25);
    glVertex2f((liste->vie)*40/(vague*10000)-20,-25);
    glVertex2f((liste->vie)*40/(vague*10000)-20,-20);
    glVertex2f(20,-20);
    glEnd();

    glLoadIdentity();
}
