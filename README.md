C Pokimac tower defense
===================

I realised this project during my first master of science in engineering year. This project is written in C and using **OpenGL**, **GLUT** and **SDL libraries**. We used complementary libraries to expand our project : **SDL_image** (for image management), **SDL_ttf** (for text management) and **SDL_mixer** (for sound management). At start-up of the application requests the file name .itd describing the game path and card, and allowing you to use the right background image ("bottom Card"), and the right path (point connections nodes and colors), and the color of buildable areas.

----------

The game
-------------

The Tower Defense is a strategy game style, the main goal is to prevent the passage of creatures from a starting area to a finish area by creating towers defence that eliminate the creatures in their progression before they get in the zone. The waves are generally more and more difficult and the player need to find the right strategy to rich the higher level.

----------


Folder structure
-------------------


----------


How to start the game
-------------

You start the game by typing bin / itd the source of PokimacTowerDefense folder, once the executable launched please enter the name of your configuration file .itd format, three default map are available, map1.itd, map2 .itd and map3.itd.

```
 cd /FOLDER
 make clean make
 ./nameofyourexecutable
```

----------


### Keyboard shortcuts

> **Q / ESC** : Quit
**H :** Help
**P :** Pause
**A :** Tour rocket
**Z :** Tour laser
**E :** Tour mitraillette
**R :** Tour hybride
**T :** Displays the path and the ray of building areas (white circle), and the radius around a tower where we cannot build anything (black), and IN nodes (green) and OUT (red)
**U :** Upgrade the tower
**S :** Sell a tower

###Structures diagrams
