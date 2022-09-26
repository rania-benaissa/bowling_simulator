#include "solid.hpp"
#include "objParser.hpp"


Solid::Solid(char path[])
{

    loadOBJ(path,positions,textures, normals,ambiantes,diffuses,speculaires,nSpeculaires);

    Model = mat4(1.0f);

    if(strcmp(path, "Pin.obj") == 0){

        Model = translate(Model,vec3(0.0f, 0.01f, 7.0f));
        Model = scale(Model,vec3(0.1f, 0.1f ,0.1f));


    }
}

Solid::Solid()
{


}

void Solid::initSolid(){



    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    //1st attribute -> vect Position
    glGenBuffers(1, &VBOPosition);
    glBindBuffer(GL_ARRAY_BUFFER, VBOPosition);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0 );

    // 2nd attribute-> vect NORMAL
    glGenBuffers(1, &VBONormal);
    glBindBuffer(GL_ARRAY_BUFFER, VBONormal);
    glBufferData(GL_ARRAY_BUFFER,normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,3 * sizeof(GLfloat), (void*)0 );

    if(!textures.empty()){

         // 3nd attribute-> vect TEXTURES
        glGenBuffers(1, &VBOTexture);
        glBindBuffer(GL_ARRAY_BUFFER, VBOTexture);
        glBufferData(GL_ARRAY_BUFFER,textures.size() * sizeof(vec2), &textures[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,2 * sizeof(GLfloat), (void*)0 );
    }



     glBindVertexArray(0);


}


void Solid::initTexture(char fileName[])
{

     char data[128*128*3];

    FILE *f = fopen(fileName, "rb");

    if(f)
    {
        fread(data, 128*128*3, 1, f);
        fclose(f);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    }else{

        printf("couldn't read file");

    }


}


void Solid::drawSolid(GLuint IDs []){

       //on envoie la matrice model (position de l'objet dans le monde reel)
        glUniformMatrix4fv(IDs[0], 1, GL_FALSE, &Model[0][0]);

        if(!textures.empty()){

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(IDs[5], 0);
            glUniform1f(IDs[6],1.0f);

        }else{

            glUniform1f(IDs[6],0.0f);
        }


        glBindVertexArray(VAO);


    for(int i =0 ; i <  int(ambiantes.size());i++){

              //on envoie la couleur ambiante
            glUniform3fv(IDs[1], 1, &ambiantes[i][0]);

            glUniform3fv(IDs[2], 1, &diffuses[i][0]);

            glUniform3fv(IDs[3], 1, &speculaires[i][0]);

            glUniform1f(IDs[4],nSpeculaires[i]);

            // dessin
            glDrawArrays(GL_TRIANGLES , i*3,3);

    }


 	glBindVertexArray(0);


}

void Solid::cleanDrawing(){


    glDeleteBuffers(1, &VBONormal);
    glDeleteBuffers(1, &VBOPosition);

    if(!textures.empty()){

        glDeleteBuffers(1, &VBOTexture);
    }

	glDeleteVertexArrays(1, &VAO);

}



