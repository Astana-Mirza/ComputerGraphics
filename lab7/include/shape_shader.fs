const std::string fragment_shader = R"(
	#version 330 core

	struct Material
	{
		vec3 diffuse;
		vec3 specular;
		float shininess;
		float amb_strength;
	};

	struct Light
	{
		vec4 position;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
		vec3 atten_params;
		vec3 direction;
		float cut_off;
		float outer_cut_off;
	};

	in vec3 normal;
	in vec3 fragment_pos;
	in vec2 texture_pos;

	out vec4 out_color;

	uniform Material material;
	uniform Light light;
	uniform vec3 camera_pos;
	uniform sampler2D tex;

	void main()
	{
		float attenuation = 1.0;
		vec3 ambient = light.ambient * material.diffuse * material.amb_strength;

		vec3 light_dir = normalize(-light.direction);;
		if (light.position.w > 0.0)
		{
			light_dir = normalize(vec3(light.position) - fragment_pos);
		}
		vec3 diffuse = light.diffuse * max(dot(normal, light_dir), 0.0) * material.diffuse;

		vec3 view_dir = normalize(camera_pos - fragment_pos);

		// Blinn-Phong
		vec3 halfway_dir = normalize(light_dir + camera_pos);
		float spec = pow(max(dot(normal, halfway_dir), 0.0), material.shininess);

		vec3 specular = light.specular * spec * material.specular;

		if (light.position.w > 0.0)
		{
			if (light.cut_off > 0.0)
			{
				float theta = dot(light_dir, normalize(-light.direction));
				float epsilon = (light.cut_off - light.outer_cut_off);
				float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
				diffuse  *= intensity;
				specular *= intensity;
			}

			float distance = length(vec3(light.position) - fragment_pos);
			attenuation = 1.0 / (light.atten_params.x
								+ light.atten_params.y * distance
								+ light.atten_params.z * (distance * distance));
		}

	    out_color = texture(tex, texture_pos) * vec4((ambient + diffuse + specular) * attenuation, 1.0);
	}
)";
