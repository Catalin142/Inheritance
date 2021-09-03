#pragma once

#include "Entity.h"
#include "Player.h"

enum class EnemyState : int
{
	IDLE,
	ATTACK,
	MELEE,
	CHASE, 
	WONDER,
};

class Enemy : public Entity
{
	friend class EntityManager;

public:
	Enemy(float x, float y, const glm::vec2& size, float rotation);

	void setBullets(const std::shared_ptr<Texture>& farFuture, const std::shared_ptr<Texture>& neanderthal, const std::shared_ptr<Texture>& ww);

	void setPlayer(const std::shared_ptr<Player>& pl) { m_Player = pl; }

	virtual void onUpdate(float deltaTime) override;

	std::shared_ptr<Player> m_Player;

private:
	float m_HitPlayerDelay = 0.1f;
	float m_Time = 0.0f;

	float m_FOV = 130.0f;

	EnemyState m_EnemyStateMachine = EnemyState::IDLE;
	AABB m_RayPoint;
	float m_Length = 0.0f;

	bool m_ClearShot = false;

	std::shared_ptr<Texture> m_FarFutureBullet;
	std::shared_ptr<Texture> m_NeanderthalSpear;
	std::shared_ptr<Texture> m_WorldWarBullet;
};