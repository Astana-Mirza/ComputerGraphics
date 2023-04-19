#include <shape.h>

#include <FL/Fl_PNG_Image.H>

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


void Shape::generate_vertices(float a, float b, float c, float d)
{
	constexpr GLfloat pi = 3.14159265f;
	constexpr GLfloat circle_offset = 0.05f;
	constexpr GLfloat radius_step = pi / circle_count;

	std::vector<GLfloat> circle_params;
	GLfloat t = 0;
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
		for (GLuint j = 0; j < vertices_per_circle; j++)
		{
			Vertex vertex{
				glm::vec3{ circle_params[i] * std::cos(angle), y, circle_params[i] * std::sin(angle) },
				glm::vec3{/* normal */},
				glm::vec2{ static_cast<GLfloat>(j) / vertices_per_circle, static_cast<GLfloat>(i) / circle_count }
			};
			vertices.push_back(vertex);
			angle += 2 * pi / vertices_per_circle;
		}
		y += circle_offset;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}


void Shape::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(draw_mode, index_count, GL_UNSIGNED_INT, 0);
}
