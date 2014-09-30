#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <math.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "node.h"
#include "carte.h"


#define MAXSZ 100
#define BUFFSIZE 1024

/*--------------------------------------------------------------
                VALIDATION DU FICHIER ITD
 ---------------------------------------------------------------*/

int validationFichierItd(GLubyte* couleurChemin,GLubyte* couleurConstruct, GLubyte* couleurNoeud,GLubyte* couleurNoeudIn,GLubyte* couleurNoeudOut,char* fileName, char** nomFichierImage){
    
    FILE *fIn;
    char nomFichierImage2[15];
    
    int i;
    i = 0;
    int nbLigneParametre = 8;
    
    //Le nombre de ligne total moins le nombre de ligne de paramètre afin d'avoir le nombre de
    //lignes de coordonnées x y pour les noeuds, afin de comparer le nombre de noeud au nombre obtenu
    int nbLigneCoordNoeud = nombreDeLigneFichierITD(fileName)-nbLigneParametre;
    //printf("le NOMBRE DE LIGNE DE COORD NOEUD EST DE : %d\n", nbLigneCoordNoeud);
    
    //malloc obligatoire avec les fscanf sinon segmantation fault11
    char* titreFichierItd = (char *) malloc(10 * sizeof(char));
    char* noeud = (char *) malloc(10 * sizeof(char));
    char* chemin = (char *) malloc(10 * sizeof(char));
    char* carte = (char *) malloc(10 * sizeof(char));
    char* construct = (char *) malloc(10 * sizeof(char));
    char* in = (char *) malloc(10 * sizeof(char));
    char* out = (char *) malloc(10 * sizeof(char));
    int valItd, validation, valNombreDeNoeud;
    int tab[15];
    int tabXY[200];
    
    validation = 0;
    //Ouverture du fichier
    fIn = fopen (fileName, "r");
    
    //Permet de comparer les chaines
    int resComp = 0;
    
    fscanf(fIn, "%s %d", titreFichierItd, &valItd);
    fscanf(fIn, "%s %s", carte, nomFichierImage2);
    fscanf(fIn,"%s %d %d %d", chemin, &tab[0],&tab[1],&tab[2]);
    fscanf(fIn,"%s %d %d %d", noeud, &tab[3],&tab[4],&tab[5]);
    fscanf(fIn,"%s %d %d %d", construct, &tab[6],&tab[7],&tab[8]);
    fscanf(fIn,"%s %d %d %d", in, &tab[9],&tab[10],&tab[11]);
    fscanf(fIn,"%s %d %d %d", out, &tab[12],&tab[13],&tab[14]);
    fscanf(fIn,"%d", &valNombreDeNoeud);
    
    //3.3 - Bonne validitée des noeuds : Les coordonnées de chaque noeud dans le ﬁchier .itf doit correspondre à un pixel de l’image.
    
    if(valNombreDeNoeud == nbLigneCoordNoeud){
        //ici qu'il faut lancer la fonction qui permet d'ajouter les valeurs !!!!!!
        //Je place les valeurs récupérer dans un tableau de coordoonées pour les vérifiers.
        for(i=0; i<nbLigneCoordNoeud; i++){
            fscanf(fIn,"%d %d", &tabXY[i],&tabXY[i+1]);
            //printf("%d %d\n", tabXY[i],tabXY[i+1]);
            if((tabXY[i]>=0)&&(tabXY[i]<=1000)&&(tabXY[i+1]>=0)&&(tabXY[i+1]<=800)){
                //validationVal = 1;
                printf("La valeur x %d est valide (entre 0 et 1000)\n", tabXY[i]);
                printf("La valeur y %d est valide (entre 0 et 800)\n", tabXY[i+1]);
            }else{
                printf("ERREUR : La valeur %d n'est pas valide (entre 0 et 255) à la ligne %d du fichier %s %d \n", tabXY[i], i+nbLigneCoordNoeud, titreFichierItd, valItd);
                validation = 1;
            }
        }
        
        /*if(nombreDeLigneLueItd==valNombreDeNoeud){
         printf("Le nombre de ligne lue est bien égal au nombre de ligne tot")
         }*/
        
        //printf(" Première ligne :%s %d\n", titreFichierItd, valItd);
        //printf("deuxième ligne carte = %s %s\n", carte, *nomFichierImage);
        //printf("troisième ligne chemin = %s %d %d %d\n", chemin, tab[0],tab[1],tab[2]);
        //printf("quatrième ligne noeud = %s %d %d %d\n", noeud, tab[3],tab[4],tab[5]);
        //printf("cinquième ligne construct = %s %d %d %d\n", construct, tab[6],tab[7],tab[8]);
        //printf("sixième ligne in = %s %d %d %d\n", in, tab[9],tab[10],tab[11]);
        //printf("septième ligne out = %s %d %d %d\n\n", out, tab[12],tab[13],tab[14]);
        
        //On vérifie que tout les intitulés sont bons, sinon validation = 1
        if(resComp == strcmp(titreFichierItd, "@ITD")){
            printf("La valeur @ITD est bonne\n");
        }else{
            printf("ERREUR :problème du titre du fichier\n");
            validation = 1;
        }
        
        if(resComp == strcmp(carte, "carte")){
            printf("L'intitulé carte est bon\n");
            
            strcat(*nomFichierImage, nomFichierImage2);
            printf("LE NOM DU FICHIER IMAGE : %s", *nomFichierImage);
        
        }else{
            printf("ERREUR :problème du titre de la carte\n");
            printf("Le chaine doit être : %s, et non pas %s\n", "carte", carte);
            validation = 1;
        }
        
        if(resComp == strcmp(chemin, "chemin")){
            printf("L'intitulé chemin est bon\n");
            
            couleurChemin[0] = tab[0];
            couleurChemin[1] = tab[1];
            couleurChemin[2] = tab[2];
            
        }else{
            printf("ERREUR :problème du titre du chemin\n");
            printf("Le chaine doit être : %s, et non pas %s\n", "chemin", chemin);
            validation = 1;
        }
        
        if(resComp == strcmp(noeud, "noeud")){
            printf("L'intitulé noeud est bon\n");
            
            couleurNoeud[0] = tab[3];
            couleurNoeud[1] = tab[4];
            couleurNoeud[2] = tab[5];
            
        }else{
            printf("ERREUR :problème du titre du noeud\n");
            printf("Le chaine doit être : %s, et non pas %s\n", "noeud", noeud);
            validation = 1;
        }
        
        if(resComp == strcmp(construct, "construct")){
            printf("L'intitulé construct est bon\n");
            
            couleurConstruct[0] = tab[6];
            couleurConstruct[1] = tab[7];
            couleurConstruct[2] = tab[8];
            
        }else{
            printf("ERREUR :problème du titre de la construct\n");
            printf("Le chaine doit être : %s, et non pas %s\n", "construct", construct);
            validation = 1;
        }
        
        if(resComp == strcmp(in, "in")){
            printf("L'intitulé in est bon\n");
            
            couleurNoeudIn[0] = tab[9];
            couleurNoeudIn[1] = tab[10];
            couleurNoeudIn[2] = tab[11];
            
        }else{
            printf("ERREUR :problème du titre du in\n");
            printf("Le chaine doit être : %s, et non pas %s\n", "in", in);
            validation = 1;
        }
        
        if(resComp == strcmp(out, "out")){
            printf("L'intitulé out est bon\n\n");
            
            couleurNoeudOut[0] = tab[12];
            couleurNoeudOut[1] = tab[13];
            couleurNoeudOut[2] = tab[14];
            
        }else{
            printf("ERREUR :problème du titre du out\n");
            printf("Le chaine doit être : %s, et non pas %s\n", "out", out);
            validation = 1;
        }
        
        //On parcour les valeurs des 8 premières informations
        //Pour vérifier si elles sont valides
        for(i=0; i<15; i++){
            if((tab[i]>=0)&&(tab[i]<=255)){
                //validationVal = 1;
                printf("La valeur %d  à la ligne %d est valide (entre 0 et 255)\n", tab[i], i);
            }else{
                printf("ERREUR : La valeur %d à la ligne %d n'est pas valide (entre 0 et 255)\n", tab[i], i);
                validation = 1;
            }
        }
        //FIN DU if(valNombreDeNoeud == nbLigneCoordNoeud)
    }else{
        printf("\nErreur le nombre de noeud lue n'est pas égal à celui afficher ligne 8 du fichier itd\n");
        validation = 1;
    }
    
    free(titreFichierItd);
    free(chemin);
    free(carte);
    free(noeud);
    free(construct);
    free(in);
    free(out);
    
    return validation;
}

