#include <memory>

#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"

#include "Maths.h"

Camera::Camera(float width, float height, const glm::vec3& pos) : m_Orthographic(glm::ortho(0.0f, width, 0.0f, height)), 
m_Position(pos.x - width / 2.0f, pos.y - height / 2.0f, 0.0f),
m_Width(width), m_Height(height)
{ }

void Camera::onUpdate(const std::shared_ptr<Player>& player, float deltaTime)
{
	glm::vec3 desiredPosition = player->getPosition() - glm::vec3(m_Width / 2.0f, m_Height / 2.0f, 0.0f);
	m_Position = moveTowards(m_Position, desiredPosition, m_Speed * deltaTime);
}

glm::mat4 Camera::getMatrix()
{
	return m_Orthographic * glm::translate(glm::mat4(1.0f), -m_Position);
}