#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "OBJParser.hpp"



struct Material{

    char name[128];
    glm::vec3 ambiante;
    glm::vec3  diffuse;
    glm::vec3  speculaire;
    float nSpec;

} ;

Material mat[10];

int indice = -1;


// Very, VERY simple OBJ loader.


bool loadOBJ(const char * path,std::vector<glm::vec3> & out_vertices,std::vector<glm::vec2> & out_textures,std::vector<glm::vec3> & out_normals,std::vector<glm::vec3> & out_ambients,std::vector<glm::vec3> & out_diffuse,std::vector<glm::vec3> & out_speculaire, GLfloat  out_nSpec[])
{
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices,normalIndices,textureIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_textures;


	glm::vec3 current_ambient;
	glm::vec3 current_diffuse;
	glm::vec3 current_speculaire;
    GLfloat  current_n;
	int s=0;

	char lineHeader[128];

	char material_path[128];
    FILE * material_file;

    int is_textured =0;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){



		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);

		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.


		if ( strcmp( lineHeader, "v" ) == 0 ){

			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);

		}else if ( strcmp( lineHeader, "vn" ) == 0 ){

			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);

        // means we re using a texture

        }else if ( strcmp( lineHeader, "vt" ) == 0 ){

            is_textured = 1;

			glm::vec2 uv;

			fscanf(file, "%f %f\n", &uv.x, &uv.y );

			temp_textures.push_back(uv);


		}else if ( strcmp( lineHeader, "mtllib" ) == 0 ){

			fscanf(file, "%s\n",material_path);
            printf("material = %s\n",material_path);
            material_file = fopen(material_path, "r");

            if( material_file == NULL ){

                printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
                getchar();
                return false;
            }


            while( 1 ){

                char matLineHeader[128];
                // read the first word of the line
                int endfile = fscanf(material_file, "%s", matLineHeader);

                if (endfile == EOF)
                    break; // EOF = End Of File. Quit the loop.


                if(strcmp(matLineHeader, "newmtl" ) == 0){

                    indice++;

                    fscanf(material_file, "%s\n",mat[indice].name);


                }else if(strcmp(matLineHeader, "Ka" ) == 0){

                    glm::vec3 amb;

                    fscanf(material_file, "%f %f %f\n",&(amb).x,&(amb).y,&(amb).z);

                    mat[indice].ambiante.x= amb.x;
                    mat[indice].ambiante.y= amb.y;
                    mat[indice].ambiante.z= amb.z;


                }else if(strcmp(matLineHeader, "Kd" ) == 0){


                    glm::vec3 diff;

                    fscanf(material_file, "%f %f %f\n",&(diff).x,&(diff).y,&(diff).z);

                    mat[indice].diffuse.x = diff.x;
                    mat[indice].diffuse.y = diff.y;
                    mat[indice].diffuse.z = diff.z;

                }else if(strcmp(matLineHeader, "Ks" ) == 0){


                    glm::vec3 spec;

                    fscanf(material_file, "%f %f %f\n",&(spec).x,&(spec).y,&(spec).z);

                    mat[indice].speculaire.x = spec.x;
                    mat[indice].speculaire.y = spec.y;
                    mat[indice].speculaire.z = spec.z;

                }else if(strcmp(matLineHeader, "Ns" ) == 0){

                    GLfloat  value;


                    fscanf(material_file, "%f\n",&value);
                    mat[indice].nSpec=value;

                }

            }


        }else if ( strcmp( lineHeader, "usemtl" ) == 0 ){

             char material_name[128];

             fscanf(file, "%s\n",material_name);

             for(int i = 0 ; i< indice+1; i++){


                if(strcmp(material_name, mat[i].name ) == 0){

                    current_ambient.x = mat[i].ambiante.x;
                    current_ambient.y = mat[i].ambiante.y;
                    current_ambient.z = mat[i].ambiante.z;

                    current_diffuse.x = mat[i].diffuse.x;
                    current_diffuse.y = mat[i].diffuse.y;
                    current_diffuse.z = mat[i].diffuse.z;

                    current_speculaire.x = mat[i].speculaire.x;
                    current_speculaire.y = mat[i].speculaire.y;
                    current_speculaire.z = mat[i].speculaire.z;

                    current_n = mat[i].nSpec;

                    break;

                }
             }



        }else if ( strcmp( lineHeader, "f" ) == 0 ){

            unsigned int vertexIndex[3],  normalIndex[3],textureIndex[3];

            int matches;

            if(is_textured ==0){

                matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1],  &normalIndex[1], &vertexIndex[2],  &normalIndex[2] );


            }else{

                 matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &textureIndex[0], &normalIndex[0], &vertexIndex[1], &textureIndex[1], &normalIndex[1],  &vertexIndex[2],&textureIndex[2],  &normalIndex[2] );

            }

            if (matches != 6 && matches!=9){
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);

             if(is_textured ==1){

                textureIndices.push_back(textureIndex[0]);
                textureIndices.push_back(textureIndex[1]);
                textureIndices.push_back(textureIndex[2]);
             }

            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);

            out_ambients.push_back(current_ambient);
            out_diffuse.push_back(current_diffuse);
            out_speculaire.push_back(current_speculaire);

            //printf("spec = %f %f %f\n",current_speculaire.x,current_speculaire.y,current_speculaire.z);
            out_nSpec[s]= current_n;

            s++;


        }else{
            // Probably a comment, eat up the rest of the line
            char buffer[1000];
            fgets(buffer, 1000, file);
        }

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];

		unsigned int normalIndex = normalIndices[i];


		unsigned int textureIndex;

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];

		glm::vec3 normal = temp_normals[ normalIndex-1 ];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
        out_normals .push_back(normal);

        if(is_textured ==1){

            textureIndex = textureIndices[i];
            glm::vec2 texture = temp_textures[ textureIndex-1 ];
            out_textures.push_back(texture);
        }




	}


	std::cout<<"number of vertices: "<<vertexIndices.size()<<std::endl;

	return true;
}
