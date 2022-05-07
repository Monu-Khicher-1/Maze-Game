# Maze-Game
This is a 2 player Game. 2 Players will start from same point. The player who will reach the end point first will won. 

## Instruction for Library installation
Run following commands to install libraries on Ubuntu system:
* `$sudo apt-get install libsdl2-2.0`
* `$sudo apt-get install libsdl2-dev`
* `$sudo apt-get install libsdl2-image-dev`
* `$sudo apt-get install libsdl2-ttf-dev`
* `$sudo apt-get install libsdl2-mixer-dev`

## Instruction for Setup

Setup:
* You should have Linux as your OS for the game to run.
* First of all ensure that you had downloaded all required library.
* Now clone this github Repo to your system.
* Open terminal. Now run command `make`
* If you want to run as server then run command `./game s`
* If you want to run as client then run command `./game c <ipAddr>`
* Here <ipAddr> is IP Address of the server computer 
* If you want to play alone then run `./game`
* Your game started. Play.
  
# Reference
 * For SDL https://lazyfoo.net/tutorials/SDL/index.php
 * For UDP connection https://www.geeksforgeeks.org/udp-client-server-using-connect-c-implementation/
 

