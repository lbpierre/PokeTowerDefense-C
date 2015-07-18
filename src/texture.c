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
#include "texture.h"


/* ********************************************************************* */
/*obtenirPixel : permet de récupérer la couleur d'un pixel
 Paramètres d'entrée/sortie :
 SDL_Surface *surface : la surface sur laquelle on va récupérer la couleur d'un pixel
 int x : la coordonnée en x du pixel à récupérer
 int y : la coordonnée en y du pixel à récupérer

 Uint32 resultat : la fonction renvoie le pixel aux coordonnées (x,y) dans la surface
 */
Uint32 obtenirPixel(SDL_Surface *surface, int x, int y)
{
    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
     En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
     de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /* Ici p est l'adresse du pixel que l'on veut connaitre */
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    switch(nbOctetsParPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;

            /*Ne devrait pas arriver, mais évite les erreurs*/
        default:
            return 0;
    }
}

/* ********************************************************************* */
/*definirPixel : permet de modifier la couleur d'un pixel
 Paramètres d'entrée/sortie :
 SDL_Surface *surface : la surface sur laquelle on va modifier la couleur d'un pixel
 int x : la coordonnée en x du pixel à modifier
 int y : la coordonnée en y du pixel à modifier
 Uint32 pixel : le pixel à insérer
 */
void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
     En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
     de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /*Ici p est l'adresse du pixel que l'on veut modifier*/
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

////////////////
////TEXTURE///////
////////////////

