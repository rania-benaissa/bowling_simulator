#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstddef>
#include <iostream>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <shader.hpp>
#include "OBJParser.hpp"
#include <math.h>
#include "Solid.h"

using namespace std;
using namespace glm;

#define GLUT_DISABLE_ATEXIT_HACK

mat4 MVP,Projection,View,track_model_arrow,track_model_ball;

int choosingDirection = 0;

float value = 1.0f, theta = 0.0f;

float zTranslations = 0.0f;

// garde un historique des modeles des pins avant de lancer la balle
vector<mat4> pins_models_hist;

// ids du programme
GLuint programID;
GLuint MatrixID;
GLuint LightPosID;
GLuint CamPosID;

//ids des objs

GLuint ModelID;
GLuint LightAID;
GLuint LightDID;
GLuint LightSID;
GLuint LightNID;
GLuint isTexturedID;
GLuint TextureID;

GLuint IDs [7];

// MY OBJECTS
Solid pins[10];
int alreadyDown[10]= {0,0,0,0,0,0,0,0,0,0};
Solid plane("plane.obj");
Solid ball("ball.obj");
Solid arrow("arrow.obj");

// position de la source de lumiere
vec3 lightPos(0.0f,1.0f,10.0f);

// position de la camera
vec3 camPos(0.0f,0.3f,10.2f);

// direction de la camera
vec3 camDirection(0,0,8.0f);

vec3 camUp(0,1,0);


void createPins(){


    for(int i = 0 ;i < 10 ; i++){

        Solid pin("Pin.obj");

        pin.initSolid();


        switch (i){

            case 1 : {pin.Model = translate(pin.Model,vec3(-2.0f, 0.0f, -3.0f));}break;
            case 2 : {pin.Model = translate(pin.Model,vec3(2.0f, 0.0f, -3.0f));}break;
            case 3 : {pin.Model = translate(pin.Model,vec3(-3.5f, 0.0f, -6.0f));}break;
            case 4 : {pin.Model = translate(pin.Model,vec3(0.0f, 0.0f, -6.0f));}break;
            case 5 : {pin.Model = translate(pin.Model,vec3(3.5f, 0.0f, -6.0f));}break;
            case 6 : {pin.Model = translate(pin.Model,vec3(-4.0f, 0.0f,-9.0f));}break;
            case 7 : {pin.Model = translate(pin.Model,vec3(-1.5f, 0.0f,-9.0f));}break;
            case 8 : {pin.Model = translate(pin.Model,vec3(1.5f, 0.0f, -9.0f));}break;
            case 9 : {pin.Model = translate(pin.Model,vec3(4.0f, 0.0f, -9.0f));}break;

        }

         pins_models_hist.push_back(pin.Model);
         pins[i] = pin;

    }

}


void drawPins(){


     for(int i = 0 ;i < 10 ; i++){

        MVP = Projection * View * pins[i].Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        pins[i].drawSolid(IDs);
    }


}


void animatePins(float ballXDirection,int hit[10]){



    float pinX =0.0f;
    float axisZ = 0.0f,axisX = 0.0f;
    srand(time(nullptr));

    int FLOAT_MIN = -1;
    int FLOAT_MAX = 1;

    for(int i = 0 ;i < 10 ; i++){

            pinX = pins[i].Model[3][0];


            if( pinX * ballXDirection>= 0 && abs(abs(pinX)-abs(ballXDirection)) <1.0 && alreadyDown[i] == 0 ){

                axisZ = FLOAT_MIN + (float)(rand()) / ((float)(RAND_MAX/(FLOAT_MAX - FLOAT_MIN)));
                axisX = FLOAT_MIN+1 + (float)(rand()) / ((float)(RAND_MAX/(FLOAT_MAX - FLOAT_MIN+1)));

                hit[i] = 1;
                pins[i].Model = rotate(pins[i].Model,radians(-6.0f),vec3(axisX,0.0f,axisZ));

                pins[i].Model = translate(pins[i].Model,vec3(0.0f,0.07f,0.0f));
            }

        }




}

