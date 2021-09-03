#pragma once
#include "Entity.h"

class Player : public Entity
{
	friend class EntityManager;

public:
	Player(float x, float y, const glm::vec2& size);

	virtual void onUpdate(float deltaTime) override;
	void setPosition(const glm::vec3& pos) { m_Position = pos; }

private:
	bool m_CanWalk = true;
	AABB m_CollisionPoint;
	glm::vec3 m_lastPosition;

	std::shared_ptr<Texture> m_FarFutureBullet;
	std::shared_ptr<Texture> m_NeanderthalSpear;
	std::shared_ptr<Texture> m_WorldWarBullet;
};