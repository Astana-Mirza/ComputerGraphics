#include <common/camera.h>

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation):
	position_{ position }, rotation_{ rotation },
	direction_{ 0.0f, 0.0f, -1.0f }, up_{ 0.0f, 1.0f, 0.0f },
	right_{ 1.0f, 0.0f, 0.0f }
{
	recalculate_direction();
}


glm::mat4 Camera::get_look_at_matr() const
{
	return glm::lookAt(position_, position_ + direction_, up_);
}


const glm::vec3& Camera::get_position() const
{
	return position_;
}


const glm::vec3& Camera::get_rotation() const
{
	return rotation_;
}


const glm::vec3& Camera::get_direction() const
{
	return direction_;
}


const glm::vec3& Camera::get_up() const
{
	return up_;
}


const glm::vec3& Camera::get_right() const
{
	return right_;
}


void Camera::move(const glm::vec3& offset)
{
	position_ += offset;
}


void Camera::rotate(const glm::vec3& offset)
{
	rotation_ += offset;
	recalculate_direction();
}


void Camera::set_position(const glm::vec3& position)
{
	position_ = position;
}


void Camera::set_rotation(const glm::vec3& rotation)
{
	rotation_ = rotation;
	recalculate_direction();
}


void Camera::recalculate_direction()
{
	const float pi = std::atanf(1) * 4;
	rotation_.x = std::clamp(rotation_.x, -pi / 2 + 0.001f, pi / 2 - 0.001f);

	if (rotation_.y > 2 * pi)
	{
		rotation_.y -= 2 * pi;
	}
	else if (rotation_.y < -2 * pi)
	{
		rotation_.y += 2 * pi;
	}

	direction_.x = std::cos(rotation_.y) * std::cos(rotation_.x);
	direction_.y = std::sin(rotation_.x);
	direction_.z = std::sin(rotation_.y) * std::cos(rotation_.x);
	direction_ = glm::normalize(direction_);

	right_ = glm::normalize(glm::cross(direction_, glm::vec3{ 0.0f, 1.0f, 0.0f }));
	up_ = glm::normalize(glm::cross(right_, direction_));
}
