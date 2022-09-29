# Bowling simulator
A computer graphics course's project. Implementation details can be found on the written report : [project report (in french)](rapport_projet_SI.pdf)
	
## Technologies
To run this project, the following libraries are required :
* OpenGl (minimum version 3.*)
* Glut
* Glew
* Glm

## Build & run
To build & run the project, run the following on your terminal :

```

cmake -B build 

make -C build

./build/bowlingSim 

```

## Usage


<img src="/images/bowling1.png" width=50% height=50%>


* The camera's orientation depends on the pre-defined point of view and ViewUp vector's direction. It can be controlled using the keyboard's ` arrows`.
* It is possible to zoom out and in by respectively pressing on `z` or  `a` .
* It's also possible to move the ball left and right by pressing on `q` or `s`. 
* A first left mouse click makes a moving cursor appear. 

<img src="/images/bowling2.png" width=50% height=50%>

* A second left mouse click allows to throw the ball on the direction pointed by the cursor.

<img src="/images/bowling3.png" width=50% height=50%>

* Once all the pins are down, the game automatically restarts.
