C/OpenGL programming Pokimac tower defense for mac os x
===================

![Screenshots of the game](http://s24.postimg.org/4h1r6hg39/example.jpg)


I realised this project during my first year of master of science in engineering with **[Maxime Blondeau](http://maximeblondeau.com/)**. This project is written in C and using **OpenGL**, **GLUT** and **SDL libraries**. We used complementary libraries to expand our project : **SDL_image** (for image management), **SDL_ttf** (for text management) and **SDL_mixer** (for sound management). At start-up of the application requests the file name .itd describing the game path and card, and allowing you to use the right background image ("bottom Card"), and the right path (point connections nodes and colors), and the color of buildable areas.



The game
-------------

The Tower Defense is a strategy game style, the main goal is to prevent the passage of creatures from a starting area to a finish area by creating towers defence that eliminate the creatures in their progression before they get in the zone. The waves are generally more and more difficult and the player need to find the right strategy to rich the higher level.




Files structure
-------------------

```
.
├── Makefile
├── README.md
├── SDLMain.h
├── SDLMain.m
├── bin
│   └── itd
├── data
│   ├── map1.itd
│   ├── map2.itd
│   └── map3.itd
├── doc
│   └── aide.txt
├── images
│   ├── Acme.ttf
│   ├── Fipps.ttf
│   ├── HARABARA.TTF
│   ├── hoog0656.ttf
│   ├── map.jpg
│   ├── map2.jpg
│   ├── map3.jpg
│   ├── monsters
│   │   ├── autreFille.png
│   │   ├── autreMec.png
│   │   ├── autreMec2.png
│   │   ├── autreSprite.png
│   │   ├── eau.png
│   │   ├── fille2.png
│   │   ├── pikachu.png
│   │   ├── pokemon.png
│   │   └── pokemonMechant.png
│   ├── scenario
│   │   ├── AIDE.png
│   │   ├── barmenu.png
│   │   ├── hand_cursor2.png
│   │   ├── pause.png
│   │   ├── start.jpg
│   │   ├── youloose.png
│   │   └── youwin.png
│   └── towers
│       └── pokeballs.png
├── include
│   ├── carte.h
│   ├── chemin.h
│   ├── map.h
│   ├── menu.h
│   ├── monster.h
│   ├── node.h
│   ├── sdl.h
│   ├── texte.h
│   ├── texture.h
│   ├── tower.h
│   └── vague.h
├── libs
│   ├── SDL.framework
│   ├── SDL_image.framework
│   ├── SDL_mixer.framework
│   └── SDL_ttf.framework
├── morganVilledieu&maximeBlondeau.pdf
├── obj
│   ├── carte.o
│   ├── chemin.o
│   ├── main.o
│   ├── map.o
│   ├── menu.o
│   ├── monster.o
│   ├── node.o
│   ├── sdl.o
│   ├── texte.o
│   ├── texture.o
│   ├── tower.o
│   └── vague.o
├── son
│   ├── LevelUp.ogg
│   ├── Notification.ogg
│   ├── Opening.mid
│   ├── PP\ Zero.ogg
│   ├── Warp.ogg
│   ├── WarpQuit.ogg
│   ├── defaite.mid
│   ├── magic100.ogg
│   └── pokerock.mid
└── src
    ├── carte.c
    ├── chemin.c
    ├── main.c
    ├── map.c
    ├── menu.c
    ├── monster.c
    ├── node.c
    ├── sdl.c
    ├── texte.c
    ├── texture.c
    ├── tower.c
    └── vague.c
```



How to start the game
-------------

You start the game by typing bin/itd the source of PokimacTowerDefense folder, once the executable launched please enter the name of your configuration file .itd format, three default map are available, map1.itd, map2 .itd and map3.itd.

```sh
 cd /FOLDER
 sudo cp -r libs/* /Library/Frameworks/
 make clean
 make
 bin/itd (to start the program)

 -- then write the name of the mapfile
```



### Keyboard shortcuts

 - **Q / ESC** : Quit
 -  **H :** Help
 -   **P :** Pause
 -   **A :** Tour rocket
 -    **Z :** Tour laser
 -    **E :** Tour mitraillette
 -    **R :** Tour hybride
 -    **T :** Displays the path and the ray of building areas (white circle), and the radius around a tower where we cannot build anything
   (black), and IN nodes (green) and OUT (red)
 -    **U :** Upgrade the tower
 -    **S :** Sell a tower

###How the maps are working

![map draw](http://s1.postimg.org/lahvoq5rh/Screenshot_2015_07_18_20_00_05.png)


An example of a mapfile that I created using pokemon sprites with drawable zones in pink :

![enter image description here](http://s17.postimg.org/bjjtc3gkt/map2.jpg)

###Characters movement

![mouvement du perso](http://s1.postimg.org/u446s33ce/Screenshot_2015_07_18_20_00_13.jpg)

###Sprites

We used pokemon sprites matrix for the game that we texturised on a OpenGL 2d rect shape.

![Sprites](http://s4.postimg.org/ndxbu3gul/pikachu.png)

###For more information

If you need some more information or if you've got any question regarding the game feel free to contact me and to read the pdf file explaining exactly the work that we've done, unfortunately this document is in french but it's composed by some good images explaining the way we build this game.

Thanks, hope you'll enjoy it.
