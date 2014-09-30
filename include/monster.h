#ifndef __MONSTER_H__
#define __MONSTER_H__


#include "node.h"

typedef struct s_monster{
    int x;
    int y;
    node * dest;
    node * oldDest;
    int vitesse;
    int vie;
    int direction; // orientation du sprite
    int idMonster; //orde dans la vague
    int typeMonster;
    float resistTower1;
    float resistTower2;
    float resistTower3;
    float resistTower4;
    int go;
    struct s_monster* next;
} monster;
typedef monster* l_monster;

l_monster ajouterMonster(l_monster liste, l_node listeNode, int idMonster, int vie, int typeMonster, float resistTower1,float resistTower2,float resistTower3,float resistTower4);
l_monster supprimerMonster(l_monster liste, int idMonster);
l_monster supprimerAllMonster(l_monster liste);
void deplaceMonster (l_monster liste, float timeVague, int vague, char** message, int * bouge, int * WinLose);
void AfficherMonster (l_monster liste, int vague, int * bouge);
void DessinMonstre(l_monster liste, int vague, int mouv);

#endif