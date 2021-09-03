#pragma once

#include "Engine/Utils/AABB.h"
#include "glm/glm.hpp"

struct Wall
{
	glm::vec3 m_Position;
	glm::vec3 m_Scale;

	AABB m_CollisionBox;

	void setCollisionBox()
	{
		m_CollisionBox = AABB(glm::vec2(m_Position.x - m_Scale.x / 2.0f, m_Position.y - m_Scale.y / 2.0f),
			glm::vec2(m_Position.x + m_Scale.x / 2.0f, m_Position.y + m_Scale.y / 2.0f));
	}
};