GLuint* texture(int **tab, char** nomFichierImage){

    //map
    GLuint *textureId;
    textureId = (GLuint*)(malloc(50*sizeof(GLuint)));
    GLenum format;
    //Concerne le chargement de texture.
    glGenTextures(30, textureId);
    //1
    char *Filename = *nomFichierImage;
    // chargement image
    SDL_Surface *image;
    image=IMG_Load(Filename);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename);
	}
	else{
	    printf("ca marche bien\n");
	}


    glBindTexture(GL_TEXTURE_2D, textureId[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Uint8 nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    /////////////////////////////
    ////Gestion re-mapping///////
    ////////////////////////////

    Uint32 pixel;
    Uint8 r,g,b,a;
    int x,y;
    int ecartType = 50;
    int valConstr = 1;
    int valNonConstr = 0;

    SDL_LockSurface(image); /*On bloque la surface*/
    for (y=0;y<730;y++)
    {
        for (x=0;x<1000;x++)
        {
            pixel = obtenirPixel(image,x,y);
            SDL_GetRGBA(pixel, image->format, &r, &g, &b, &a);

            /*Ici, on mettra du code pour modifier les composantes du pixel.*/
            if(r >= 255-ecartType && g <= 0+ecartType && b>=255-ecartType){
                //printf("couleur r : %d g : %d b : %d et %d\n",r, g, b, a);
                //printf("couleur X : %d Y : %d\n", x, y);
                //On place la valeur qui définit s'il est possible de construire ou non.
                tab[y][x] = valConstr;

                //valeurs remapping
                r = 184;
                g = 200;
                b = 224;
                a = 255;

                /*Et une fois qu'on les a modifiés :*/
                pixel = SDL_MapRGBA(image->format, r, g, b, a);
                //printf("couleur r : %d g : %d b : %d et %d\n",r, g, b, a);

            }else{
                //On place la valeur qui définit qu'il est impossible de construire.
                tab[y][x] = valNonConstr;
            }
            /*Et pour changer la valeur d'un pixel :*/
            definirPixel(image,x,y,pixel);
        }
    }
    SDL_UnlockSurface(image); /*On libère la surface, elle peut être utilisée*/

    /*
     for (y=0;y<image->h;y++)
     {
     for (x=0;x<image->w;x++)
     {
         printf("%d\t", tab[y][x]);
         printf("\n");
     }
     }
    */

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, format,GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //perso
    char *Filename2 = "images/monsters/pokemon.png";
    // chargement image
    image=IMG_Load(Filename2);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename2);
	}
	else{
	    printf("ca marche bien\n");
	}

    glBindTexture(GL_TEXTURE_2D, textureId[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //menu
    char *Filename3 = "images/scenario/barmenu.png";
    // chargement image
    image=IMG_Load(Filename3);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename3);
	}
	else{
	    printf("ca marche bien\n");
	}

    glBindTexture(GL_TEXTURE_2D, textureId[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //tower
    char *Filename4 = "images/towers/pokeballs.png";
    // chargement image
    image=IMG_Load(Filename4);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename4);
	}
	else{
	    printf("ca marche bien\n");
	}

    glBindTexture(GL_TEXTURE_2D, textureId[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img start
    char *Filename5 = "images/scenario/start.jpg";
    // chargement image
    image=IMG_Load(Filename5);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename5);
	}
	else{
	    printf("start\n");
	}

    glBindTexture(GL_TEXTURE_2D, textureId[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img pause
    char *Filename6 = "images/scenario/pause.png";
    // chargement image
    image=IMG_Load(Filename6);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename6);
	}
	else{
	    printf("start\n");
	}

    glBindTexture(GL_TEXTURE_2D, textureId[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img Win
    char *Filename7 = "images/scenario/youwin.png";
    // chargement image
    image=IMG_Load(Filename7);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename7);
	}
	else{
	    printf("start\n");
	}

    glBindTexture(GL_TEXTURE_2D, textureId[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img Lose
    char *Filename8 = "images/scenario/youloose.png";
    // chargement image
    image=IMG_Load(Filename8);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename8);
	}
	else{
	    printf("start\n");
	}

    glBindTexture(GL_TEXTURE_2D, textureId[7]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img perso mechant
    char *Filename9 = "images/monsters/pokemonMechant.png";
    // chargement image
    image=IMG_Load(Filename9);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename9);
	}
	else{
	    printf("perso\n");
	}
    glBindTexture(GL_TEXTURE_2D, textureId[8]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img HELP
    char *Filename10 = "images/scenario/AIDE.png";
    // chargement image
    image=IMG_Load(Filename10);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename10);
	}
	else{
	    printf("perso\n");
	}
    glBindTexture(GL_TEXTURE_2D, textureId[9]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img CURSOR
    char *Filename11 = "images/scenario/hand_cursor2.png";
    // chargement image
    image=IMG_Load(Filename11);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename11);
	}
	else{
	    printf("perso\n");
	}
    glBindTexture(GL_TEXTURE_2D, textureId[11]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img mechant 2
    char *Filename12 = "images/monsters/autreSprite.png";
    // chargement image
    image=IMG_Load(Filename12);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename12);
	}
	else{
	    printf("mechant 2\n");
	}
    glBindTexture(GL_TEXTURE_2D, textureId[12]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img mechant 3
    char *Filename13 = "images/monsters/autreMec.png";
    // chargement image
    image=IMG_Load(Filename13);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename13);
	}
	else{
	    printf("mechant 3\n");
	}
    glBindTexture(GL_TEXTURE_2D, textureId[13]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img mechant 3
    char *Filename14 = "images/monsters/autreFille.png";
    // chargement image
    image=IMG_Load(Filename14);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename14);
	}
	else{
	    printf("mechant 4\n");
	}
    glBindTexture(GL_TEXTURE_2D, textureId[14]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img mechant 5
    char *Filename15 = "images/monsters/autreMec2.png";
    // chargement image
    image=IMG_Load(Filename15);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename15);
	}
	else{
	    printf("mechant 5\n");
	}
    glBindTexture(GL_TEXTURE_2D, textureId[15]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img mechant 3
    char *Filename16 = "images/monsters/pikachu.png";
    // chargement image
    image=IMG_Load(Filename16);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename16);
	}
	else{
	    printf("mechant 6\n");
	}
    glBindTexture(GL_TEXTURE_2D, textureId[16]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img mechant 3
    char *Filename17 = "images/monsters/eau.png";
    // chargement image
    image=IMG_Load(Filename17);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename17);
	}
	else{
	    printf("mechant 7\n");
	}
    glBindTexture(GL_TEXTURE_2D, textureId[17]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    //img mechant 3
    char *Filename18 = "images/monsters/fille2.png";
    // chargement image
    image=IMG_Load(Filename18);
	if(image == NULL) {
	    fprintf(stderr,"impossible de charger l'image %s\n", Filename18);
	}
	else{
	    printf("mechant 8\n");
	}
    glBindTexture(GL_TEXTURE_2D, textureId[18]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	nOfColors = image->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
	    	format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
        if (image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
	} else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image);

    return textureId;

}
