#include <vector>
#include <iostream>
#ifndef OBJLOAD_H
#define OBJLOAD_H

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_textures,
	std::vector<glm::vec3> & out_normals,
	std::vector<glm::vec3> & out_ambients,
	std::vector<glm::vec3> & out_diffuse,
	std::vector<glm::vec3> & out_speculaire,
	float out_nSpec[]
);


#endif