/*--------------------------------------------------------------
            AJOUT DES COORDONNÉES AU NOEUDS
 ---------------------------------------------------------------*/

l_node addCoordNoeud(char* fileName) {
    char line[MAXSZ];
    int x, y;
    FILE *fIn;
    l_node ma_liste = NULL;
    int compteurLigneNoeud;
    
    //compteur j à utiliser pour boucler sur les valeurs des points formant un segment
    int i;
    //compteur à utiliser pour les lignes dans la boucle while.
    i = 0;
    //printf("nom du fichier ////////// %s", fileName);
    fIn = fopen (fileName, "r");
    // effectue le truc ligne par ligne.
    while (fgets (line, sizeof(line), fIn) != NULL) {
        i++;
        if(i>=8){
            //listing des 8 lignes qui permettent de créer des segments
            fscanf(fIn, "%d %d", &x,&y);
            printf("x : %d y : %d \n", x,y);
            ma_liste = ajouterNode(ma_liste, x, y);
            compteurLigneNoeud++;
            
        }//endif
    }//endWhile
    //printf("Nombre de ligne dans la fonction %d", compteurLigneNoeud);
    fclose (fIn);
    return ma_liste;
}

/*--------------------------------------------------------------
        COMPTE LE NOMBRE DE LIGNE TOTAL DU FICHIER ITD
 ---------------------------------------------------------------*/

