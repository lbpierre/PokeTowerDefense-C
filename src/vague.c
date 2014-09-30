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

#include "vague.h"
#include "node.h"
#include "monster.h"


////////////////
////VAGUE///////
////////////////

l_monster genereVague(int vague, l_monster monst, l_node ma_liste, int nbMonster){
    float resistTower1 = 1;
    float resistTower2 = 1;
    float resistTower3 = 1;
    float resistTower4 = 1;
    if(vague == 5){
        resistTower1 = 0.5;
    }
    if(vague == 10){
        resistTower2 = 0.5;
    }
    if(vague == 15){
        resistTower2 = 0.3;
        resistTower3 = 0.5;
    }
    if(vague == 20){
        resistTower2 = 0.7;
        resistTower3 = 0.5;
        resistTower4 = 0.5;
    }
    int idMonster=0;
    int vie = vague*10000;
    for(idMonster=0;idMonster<nbMonster;idMonster++){
        monst=ajouterMonster(monst, ma_liste, idMonster, vie, 1, resistTower1, resistTower2, resistTower3, resistTower4);
        //ajouterMonster(l_monster monst, l_node ma_liste, int idMonster, int vie, int typeMonster);
    }
    return monst;
}
