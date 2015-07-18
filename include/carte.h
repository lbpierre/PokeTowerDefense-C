#ifndef __CARTE_H__
#define __CARTE_H__

void openTextFile(char* fileName);
l_node addCoordNoeud(char* fileName);

int validationFichierItd(GLubyte* couleurChemin, GLubyte* couleurConstruct, GLubyte* couleurNoeud, GLubyte* couleurNoeudIn, GLubyte* couleurNoeudOut, char* fileName, char** nomFichierImage);
int nombreDeLigneFichierITD(char* fileName);

void supprLignesVides(char* fileName);
void filecopy(FILE *from, FILE *to);

#endif
