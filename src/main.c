#define M_PI 3.14159265358979323846

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

#include <math.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "sdl.h"
#include "vague.h"
#include "node.h"
#include "tower.h"
#include "monster.h"
#include "texture.h"
#include "carte.h"
#include "menu.h"
#include "texte.h"
#include "chemin.h"
#include "map.h"

static unsigned int WINDOW_WIDTH = 1000;
static unsigned int WINDOW_HEIGHT = 800;
//static unsigned int PIXEL_WIDTH = 1000; INUTILISÉ
static unsigned int PIXEL_HEIGHT = 800;
static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    
    /*--------------------------------------------------------------
     INITIALISATION
     --------------------------------------------------------------*/
    
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    /*--------------------------------------------------------------
                    INITIALISATION SDL TTF POUR LE TEXTE
     --------------------------------------------------------------*/
    
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    
    /*--------------------------------------------------------------
                    INITIALISATION SDL MIXER POUR LA MUSIQUE
     --------------------------------------------------------------*/
    
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        printf("%s", Mix_GetError());
    }
    
    /*--------------------------------------------------------------
                    DÉCLARATION ET ALLOCATION
                   QUI FAIT LA TAILLE DE L'IMAGE
                    ET QUI PERMET DE SAVOIR SI 
                 UNE ZONE EST CONSTRUCTIBLE OU NON
                1 CONSTRUCTIBLE, 0 NON CONSTRUCTIBLE
     --------------------------------------------------------------*/

    //Déclaration du tabl de construction
    int **tab = (int**) malloc(730 * sizeof(int*));//hauteur
    size_t i;
    for(i = 0 ; i < 1000 ; i++)
        tab[i] = malloc(1000 * sizeof(int)); //largeur
    if(tab == NULL){
        fprintf(stderr, "Erreur allocation : tab\n");
        exit(EXIT_FAILURE);
    }
    
    /*--------------------------------------------------------------
                ON DEMANDE À L'UTILISATEUR DE RENTRER
                LE NOM DU FICHIER ITD, S'IL N'EXISTE PAS
                            ON ARRÊTE LÀ.
     --------------------------------------------------------------*/
    
    //chargement nom fichier
    char fileName2[15];
    char fileName[30] = "data/";
    printf("NOM DU FICHIER À CHARGER ex : map1.itd : ");
    scanf("%s", fileName2);
    strcat(fileName,fileName2);
    printf("TEXTE FICHIER À CHARGER : %s", fileName);
    
    FILE *fIn;
    if ( (fIn = fopen ( fileName, "r" ) ) == NULL )
    {
        printf ( "MAIN : fichier non-existant!\n" );
        SDL_Quit();
        return EXIT_FAILURE;
    }
    else
    {
        printf ( "Le fichier existe!\n" );
        fclose ( fIn );
    }
    
    /*--------------------------------------------------------------
                    VALIDATION FICHIER ITD.
     --------------------------------------------------------------*/
    
    //Initialisation des tableaux rgb grâce à GLubyte
    GLubyte couleurConstruct[3];
    GLubyte couleurChemin[3];
    GLubyte couleurNoeud[3];
    GLubyte couleurNoeudIn[3];
    GLubyte couleurNoeudOut[3];
    
    //On met le préfixe afin de récuperer le nom de l'image
    //de map au sein du fichier itd, puis les concatener.
    char label[20] = "images/";
    char* nomFichierImage = label;
    
    //On test si le fichier est bien valide et on remplit les valeurs des tableaux de couleurs
    //S'il ne l'est pas on quitte la fenêtre et le jeu.
    if(validationFichierItd(couleurChemin, couleurConstruct, couleurNoeud, couleurNoeudIn, couleurNoeudOut, fileName, &nomFichierImage) == 0){
        //printf("Le fichier est bien un fichier @ITD valide")
        //printf("couleur constructions : %d %d %d\n",couleurConstruct[0],couleurConstruct[1],couleurConstruct[2]);
        //printf("couleur chemin : %d %d %d\n",couleurChemin[0],couleurChemin[1],couleurChemin[2]);
        //printf("couleur noeuds : %d %d %d\n",couleurNoeud[0],couleurNoeud[1],couleurNoeud[2]);
        //printf("couleur noeud in : %d %d %d\n",couleurNoeudIn[0],couleurNoeudIn[1],couleurNoeudIn[2]);
        //couleur Noeud fin
        //printf("Couleur noeud out : %d %d %d\n",couleurNoeudOut[0],couleurNoeudOut[1],couleurNoeudOut[2]);
    }else{
        printf("\nMAIN : IMPOSSIBLE DE DEMMARER LE JEUX DES VALEURS DU FICHIER ITD NE SONT PAS VALIDES\n");
        SDL_Quit();
        return EXIT_FAILURE;
    }//endelseValidationFichier
    
    /*--------------------------------------------------------------
                    INITIALISATION DES LISTES
     --------------------------------------------------------------*/
    
    //Gestion des noeuds
    l_node ma_liste = NULL;
    //On ajoute les coordonnées des noeuds
    ma_liste = addCoordNoeud(fileName);
    //On affiche les noeuds
    //afficherNodes(ma_liste);
    
    //Liste des monster//
    l_monster monst=NULL;
    
    //Liste des tower//
    l_tower towers=NULL;
    l_tower towerSelected=NULL;
    
    /*--------------------------------------------------------------
                    DÉCLARATION DES SONS DU JEU
     --------------------------------------------------------------*/
    
    //Les sons de tire etc...
    Mix_AllocateChannels(32); //Allouer 32 canaux
    Mix_Volume(1, MIX_MAX_VOLUME/2); //Mettre à mi-volume le post 1
    Mix_Volume(2, MIX_MAX_VOLUME/2); //Mettre à mi-volume le post 2
    Mix_Volume(3, MIX_MAX_VOLUME/2); //Mettre à mi-volume le post 3
    Mix_Volume(4, MIX_MAX_VOLUME/9); //Mettre à mi-volume le post 4
    
    
    //Son lorsque qu'une vague est finie
    Mix_Chunk *LevelUp;//Créer un pointeur pour stocker un .WAV
    LevelUp = Mix_LoadWAV("son/LevelUp.ogg"); //Charger un wav dans un pointeur
    Mix_VolumeChunk(LevelUp, MIX_MAX_VOLUME/2); //Mettre un volume pour ce wav
    
    //Son de sélection d'une tower
    Mix_Chunk *clickSelectTower;
    clickSelectTower = Mix_LoadWAV("son/magic100.ogg");
    Mix_VolumeChunk(clickSelectTower, MIX_MAX_VOLUME/2);
    
    //Le son lorsque l'on ce met en pause
    Mix_Chunk *PauseJeu;
    PauseJeu = Mix_LoadWAV("son/Warp.ogg"); 
    Mix_VolumeChunk(PauseJeu, MIX_MAX_VOLUME/2); 
    
    //Le son lorsque l'on quitte la pause
    Mix_Chunk *QuitPauseJeu;
    QuitPauseJeu = Mix_LoadWAV("son/WarpQuit.ogg");
    Mix_VolumeChunk(QuitPauseJeu, MIX_MAX_VOLUME/2);
    
    //Musiques de fond ( ne fonctionne pas sur les ordinateurs de la fac
    //Car ils ne lisent pas les sons midi, "no soundcard detected".
    
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8); //Mettre le volume à la moitié
    Mix_Music *musique; //Création d'un pointeur de type Mix_Music
    musique = Mix_LoadMUS("son/Opening.mid"); //Chargement de la musique
    
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8); 
    Mix_Music *musiqueFin; 
    musiqueFin = Mix_LoadMUS("son/defaite.mid"); 
    
    Mix_VolumeMusic(MIX_MAX_VOLUME / 8); 
    Mix_Music *musiqueVictoire; 
    musiqueVictoire = Mix_LoadMUS("son/pokerock.mid"); 
    
    //Paramètre le mode vidéo avec une largeur, hauteur, et le nombre
    //de bits par pixel.
    setVideoMode();
    //Nom de la fenêtre.
    SDL_WM_SetCaption("Pokimac Tower Defense", NULL);
    //Specifie le diamètre des points rasterisés.
    glPointSize(20);
    
    
    /*--------------------------------------------------------------
                            VARIABLES JEU
     --------------------------------------------------------------*/
    
    //position cursor
    int xClicked = -1, yClicked=-1;
    int xMove = -1, yMove=-1;
    //pause help
    int pause = 1;
    int help = 0;
    //bonhomme qui bouge
    int bouge = 0;
    //gagne ou perdu
    int WinLose = 0;
    //Si c'est le début du jeu
    int gameStart = 1;
    //vague
    int vague = 0;
    float timeVague;
    timeVague = 0.0;
    //time
    Uint32 startTime;
    startTime = 0.0;
    float SecDixieme = 0;
    float dernierMouv = 0;
    //player
    int coins = 100;
    
    //message
    char** message;
    message = malloc(50*sizeof(char));
    if(message == NULL){
        fprintf(stderr, "Erreur allocation : MESSAGE\n");
        exit(EXIT_FAILURE);
    }
    *message = NULL;
    
    //gestion d'évennement
    int towerTypeSelected = 0;
    int RaySelected = 0;
    //On crée le pointeur sur la texture.
    GLuint* textureId;
    textureId = texture(tab, &nomFichierImage);
    
    /*--------------------------------------------------------------
                        GESTION DE L'AFFICHAGE
     --------------------------------------------------------------*/
    
    int loop = 1;
    while(loop) {
        glClear(GL_COLOR_BUFFER_BIT);
        SecDixieme = (float)SDL_GetTicks()/1000.f;
        
        /*system("clear" );
        printf("///////////////////////////  \033[37;41mSUPER TOWER\033[30;49m  ////////////////////////////\n");
        printf("///////////////////////////  \033[37;41mVAGUE %d\033[30;49m     ////////////////////////////\n",vague);
        printf("///   Coins : %d \n",coins);
        printf("///   selected : %d \n",towerTypeSelected);
        printf("///   message : %s \n",*message);
        printf("///   Tower Clicked : %d \n",towerClicked);
        printf("/////////////////////////////////////////////////////////////////////\n");
         */
        
        /* dessin */
        
        glMatrixMode(GL_MODELVIEW);
        DessinMap(); //On affiche la map

        //chemin
        if(RaySelected == 1){
            chemin(ma_liste,couleurChemin[0],couleurChemin[1],couleurChemin[2], couleurNoeudIn[0], couleurNoeudIn[1], couleurNoeudIn[2], couleurNoeudOut[0], couleurNoeudOut[1], couleurNoeudOut[2]);
        }
        
        //Prend la position du curseur
        SDL_GetMouseState(&xMove, &yMove);
        
        // si la partie est en cours
        if(WinLose == 0){
            Mix_PlayMusic(musique, -1); //Jouer infiniment la musique de base
            
            // affichage des tower
            afficherTower(towers, RaySelected, towerSelected);
            
            //gestion des vagues
            if(monst==NULL && pause == 0){
                if(vague == 20){
                    WinLose = 1;
                }
                if(WinLose == 0){
                    vague++;
                    if(vague == 1){
                        timeVague = 0.0;
                    }
                    timeVague += startTime;
                    monst = genereVague(vague, monst, ma_liste, 10);
                    //genereVague(int vague, l_monster monst, l_node ma_liste, int nbMonster);
                    Mix_PlayChannel(2, LevelUp, 0);
                }
            }
            else{
                startTime = SDL_GetTicks() - timeVague;
            }
            
            //on supprime les monster dead
            l_monster tmpMonster;
            tmpMonster = monst;
            while(tmpMonster != NULL){
                if(tmpMonster->vie<1){
                    monst = supprimerMonster(monst, tmpMonster->idMonster);
                    coins +=5+vague-1;
                }
                tmpMonster=tmpMonster->next;
            }
            
            /*--------------------------------------------------------------
                                GESTION DES 10 MS.
             --------------------------------------------------------------*/
            
            //si il s'est passé 0.1 sec depuis la dernière gestion des evenements
            if(SecDixieme-dernierMouv > 0.1 && pause == 0){
                dernierMouv = SecDixieme;
                //gestion monster
                if(monst!=NULL){
                    // on gere les deplacement des monsters
                    deplaceMonster(monst,((float)startTime)/1000, vague, message, &bouge, &WinLose);
                    
                    // gestiondes tir des tower
                    gestionTower(towers, monst);
                    
                }else{
                    printf("\033[37;41m");
                    printf("YOU WIN Vague : %d\n", vague);
                    printf("\033[30;49m");
                }
                
            }
            // affichage des monster
            AfficherMonster (monst, vague, &bouge);
            
            //glissage de la tower selected
            if(towerTypeSelected != 0){
                glisserTower(xMove, PIXEL_HEIGHT-yMove, towerTypeSelected);
            }

        }//FIN IF WinLose
        else{
            towers = supprimerAllTower(towers);
            monst = supprimerAllMonster(monst);
            if(WinLose == 1){//WINNER
                Mix_PlayMusic(musiqueVictoire, -1); //Joue infiniment la musique de victoire
                pause = 1;
                vague = 0;
                coins = 100;
            }
            if(WinLose == 2){//LOSER
                Mix_PlayMusic(musiqueFin, -1); //Joue infiniment la musique de défaite
                pause = 1;
                vague = 0;
                coins = 100;
            }
        }
        
        //on affiche le message
        if(*message != NULL){
            afficheMessageErreur();
            afficheTexte(textureId, 250, 610, *message, 255, 255, 255, 20);
        }
        
        //on affiche le menu
        afficherMenu(towerTypeSelected);
        
        //On affiche les descriptions correspondant au tower
        if(towerSelected == NULL){
            if(towerTypeSelected != 0){
                textureId = description(textureId, towerTypeSelected);
            }
        }else{
            textureId = descriptionTowerSelected(textureId, towerSelected);
        }
        
        //On affiche l'argent et le nombre de vague en cours
        afficheCoins(textureId, coins);
        afficheVagues(textureId, vague);
        
        // gestion de la pause
        if(pause == 1){
            if(WinLose == 1){
                //Lorsque l'on gagne
                afficherMenuWin(&pause);
                if(((int)SDL_GetTicks()/100)%2 == 0){
                    afficheTexte(textureId, 430, 30, "PLAY", 1, 1, 1, 50);
                }
            }
            if(WinLose == 2){
                //Lorsque l'on perd
                afficherMenuLose(&pause);
                if(((int)SDL_GetTicks()/100)%2 == 0){
                    afficheTexte(textureId, 430, 30, "PLAY", 1, 1, 1, 50);
                }
            }
            //afficheTexte(textureId, x, y, "TEXT", R, G, B,size);
            if(WinLose == 0){
                //Lorsque l'on est en pause
                afficherMenuPause(&pause);
                if(((int)SDL_GetTicks()/100)%2 == 0){
                    afficheTexte(textureId, 430, 30, "PLAY", 1, 1, 1, 50);
                }
            }
            if(gameStart ==1){
                //Lorsque le jeu démarre
                afficherMenuStart(&pause);
                if(((int)SDL_GetTicks()/100)%2 == 0){
                    afficheTexte(textureId, 430, 30, "PLAY", 1, 1, 1, 50);
                }
            }
        }
        
        //gestion de l'aide
        if(help == 1){
            afficherMenuHelp(textureId);
        }
        
        //on retire le curseur de base
        SDL_ShowCursor(0);
        //Affiche le curseur.
        afficherMouse(xMove, yMove, PIXEL_HEIGHT);
        
        SDL_GL_SwapBuffers();
        /* ****** */
        
        /*--------------------------------------------------------------
                    GESTION DES ÉVÉNNEMENTS LIÉES AU CLIK
         --------------------------------------------------------------*/
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }
            switch(e.type) {
                case SDL_MOUSEBUTTONDOWN:
                    *message = "Selectionner une tour";
                    xClicked = e.button.x;
                    yClicked = (PIXEL_HEIGHT-e.button.y);                    

                    //gestion creation de tower                    
                    if(pause ==  0 && yClicked<730 && towerTypeSelected != 0){
                        if(xClicked!=-1 && yClicked!=-1){
                            if(tab[730-yClicked][xClicked] == 1){
                                towers = ajouterTower(towers, &coins, xClicked, yClicked, towerTypeSelected, message);
                            }else{
                                *message = "Zone non constructible !!! ";
                            }
                        }
                    }
                    
                    //bouton menu
                    if(pause == 0 && xClicked>712 && xClicked<746 && yClicked>760 && yClicked<793){
                        Mix_PlayChannel(3, clickSelectTower, 0);
                        if(towerTypeSelected == 1){
                            towerTypeSelected = 0;
                        }else{
                            towerTypeSelected = 1;
                        }
                    }
                    if(pause == 0 && xClicked>746 && xClicked<780 && yClicked>760 && yClicked<793){
                        Mix_PlayChannel(3, clickSelectTower, 0);
                        if(towerTypeSelected == 2){
                            towerTypeSelected = 0;
                        }else{
                            towerTypeSelected = 2;
                        }
                    }
                    if(pause == 0 && xClicked>712 && xClicked<746 && yClicked>722 && yClicked<756){
                        Mix_PlayChannel(3, clickSelectTower, 0);
                        if(towerTypeSelected == 3){
                            towerTypeSelected = 0;
                        }else{
                            towerTypeSelected = 3;
                        }
                    }
                    if(pause == 0 && xClicked>746 && xClicked<780 && yClicked>722 && yClicked<756){
                        Mix_PlayChannel(3, clickSelectTower, 0);
                        if(towerTypeSelected == 4){
                            towerTypeSelected = 0;
                        }else{
                            towerTypeSelected = 4;
                        }
                    }
                    //clique sur le bouton "VENDRE"
                    if(pause == 0 && towerSelected != NULL && xClicked>845 && xClicked<905 && yClicked>645 && yClicked<660){
                        towers = supprimerTower(towers, towerSelected);
                        coins += towerSelected->cost/2;
                        towerSelected = NULL;
                        *message = "Tower destroyed";
                    }
                    
                    //clique sur le bouton "Evoluer"
                    if(pause == 0 && towerSelected != NULL && xClicked>915 && xClicked<975 && yClicked>645 && yClicked<660){
                        if(coins>=100){
                            towerSelected->puissance += towerSelected->puissance;
                            coins -= 100;
                            *message = "Tower evolved";
                        }else{
                            *message = "Manque de coins";
                        }
                        
                    }
                    
                    //bouton menu pause
                    if(pause == 1 && xClicked>400 && xClicked<600 && yClicked>100 && yClicked<160){
                        gameStart = 0;
                        printf("pause \n");
                        pause = 0;
                        WinLose = 0;
                    }
                    
                    towerSelected = selectTower(towers, xClicked, yClicked, message);
                    
                    break;
                case SDL_MOUSEBUTTONUP:
                    xClicked = -1;
                    yClicked = -1;
                    break;
                    
                case SDL_VIDEORESIZE:
                    WINDOW_WIDTH = e.resize.w;
                    WINDOW_HEIGHT = e.resize.h;
                    setVideoMode();
                    break;
                    
                /*--------------------------------------------------------------
                        BOUCLE DE GESTION DES ÉVÉNNEMENTS CLAVIER
                --------------------------------------------------------------*/
                    
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case 'q' :
                            exit(1);
                            break;
                        case SDLK_p:
                            if(pause == 1){
                                Mix_PlayChannel(5, PauseJeu, 0);
                                pause = 0;
                            }else{
                                Mix_PlayChannel(5, QuitPauseJeu, 0);
                                pause = 1;
                            }
                            gameStart = 0;
                            break;
                        case SDLK_BACKSPACE:
                            Mix_RewindMusic(); //Revient au début de la musique
                            break;
                        case SDLK_ESCAPE:
                            Mix_HaltMusic(); //Arrête la musique
                            Mix_HaltChannel(-1); //On arrête tous les canaux
                            loop = 0;
                            break;
                        default : break;
                        case 'a' :
                            if(pause == 0){
                                towerTypeSelected = 1;
                                towerSelected = NULL;
                            }
                        break;
                        case 'z' :
                            if(pause == 0){
                                towerTypeSelected = 2;
                                towerSelected = NULL;
                            }
                        break;
                        case 'e' :
                            if(pause == 0){
                                towerTypeSelected = 3;
                                towerSelected = NULL;
                            }
                        break;
                        case 'r' :
                            if(pause == 0){
                                towerTypeSelected = 4;
                                towerSelected = NULL;
                            }
                        break;
                        case 't' :
                            if(pause == 0){
                                RaySelected = 1;
                            }
                        break;
                        case 'h' :
                            if(help == 0){
                                help = 1;
                            }else{
                                help = 0;
                            }
                            break;
                        case 's' :
                            if(towerSelected != NULL){
                                towers = supprimerTower(towers, towerSelected);
                                coins += towerSelected->cost/2;
                                towerSelected = NULL;
                                *message = "Tower destroyed";
                            }
                            break;
                        case 'u' :
                            if(towerSelected != NULL){
                                if(coins>=100){
                                    *message = "Tower evolved";
                                    towerSelected->puissance += towerSelected->puissance;
                                    coins -= 100;

                                }else{
                                    *message = "Manque de coins";
                                }
                            }
                            break;
 			case 'c' :
			if(help == 1){
				if(coins<100000){
					*message = "TRIIIIICHEEEUUURRR !";
                           		coins += 10000;
			   	}else{
					*message = "C'est pas bientot fini ?!";
			   	}
			}
                            break;
                    }
                    break;
                    
                case SDL_KEYUP:
                        towerTypeSelected = 0;
                        RaySelected = 0;
                    help = 0;
                break;
                    
                default:
                    break;
            }
        }
        
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }
    
    /*--------------------------------------------------------------
                    FREE DES ALLOCATIONS MÉMOIRE
     --------------------------------------------------------------*/
    
    towers = supprimerAllTower(towers);
    if(towers == NULL){
        printf("TOWERS SUPP \n");
    }
    monst = supprimerAllMonster(monst);
    if(towers == NULL){
        printf("MONSTER SUPP \n");
    }
    ma_liste = supprimerAllNode(ma_liste);
    if(ma_liste == NULL){
        printf("NODE SUPP \n");
    }
    free(message);
   
    Mix_FreeChunk(PauseJeu);//Libération du son de la tower
    Mix_FreeChunk(QuitPauseJeu);//Libération du son de la pause
    Mix_FreeChunk(clickSelectTower);//Libération du son du clik
    Mix_FreeChunk(LevelUp);//Libération du son vague win
    
    Mix_FreeMusic(musiqueVictoire);
    Mix_FreeMusic(musiqueFin); //Libération de la musique de fin
    Mix_FreeMusic(musique); //Libération de la musique courante
    
    /*--------------------------------------------------------------
                ON FERME SDL TTF SDL MIXER ET LA SDL
     --------------------------------------------------------------*/
    
    Mix_CloseAudio(); //Fermeture de l'API audio
    TTF_Quit(); //Fermeture de sdl ttf
    SDL_Quit(); //Fermeture de la sdl
    
    return EXIT_SUCCESS;
}
