#include <memory>

#include "Bullet.h"
#include "Engine/Graphics/Renderer.h"

Bullet::Bullet(const std::shared_ptr<Texture> tex, const glm::vec3& position, const glm::vec3& size, const glm::vec2& vel, float rotation) : 
	m_Texture(tex), m_Position(position), m_Size(size), m_Velocity(vel), m_Rotation(rotation)
{

}

void Bullet::onUpdate(float deltaTime)
{
	float a = 1.0f;
	if (m_Size.x < 0.0f)
		a = -1.0f;

	m_CollisionBox = AABB(glm::vec2(m_Position.x - m_Size.x * a / 2.0f, m_Position.y - m_Size.x * a / 2.0f),
		glm::vec2(m_Position.x + m_Size.x * a / 2.0f, m_Position.y + m_Size.x * a / 2.0f));

	m_Position.x += m_Velocity.x * m_Speed * deltaTime;
	m_Position.y += m_Velocity.y * m_Speed * deltaTime;

	Renderer::drawQuad(m_Texture, { 1.0f, 1.0f, 1.0f, 1.0f }, m_Position, m_Size, m_Rotation);
}
