#ifndef __TOWER_H__
#define __TOWER_H__


#include "monster.h"

typedef struct s_tower{
    int x; //position
    int y;
    int rayon;
    int cost;
    int type;
    int puissance;
    float cadence;
    int active;
    float timeCrea;
    float timeConst;
    float timeDernierTir;
    struct s_tower* next;
} tower;
typedef tower* l_tower;

l_tower ajouterTower(l_tower liste, int* coins, int x, int y,int type, char** message);
l_tower supprimerAllTower(l_tower liste);
l_tower selectTower(l_tower liste, int x, int y, char** message);
void glisserTower(int x, int y, int type);
l_tower supprimerTower(l_tower liste, l_tower aSuppr);
void afficherTower(l_tower liste, int ray, l_tower towerSelected);
void gestionTower(l_tower listeTower, l_monster listeMonster);

#endif