int nombreDeLigneFichierITD(char* fileName){
    FILE *fp=fopen(fileName,"r+");
    
    //ON SUPPRIME LES LIGNES VIDES INUTILES
    supprLignesVides(fileName);
    
    int ligne=0;
    char buffer[BUFFSIZE];
    while(fgets(buffer, sizeof buffer, fp)) {
        ligne++;
    }
    //printf("NOMBRE DE LIGNES : %d\n", ligne);
    fclose(fp),
    fp=NULL;
    
    return ligne;
}

/*--------------------------------------------------------------
        SUPPRIME LES LIGNES VIDES DU FICHIER ITD
 ---------------------------------------------------------------*/

void supprLignesVides(char* fileName)
{
    FILE *f;
    char buffer[BUFFSIZE]={0};
    if ((f=fopen(fileName,"r+")))
    {
        char c; int i=0,isempty=1;
        FILE *tmp;
        tmp=tmpfile();
        if (tmp != NULL)
        {
            while (((c=getc(f))!=EOF))
            {
                buffer[i++]=c;
                if (c!=' '&&c!='\n'&&c!='\t')
                {
                    isempty=0;
                }
                else
                {
                    if (c=='\n'&&isempty==1)
                    {
                        buffer[i]='\0';
                        i=0;
                        isempty=1;
                    }
                    else
                        if (c=='\n'&& isempty==0)
                        {
                            buffer[i]='\0';
                            fprintf(tmp,"%s",buffer);
                            i=0;
                            isempty=1;
                        }
                }
            }
            //LAST LINE
            if (!isempty)
            {
                buffer[i]='\0';
                fprintf(tmp,"%s",buffer);
            }
            fclose (f);
            if ((f=fopen(fileName,"w+")))
            {
                filecopy(tmp,f);
                fclose(tmp);
                fclose(f);
            }
            else
            {
                printf("impossible de reouvrir le fichier f.\n");
                fclose(tmp);
            }
        }
        else
        {
            printf("Impossible de créer un fichier temp.\n");
            fclose(f);
        }
    }
    else
    {
        printf("Impossible d'ouvrir le fichier\n");
    }
}

/*----------------------------------------------------------------------
 COPIE LE FICHIER TEXTE POUR LE MODIFIER EN SUPPRIMANT LES LIGNES VIDES
 ----------------------------------------------------------------------*/

void filecopy(FILE *from, FILE *to)
{
    int c;
    rewind(from);
    rewind(to);
    while ((c=getc(from))!=EOF)
    {
        putc(c,to);
    }
}
