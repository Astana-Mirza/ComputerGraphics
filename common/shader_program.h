#ifndef _CG_SHADER_PROGRAM_H
#define _CG_SHADER_PROGRAM_H

#include <cg_config.h>

#if defined(USE_GLAD)
#   include <glad/glad.h>
#elif defined(USE_GLEW)
#    include <GL/glew.h>
#endif

#include <string>

class ShaderProgram
{
public:
	static void reset_current_shader();

	ShaderProgram();
	ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader);
	ShaderProgram(const std::string& vertex_shader,
				  const std::string& geometry_shader,
				  const std::string& fragment_shader);

	// move only
	ShaderProgram(ShaderProgram&&) noexcept;
	ShaderProgram(const ShaderProgram&) = delete;

	~ShaderProgram();

	void use() const;

	void set_uniform(const std::string& uniform_name, float value) const;

private:
	GLuint attach_shader(const std::string& shader_source, GLuint shader_type) const;

private:
	GLuint program_id_;
};

#endif // #ifndef _CG_SHADER_PROGRAM_H