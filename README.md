# Bowling simulator
Computer graphics course's project at USTHB, implementation details can be found on the written report  
	
## Technologies
To run this project, the following libraries are required :
* OpenGl (minimum version 3.*)
* Glut
* Glew
	
## Build & run
To build & run the project, run the following on your terminal :

```

cmake -B build 

make -C build

./build/bowlingSim # to run 

```

## Usage

* The camera's orientation depends on the pre-defined point of view and ViewUp vector's direction. It can be controlled using the keyboard's ` arrows`.
* It is possible to zoom out and in by respectively pressing on `z` or  `a` .
* It's also possible to move the ball left and right by pressing on `q` or `s`. 
* A first left mouse click makes a moving cursor appear. A second left mouse click allows to throw the ball on the direction pointed by the cursor.
* Once all the pins are down, the game automatically restarts.
