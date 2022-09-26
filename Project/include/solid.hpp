#ifndef SOLID_H
#define SOLID_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstddef>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>

using namespace std;
using namespace glm;

#define GLUT_DISABLE_ATEXIT_HACK

class Solid
{
    public:

        std::vector<vec3>  positions;

        std::vector<vec3>  normals;

        std::vector<vec2>  textures;

        std::vector<vec3>  ambiantes;

        std::vector<vec3>  diffuses;

        std::vector<vec3>  speculaires;

        GLfloat nSpeculaires[1000];



        mat4 Model;

        GLuint VBOPosition;
        GLuint VBONormal;
        GLuint VBOTexture;
        GLuint VAO;

        // num de texture
        GLuint texture;

        Solid(char const *path);
        Solid();

        void drawSolid(GLuint IDs []);

        void initTexture(char const* fileName);

        void initSolid();

        void cleanDrawing();


    protected:

    private:
};

#endif // SOLID_H
