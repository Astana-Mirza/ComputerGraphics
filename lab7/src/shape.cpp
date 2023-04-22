#include <shape.h>
#include <common/shader_program.h>

#include <FL/Fl_PNG_Image.H>
#include <glm/gtc/matrix_inverse.hpp>

std::vector<GLuint> Shape::generate_indices()
{
	std::vector<GLuint> indices;
	for (GLuint i = 0; i < circle_count - 1; i++)
	{
		for (GLuint j = 0; j < vertices_per_circle; j++)
		{
			indices.push_back(i * vertices_per_circle + j);
			indices.push_back((i + 1) * vertices_per_circle + j);
		}
		indices.push_back(i * vertices_per_circle);
		indices.push_back((i + 1) * vertices_per_circle);
	}
	return indices;
}


void Shape::calculate_normal_matr()
{
	normal_matr = glm::inverseTranspose(glm::mat3(model_transform));
}


void Shape::generate_vertices(float a, float b, float c, float d)
{
	constexpr GLfloat circle_offset = 0.05f;
	const GLfloat pi = std::atanf(1.0f) * 4;
	const GLfloat radius_step = pi / (circle_count - 1);

	std::vector<GLfloat> circle_params;
	GLfloat t = 0.0f;
	for (GLuint i = 0; i < circle_count; i++)							// расчёт радиусов окружностей
	{
		circle_params.push_back(a * std::cos(b * t) + c * std::sin(d * t));
		t += radius_step;
	}

	std::vector<Vertex> vertices;
	GLfloat y = -circle_offset * circle_count / 2;				// создание окружностей
	for (GLuint i = 0; i < circle_count; i++)
	{
		GLfloat angle = 0.0f;
		for (GLuint j = 0; j < vertices_per_circle - 1; j++)
		{
			Vertex vertex{
				glm::vec3{ circle_params[i] * std::cos(angle), y, circle_params[i] * std::sin(angle) },
				glm::vec3{},
				glm::vec2{ static_cast<GLfloat>(j) / vertices_per_circle, static_cast<GLfloat>(i) / circle_count }
			};
			vertices.push_back(vertex);
			angle += 2 * pi / (vertices_per_circle - 1);
		}
		vertices.push_back(vertices[i * vertices_per_circle]);
		vertices.back().texture_coords.x = 1.0f;
		y += circle_offset;
	}

	for (GLuint i = 0; i < circle_count; i++)
	{
		for (GLuint j = 0; j < vertices_per_circle-1; j++)
		{
			Vertex& v = vertices[i * vertices_per_circle + j];
			v.normal = glm::normalize(glm::cross(
				vertices[(i + (i == circle_count - 1 ? -1 : 1)) * vertices_per_circle + j].position - v.position,
				vertices[i * vertices_per_circle + (j + 1) % vertices_per_circle].position - v.position
			));
		}
		vertices[(i + 1) * vertices_per_circle - 1].normal = vertices[i * vertices_per_circle].normal;
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);  // position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));  // normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));  // texture_coords
	glEnableVertexAttribArray(2);
}


void Shape::load_texture(const std::string& path)
{
	const GLubyte base_texture[3] = { 255, 255, 255 };
	glBindTexture(GL_TEXTURE_2D, texture);
	texture_path = path;
	if (path.empty())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, base_texture);
		draw_mode = GL_LINE_STRIP;
	}
	else
	{
		Fl_PNG_Image png_image(path.c_str());
		GLenum format = GL_RGB;
		switch (png_image.d())
		{
		case 4: format = GL_RGBA; break;
		case 3: format = GL_RGB; break;
		default:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, base_texture);
			texture_path = "";
			draw_mode = GL_LINE_STRIP;
			return;
		}
		draw_mode = GL_TRIANGLE_STRIP;
		glTexImage2D(GL_TEXTURE_2D, 0, format, png_image.w(), png_image.h(), 0, format, GL_UNSIGNED_BYTE, *png_image.data());
	}
	glGenerateMipmap(GL_TEXTURE_2D);	// ORDER MATTERS HERE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}


void Shape::draw(ShaderProgram& shader)
{
	shader.set_uniform("model", model_transform);
	shader.set_uniform("normal_matr", normal_matr);
	shader.set_uniform("material.diffuse", diffuse);
	shader.set_uniform("material.specular", specular);
	shader.set_uniform("material.shininess", shininess);
	shader.set_uniform("material.amb_strength", amb_strength);
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(draw_mode, index_count, GL_UNSIGNED_INT, 0);
}
