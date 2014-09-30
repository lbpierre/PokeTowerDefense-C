#ifndef __MENU_H__
#define __MENU_H__



void afficherMenu(int selected);
void afficherMenuPause(int * pause);
void afficherMenuStart(int * pause);
void afficherMenuWin(int * pause);
void afficherMenuLose(int * pause);
void afficherMenuHelp(GLuint* textureId);
void afficherMouse(int xMove, int yMove, int PIXEL_HEIGHT);
#endif