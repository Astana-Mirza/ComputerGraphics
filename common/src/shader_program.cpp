#include <common/shader_program.h>

#include <utility> // std::swap
#include <stdexcept> // std::runtime_error

void ShaderProgram::reset_current_shader()
{
	glUseProgram(0);
}


ShaderProgram::ShaderProgram() : program_id_{ 0 } {}


ShaderProgram::ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader) :
	program_id_{ 0 }
{
	program_id_ = glCreateProgram();
	GLuint vs_id = attach_shader(vertex_shader, GL_VERTEX_SHADER);
	GLuint fs_id = attach_shader(fragment_shader, GL_FRAGMENT_SHADER);
	glLinkProgram(program_id_);

	glDeleteShader(vs_id);
	glDeleteShader(fs_id);
}


ShaderProgram::ShaderProgram(const std::string& vertex_shader,
							 const std::string& geometry_shader,
							 const std::string& fragment_shader):
	program_id_{ 0 }
{
	program_id_ = glCreateProgram();
	GLuint vs_id = attach_shader(vertex_shader, GL_VERTEX_SHADER);
	GLuint gs_id = attach_shader(geometry_shader, GL_GEOMETRY_SHADER);
	GLuint fs_id = attach_shader(fragment_shader, GL_FRAGMENT_SHADER);
	glLinkProgram(program_id_);

	glDeleteShader(vs_id);
	glDeleteShader(gs_id);
	glDeleteShader(fs_id);
}


ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept:
	program_id_{ 0 }
{
	std::swap(other.program_id_, program_id_);
}


ShaderProgram::~ShaderProgram()
{
	if (program_id_)
	{
		glDeleteProgram(program_id_);
	}
}


void ShaderProgram::use() const
{
	if (program_id_)
	{
		glUseProgram(program_id_);
	}
}


void ShaderProgram::set_uniform(const std::string& uniform_name, float value) const
{
	GLuint location = glGetUniformLocation(program_id_, uniform_name.c_str());
	if (location == -1)
	{
		throw std::runtime_error{ "cannot find uniform: " + uniform_name };
	}
	glUniform1f(location, value);
}


GLuint ShaderProgram::attach_shader(const std::string& shader_source, GLuint shader_type) const
{
	GLuint shader_id = glCreateShader(shader_type);
	const char *source_c_str = shader_source.c_str();
	glShaderSource(shader_id, 1, &source_c_str, nullptr);
	glCompileShader(shader_id);
	glAttachShader(program_id_, shader_id);
	return shader_id;
}
