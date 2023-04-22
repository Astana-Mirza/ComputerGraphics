#include <light.h>

#include <common/shader_program.h>

void Light::apply_settings(const ShaderProgram& shader) const
{
	shader.set_uniform("light.position", position);
	shader.set_uniform("light.ambient", ambient);
	shader.set_uniform("light.diffuse", diffuse);
	shader.set_uniform("light.specular", specular);
	shader.set_uniform("light.atten_params", atten_params);
	shader.set_uniform("light.direction", direction);
	shader.set_uniform("light.cut_off", cut_off);
	shader.set_uniform("light.outer_cut_off", outer_cut_off);
}