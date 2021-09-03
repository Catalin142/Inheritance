#include "Enemy.h"
#include <iostream>
#include "Engine/Utils/Maths.h"
#include "Engine/Graphics/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Map.h"
#include "Bullet.h"
#include "EntityManager.h"

Enemy::Enemy(float x, float y, const glm::vec2& size, float rotation) : Entity(x, y, size)
{ 
	m_CollisionBox = AABB(glm::vec2(m_Position.x - m_Scale.x / 2.0f, m_Position.y - m_Scale.y / 2.0f),
		glm::vec2(m_Position.x + m_Scale.x / 2.0f, m_Position.y + m_Scale.y / 2.0f));

	m_Speed = 270.0f;
	m_AttackCollisionBox.m_IsActive = false;

	m_DefaultPosition = m_Position;
	m_Rotation = rotation;
}

void Enemy::setBullets(const std::shared_ptr<Texture>& farFuture, const std::shared_ptr<Texture>& neanderthal, const std::shared_ptr<Texture>& ww)
{
	m_FarFutureBullet = farFuture;
	m_NeanderthalSpear = neanderthal;
	m_WorldWarBullet = ww;
}

void Enemy::onUpdate(float deltaTime)
{
	bool canShoot = false;
	m_Melee = true;

	if (m_WorldState == WorldState::Far_Future || m_WorldState == WorldState::Neanderthal || m_WorldState == WorldState::World_War1)
	{
		canShoot = true;

		if (m_WorldState == WorldState::Far_Future)
		{
			m_ShotCooldown = 0.3f;
			m_BulletSpeed = 450.0f;
			m_Melee = false;
			m_BulletSize = { 15.0f, 20.0f, 1.0f };
			m_xOffset = 20.0f;
		}

		else if (m_WorldState == WorldState::Neanderthal)
		{
			m_ShotCooldown = 0.8f;
			m_BulletSpeed = 450.0f;
			m_BulletSize = { 16.0f, 60.0f, 1.0f };
			m_xOffset = 30.0f;
		}

		else
		{
			m_ShotCooldown = 0.4f;
			m_BulletSpeed = 500.0f;
			m_BulletSize = { 10.0f, 20.0f, 1.0f };
			m_xOffset = 20.0f;
		}
	}

	static std::vector<glm::vec3> m_Path;

	glm::vec3 dist = m_Position - m_Player->getPosition();

	glm::vec2 direction = { cos(m_Rotation - glm::radians(90.0f)), sin(m_Rotation - glm::radians(90.0f)) };
	auto distFromPlayer = distance(m_Position, m_Player->getPosition());

	float objAng = atan2f(direction.x, direction.y) - atan2f(dist.x, dist.y);

	if (objAng < -PI)
		objAng += 2.0f * PI;
	if (objAng > PI)
		objAng -= 2.0f * PI;

	auto angle = angle2Vec(m_Position, m_Player->getPosition());// - glm::radians(90.0f);
	if (fabs(objAng) < glm::radians(m_FOV) / 2.0f && distFromPlayer < 700.0f)
	{
		glm::vec3 shotPosition = m_Position;
		shotPosition += glm::vec3(cos(m_Rotation), sin(m_Rotation), 0.0f) * (m_xOffset);
		glm::vec3 RayPosition = shotPosition + (glm::vec3(cos(angle - glm::radians(90.0f)), sin(angle - glm::radians(90.0f)), 0.0f) * m_Length);

		m_RayPoint = AABB(glm::vec2(RayPosition.x - 1.0f, RayPosition.y - 1.0f),
			glm::vec2(RayPosition.x + 1.0f, RayPosition.y + 1.0f));

		m_Length += 600.0f * deltaTime;

		if (m_EnemyStateMachine == EnemyState::IDLE && m_ClearShot == false)
			m_EnemyStateMachine = EnemyState::IDLE;
		else
		{
			m_Path = Map::findPath(m_Position, m_Player->getPosition());
			if (!m_Path.empty())
				m_EnemyStateMachine = EnemyState::CHASE;
			else if (m_Melee == true)
				m_EnemyStateMachine = EnemyState::MELEE;

			if (m_ClearShot && canShoot == true && distFromPlayer < 800.0f)
				m_EnemyStateMachine = EnemyState::ATTACK;

			else if (distFromPlayer <= 100.0f && m_Melee == true)
				m_EnemyStateMachine = EnemyState::MELEE;
		}

		if (m_Length >= 700.0f)
			m_Length = 0.0f;
	}
	else
		m_EnemyStateMachine = EnemyState::IDLE;


	int a = m_Animations[(int)m_WorldState][(int)m_CurrentAnimation].getFrameIndex();
	// state machine
	switch (m_EnemyStateMachine)
	{
	case EnemyState::IDLE:
		m_FOV = 130.0f;
		m_CurrentAnimation = AnimationState::IDLE;
		m_Path = Map::findPath(m_Position, m_DefaultPosition);

		if (!m_Path.empty())
		{
			auto angle = angle2Vec(m_Position, m_DefaultPosition) + glm::radians(180.0f);
			m_Rotation = angle;
			m_Position = moveTowards(m_Position, m_Path[m_Path.size() - 1], m_Speed * deltaTime);
			m_CurrentAnimation = AnimationState::WALK;
		}

		break;


	case EnemyState::CHASE:
		m_FOV = 360.0f;
		m_Rotation = angle + glm::radians(180.0f);

		if (!m_Path.empty())
			m_Position = moveTowards(m_Position, m_Path[m_Path.size() - 1], m_Speed * deltaTime);
		m_CurrentAnimation = AnimationState::WALK;

		break;

	case EnemyState::MELEE:
		m_FOV = 360.0f;
		m_Rotation = angle + glm::radians(180.0f);

		m_CurrentAnimation = AnimationState::MELEE;

		if (a == m_Animations[(int)m_WorldState][(int)m_CurrentAnimation].getNoFrames() - 1)
		{
			glm::vec3 hitBoxMelee = m_Position + (glm::vec3(cos(m_Rotation + glm::radians(90.0)), sin(m_Rotation + glm::radians(90.0)), 0.0f) * 60.0f);

			m_AttackCollisionBox = AABB(glm::vec2(hitBoxMelee.x - m_AttackScale.x / 2.0f, hitBoxMelee.y - m_AttackScale.y / 2.0f),
				glm::vec2(hitBoxMelee.x + m_AttackScale.x / 2.0f, hitBoxMelee.y + m_AttackScale.y / 2.0f));

			m_AttackCollisionBox.m_IsActive = true;
			m_Time = 0.0f;
		}

		else m_AttackCollisionBox.m_IsActive = false;

		break;

	case EnemyState::ATTACK:
		m_FOV = 360.0f;
		m_Rotation = angle + glm::radians(180.0f);
		m_AttackCollisionBox.m_IsActive = false;

		if (m_LastShot > m_ShotCooldown)
		{
			std::shared_ptr<Bullet> newBullet;
			glm::vec3 shotPosition = m_Position;
			shotPosition += glm::vec3(cos(m_Rotation), sin(m_Rotation), 0.0f) * (m_xOffset);
			if (m_WorldState == WorldState::Far_Future)
			{
				newBullet = std::make_shared<Bullet>(m_FarFutureBullet, shotPosition, m_BulletSize,
					glm::vec2(cos(m_Rotation + glm::radians(90.0)), sin(m_Rotation + glm::radians(90.0))), m_Rotation);
			}
			else if (m_WorldState == WorldState::Neanderthal)
			{
				newBullet = std::make_shared<Bullet>(m_NeanderthalSpear, shotPosition, m_BulletSize,
					glm::vec2(cos(m_Rotation + glm::radians(90.0)), sin(m_Rotation + glm::radians(90.0))), m_Rotation);
			}

			else
			{
				newBullet = std::make_shared<Bullet>(m_WorldWarBullet, shotPosition, m_BulletSize,
					glm::vec2(cos(m_Rotation + glm::radians(90.0)), sin(m_Rotation + glm::radians(90.0))), m_Rotation);
			}

			newBullet->setSpeed(m_BulletSpeed);
			EntityManager::addEnemyBullet(newBullet);
			m_LastShot = 0.0f;
		}

		m_CurrentAnimation = AnimationState::ATTACK;
		break;
	}

	m_LastShot += deltaTime;

	m_Animations[(int)m_WorldState][(int)m_CurrentAnimation].Play(deltaTime);

	m_CollisionBox = AABB(glm::vec2(m_Position.x - m_Scale.x / 2.0f, m_Position.y - m_Scale.y / 2.0f),
		glm::vec2(m_Position.x + m_Scale.x / 2.0f, m_Position.y + m_Scale.y / 2.0f));
}