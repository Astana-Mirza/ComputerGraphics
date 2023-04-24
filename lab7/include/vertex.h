#ifndef _CG_VERTEX_H
#define _CG_VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coords;
};

#endif // #ifndef _CG_VERTEX_H