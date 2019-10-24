
#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <glm-0.9.6.3/glm/glm.hpp>
#include <vector>
bool loadOBJ(
	const char* path,
	std::vector<glm::vec3> &out_vertices,
	std::vector<glm::vec3> &out_normals
	
);



#endif
