#ifndef _CG_SHAPE_INFO
#define _CG_SHAPE_INFO

#include <common/utils.h>

#include <vertex.h>

#include <string>
#include <vector>

class ShaderProgram;

struct Shape
{
	static constexpr GLuint vertices_per_circle = 40;
	static constexpr GLuint circle_count = 40;

	std::string texture_path;
	glm::mat4 model_transform = glm::mat4(1.0f);
	glm::mat3 normal_matr = glm::mat3(1.0f);
	glm::vec3 diffuse = glm::vec3(0.5f, 0.31f, 0.28f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
	GLuint vao = 0;
	GLuint ebo = 0;
	GLuint vbo = 0;
	GLuint texture = 0;
	GLuint index_count = 0;
	GLfloat shininess = 128.0f;
	GLfloat amb_strength = 0.2f;
	GLenum draw_mode = GL_LINE_STRIP;

public:
	static std::vector<GLuint> generate_indices();

	void calculate_normal_matr();
	void generate_vertices(float a, float b, float c, float d);
	void load_texture(const std::string& path = "");
	void draw(ShaderProgram& shader);
};

#endif // #ifndef _CG_SHAPE_INFO