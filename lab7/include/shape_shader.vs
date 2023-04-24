const std::string vertex_shader = R"(
	#version 330 core

	layout (location = 0) in vec3 in_pos;
	layout (location = 1) in vec3 in_normal;
	layout (location = 2) in vec2 in_texture_pos;

	out vec3 normal;
	out vec3 fragment_pos;
	out vec2 texture_pos;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	uniform mat3 normal_matr;

	void main()
	{
		gl_Position = projection * view * model * vec4(in_pos, 1.0);
		fragment_pos = vec3(model * vec4(in_pos, 1.0));
		normal = normalize(normal_matr * in_normal);
		texture_pos = in_texture_pos;
	}
)";
