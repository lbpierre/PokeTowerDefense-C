#ifndef __TEXTE_H__
#define __TEXTE_H__

GLuint* titre(GLuint *textureId);
GLuint* description(GLuint *textureId, int type);
GLuint* descriptionTowerSelected(GLuint *textureId, l_tower towerSelected);
GLuint* afficheCoins(GLuint *textureId, int coins);
GLuint* afficheTexte(GLuint *textureId, int x, int y, char * Texte, int r, int g, int b, int size);
GLuint* afficheVagues(GLuint *textureId, int vague);
void afficheMessageErreur(void);

#endif