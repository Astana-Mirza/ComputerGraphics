#ifndef _CG_LIGHT_H
#define _CG_LIGHT_H

#include <glm/glm.hpp>

class ShaderProgram;

struct Light
{
	glm::vec4 position = glm::vec4(0.4f, 0.2f, 0.3f, 0.0f);
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 atten_params = glm::vec3(1.0f, 0.007f, 0.0002f);
	glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);
	float cut_off = 0.0f;
	float outer_cut_off = 0.0f;

	void apply_settings(const ShaderProgram& shader) const;
};

#endif // #ifndef _CG_LIGHT_H