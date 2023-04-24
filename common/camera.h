#ifndef _CG_CAMERA_H
#define _CG_CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::vec3& position = glm::vec3(),
		   const glm::vec3& rotation = glm::vec3());

	glm::mat4 get_look_at_matr() const;
	const glm::vec3& get_position() const;
	const glm::vec3& get_rotation() const;
	const glm::vec3& get_direction() const;
	const glm::vec3& get_up() const;
	const glm::vec3& get_right() const;

	void move(const glm::vec3& offset);
	void rotate(const glm::vec3& offset);

	void set_position(const glm::vec3& position);
	void set_rotation(const glm::vec3& rotation);

private:
	void recalculate_direction();

private:
	glm::vec3 position_;
	glm::vec3 rotation_;
	glm::vec3 direction_;
	glm::vec3 up_;
	glm::vec3 right_;
};

#endif // #ifndef _CG_CAMERA_H