void cleanPins(){

     for(int i = 0 ;i < 10 ; i++){

        pins[i].cleanDrawing();
    }

}

int pinsAreDown(){

     for(int i = 0 ;i < 10 ; i++){

        if(alreadyDown[i]==0){

            return 0 ;
        }
    }

    return 1;

}

void initPinsState(){

     for(int i = 0 ;i < 10 ; i++){

        alreadyDown[i] = 0;

        pins[i].Model =   pins_models_hist[i];
    }


}

void ExitFunction(int value){
	cout<<"Exit called."<<endl;
	// Cleanup VBO

    plane.cleanDrawing();
    ball.cleanDrawing();
    cleanPins();

}

void MouseClick (int button,int state,int x, int y){

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){

         switch(choosingDirection){

            case 0 : choosingDirection = 1; break;

            case 1 : choosingDirection = 2; break;


         }
    }

    glutPostRedisplay();
}

void SpecialKeys (int touche, int x, int y){

    vec3 forwardVector = normalize(camDirection - camPos);


    float camSpeed = 0.1f;

    switch (touche)
    {


        //moving left
         case GLUT_KEY_LEFT:{

            vec3 leftVector = cross(camUp,forwardVector);

            camPos += camSpeed * leftVector;

        }break;


          //moving right

         case GLUT_KEY_RIGHT:{
            vec3 rightVector = cross(forwardVector,camUp);
            camPos += camSpeed * rightVector;

        }break;



          //moving up down
         case GLUT_KEY_UP:{


            camPos += camSpeed * camUp;

        }break;

         case GLUT_KEY_DOWN :{
            camPos -= camSpeed * camUp;

        }break;


    }


    glutPostRedisplay();
}

void KeyPressed (unsigned char touche, int x, int y){

    vec3 forwardVector = normalize(camDirection - camPos);


    float speed = 0.1f;

    float speedArrow = 0.07f;

    switch (touche)
    {

        // ZOOM IN / OUT
        case 'a':{

            camPos += speed * forwardVector;
            camDirection += speed * forwardVector;

        }break;


         case 'z':{

            camPos -= speed * forwardVector;
            camDirection -= speed * forwardVector;

        }break;


          // move the ball to the left or right
        case 'q':{

          ball.Model  = translate(ball.Model,vec3(-speed,0.0f ,0.0f));
        //  arrow.Model  = translate(arrow.Model,vec3(-speedArrow,0.0f ,0.0f));

        }break;


         case 's':{

            ball.Model  = translate(ball.Model,vec3(speed,0.0f,0.0f));
           // arrow.Model  = translate(arrow.Model,vec3(speedArrow,0.0f ,0.0f));

        }break;
    }



    glutPostRedisplay();


}

void reInitBallPosition(){

    choosingDirection = 0;
    theta = 0.0f;
    value = 1.0f;
    arrow.Model = track_model_arrow;
    ball.Model = track_model_ball;
    zTranslations = 0.0f;

    if(pinsAreDown() == 1){


        initPinsState();

    }

}

void copyArray(int tab1[],int tab2[]){


  for(int i = 0 ;i < 10 ; i++){

        if( tab2[i]== 1)

            tab1[i] = tab2[i];
    }


}

void animateBall(void){

    int hit[10] = {0,0,0,0,0,0,0,0,0,0};

    float speed =0.1f,ang = 2.0f;

    //forward vect de l'objet
    vec3 normal(0.0f,0.0f, -10.0f);


    float translateX = -(normal.length() * tan(radians(theta))) ;

    if(translateX<0){

        ang = -ang;

    }

    // make the ball rotate around itself

     ball.Model  = rotate(ball.Model,radians(ang),vec3(translateX* speed,0.0f,normal.z*speed));

     ball.Model  = translate(ball.Model, vec3(translateX* speed,0.0f,normal.z*speed));




     zTranslations += normal.z*speed;

     if(zTranslations <= -25.0f){

        animatePins(translateX,hit);

     }

     if(zTranslations <= -40.0f){


        Sleep(200);
        copyArray(alreadyDown,hit);
        reInitBallPosition();

     }


}

