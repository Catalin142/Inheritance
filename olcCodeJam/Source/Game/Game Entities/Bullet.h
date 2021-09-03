#pragma once
#include "Engine/Graphics/Texture.h"
#include "glm/glm.hpp"
#include "Engine/Utils/AABB.h"

class Bullet
{
	friend class EntityManager;

public:
	Bullet(const std::shared_ptr<Texture> tex, const glm::vec3& position, const glm::vec3& size, const glm::vec2& vel, float rotation);

	void onUpdate(float deltaTime);
	void setSpeed(float speed) { m_Speed = speed; }
	bool isAlive() { return m_IsAlive; }

private:
	std::shared_ptr<Texture> m_Texture;
	glm::vec3 m_Position;
	glm::vec3 m_Size;
	glm::vec2 m_Velocity;

	float m_Speed;
	float m_Rotation;

	bool m_IsAlive = true;

	AABB m_CollisionBox;
};