void animateArrow(void){


    if(choosingDirection == 1){


           theta += value;

            if(theta >= 90.f || theta <=-90.0f ){

                value = -value;
            }


        arrow.Model = rotate(arrow.Model,radians(value),vec3(0.0f,1.0f,0.0f));

        glutPostRedisplay();
    }

     if(choosingDirection == 2){

            animateBall();
            glutPostRedisplay();
        }


}

void affichage(){


    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);

    glUniform3fv(LightPosID, 1, &lightPos[0]);

    // first argument my position, second argument -> vers ou je vois, third arg :vect view up
    View = lookAt(camPos,camDirection , camUp );

    //DRAW PLANE
    MVP = Projection * View * plane.Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    plane.drawSolid(IDs);


    //DRAW BALL

    MVP = Projection * View * ball.Model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    ball.drawSolid(IDs);


    // DRAW PINS
    drawPins();


    if(choosingDirection ==1){


        //DRAW ARROW
        MVP = Projection * View * arrow.Model;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        arrow.drawSolid(IDs);

    }



    glutSwapBuffers(); //afficher


}



int main(int argc, char** argv)
{
    // INITIALISATIONS

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);

    glutInitWindowSize(720, 720);

    glutInitWindowPosition(100, 100);

    glutInitContextVersion(3, 3);

    glutInitContextFlags(GLUT_CORE_PROFILE);

    __glutCreateWindowWithExit("TP OpenGL", ExitFunction);

    glewExperimental = TRUE;

    glewInit();

    glEnable(GL_DEPTH_TEST);


    // EVENEMENTS
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(KeyPressed);
    glutMouseFunc(MouseClick);
    glutIdleFunc(animateArrow);

    // INIT LES OBJETS (LIER VBO+ VAO)
    createPins();
    ball.initSolid();
    plane.initSolid();
    arrow.initSolid();
    plane.initTexture("wood.raw");


    //TRANSFORMER LES OBJETS
    ball.Model  = translate(ball.Model,vec3(0.0f,0.1f ,9.5f));
    ball.Model  = scale(ball.Model,vec3(0.1f, 0.1f ,0.1f));
    track_model_ball = ball.Model;



    plane.Model = rotate(plane.Model,radians(90.0f),vec3(0.0f,1.0f,0.0f));


    arrow.Model  = translate(arrow.Model,vec3(0.0f,0.1f ,9.2f));
    arrow.Model  = scale(arrow.Model,vec3(0.1f, 0.1f ,0.1f));
    track_model_arrow = arrow.Model;

    Projection = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);



    programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

    // A ENVOYER AU SHADER

    MatrixID = glGetUniformLocation(programID, "MVP");

    ModelID = glGetUniformLocation(programID, "Model");

    LightAID = glGetUniformLocation(programID, "lightA");

    LightDID = glGetUniformLocation(programID, "lightD");

    LightSID = glGetUniformLocation(programID, "lightS");

    LightNID = glGetUniformLocation(programID, "lightN");

    LightPosID = glGetUniformLocation(programID, "lightPos");

    CamPosID = glGetUniformLocation(programID, "camPos");

    TextureID = glGetUniformLocation(programID, "myTexture");

    isTexturedID = glGetUniformLocation(programID, "isTextured");

     IDs[0] = ModelID;
     IDs[1] = LightAID;
     IDs[2] = LightDID;
     IDs[3] = LightSID;
     IDs[4] = LightNID;
     IDs[5] = TextureID;
     IDs[6] = isTexturedID;


    glutDisplayFunc(affichage);


    glClearColor(0.431, 0.164, 0.188, 0.0f);
    glClearDepth( 1.0f );
    glDepthFunc(GL_LESS);



    glutMainLoop() ;

    return(0) ;
}
