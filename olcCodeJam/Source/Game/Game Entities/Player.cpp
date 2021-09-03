#include <string>
#include <memory>
#include <iostream>

#include "Player.h"
#include "Engine/Graphics/Renderer.h"

#include "GLFW/glfw3.h"
#include "Engine/Utils/Input.h"

#include "Engine/Utils/Maths.h"
#include "glm/gtc/matrix_transform.hpp"

#include "EntityManager.h"
#include "Bullet.h"

Player::Player(float x, float y, const glm::vec2& size): Entity(x, y, size)
{
	m_CollisionBox = AABB(glm::vec2(m_Position.x - m_Scale.x / 2.0f, m_Position.y - m_Scale.y / 2.0f),
		glm::vec2(m_Position.x + m_Scale.x / 2.0f, m_Position.y + m_Scale.y / 2.0f));

	m_AttackCollisionBox.m_IsActive = false;

	m_FarFutureBullet = std::make_shared<Texture>("Resources/Texture/Player/Robot/Bullet.png");
	m_NeanderthalSpear = std::make_shared<Texture>("Resources/Texture/Player/Neanderthal/Spear.png");
	m_WorldWarBullet = std::make_shared<Texture>("Resources/Texture/Player/WW1/Bullet.png");
}

void Player::onUpdate(float deltaTime)
{
	bool canShoot = false;
	m_Melee = true;

	if (m_WorldState == WorldState::Far_Future || m_WorldState == WorldState::Neanderthal || m_WorldState == WorldState::World_War1)
	{
		canShoot = true;

		if (m_WorldState == WorldState::Far_Future)
		{
			m_ShotCooldown = 0.3f;
			m_BulletSpeed = 600.0f;
			m_Melee = false;
			m_BulletSize = { 15.0f, 20.0f, 1.0f };
			m_xOffset = 25.0f;
		}

		else if (m_WorldState == WorldState::Neanderthal)
		{
			m_ShotCooldown = 0.8f;
			m_BulletSpeed = 600.0f;
			m_BulletSize = { 16.0f, 60.0f, 1.0f };
			m_xOffset = 30.0f;
		}

		else
		{
			m_ShotCooldown = 0.4f;
			m_BulletSpeed = 700.0f;
			m_BulletSize = { 10.0f, 20.0f, 1.0f };
			m_xOffset = 20.0f;
		}
	}

	m_Velocity = { 0.0f, 0.0f, 0.0f };
	if (isPressed(GLFW_KEY_W))
		m_Velocity.y += 1.0f;
	if (isPressed(GLFW_KEY_S))
		m_Velocity.y -= 1.0f;

	if (isPressed(GLFW_KEY_D))
		m_Velocity.x += 1.0f;
	if (isPressed(GLFW_KEY_A))
		m_Velocity.x -= 1.0f;

	m_Rotation = angle2Vec(glm::vec2(m_Position), m_LookTarget) + glm::radians(180.0);

	glm::vec3 collisionPosition = m_Position + glm::normalize(m_Velocity) * m_Speed * deltaTime * 5.0f;
	m_CollisionPoint = AABB(glm::vec2(collisionPosition.x - 20.0f, collisionPosition.y - 20.0f),
		glm::vec2(collisionPosition.x + 20.0f, collisionPosition.y + 20.0f));

	if (m_Velocity != glm::vec3(0.0f))
	{
		if (m_CanWalk)
		{
			m_lastPosition = m_Position;
			m_Position += glm::normalize(m_Velocity) * m_Speed * deltaTime;
		}
		else
			m_Position = m_lastPosition;

		m_CurrentAnimation = AnimationState::WALK;
	}

	else
	{
		m_CurrentAnimation = AnimationState::IDLE;
	}

	if (isMousePressed(GLFW_MOUSE_BUTTON_LEFT) && m_Melee == true)
	{
		m_CurrentAnimation = AnimationState::MELEE;
		auto a = m_Animations[(int)m_WorldState][(int)m_CurrentAnimation].getFrameIndex();
		if (a == m_Animations[(int)m_WorldState][(int)m_CurrentAnimation].getNoFrames() - 1)
		{
			glm::vec3 hitBoxMelee = m_Position + (glm::vec3(cos(m_Rotation + glm::radians(90.0)), sin(m_Rotation + glm::radians(90.0)), 0.0f) * 60.0f);
			m_AttackCollisionBox.m_IsActive = true;

			m_AttackCollisionBox = AABB(glm::vec2(hitBoxMelee.x - m_AttackScale.x / 2.0f, hitBoxMelee.y - m_AttackScale.y / 2.0f),
				glm::vec2(hitBoxMelee.x + m_AttackScale.x / 2.0f, hitBoxMelee.y + m_AttackScale.y / 2.0f));
		}
		else
			m_AttackCollisionBox.m_IsActive = false;
	}
	else
		m_AttackCollisionBox.m_IsActive = false;

	if (isMousePressed(GLFW_MOUSE_BUTTON_RIGHT) && canShoot == true)
	{
		m_CurrentAnimation = AnimationState::ATTACK;
		if (m_AttackCollisionBox.m_IsActive == false && m_LastShot > m_ShotCooldown)
		{
			std::shared_ptr<Bullet> newBullet;
			if (m_WorldState == WorldState::Far_Future)
			{
				m_LastShotOffset = -m_LastShotOffset;

				glm::vec3 shotPosition = m_Position;
				shotPosition += glm::vec3(cos(m_Rotation), sin(m_Rotation), 0.0f) * (m_xOffset * m_LastShotOffset);

				newBullet = std::make_shared<Bullet>(m_FarFutureBullet, shotPosition, m_BulletSize,
					glm::vec2(cos(m_Rotation + glm::radians(90.0)), sin(m_Rotation + glm::radians(90.0))), m_Rotation);
			}
			else if (m_WorldState == WorldState::Neanderthal)
			{
				glm::vec3 shotPosition = m_Position;
				shotPosition += glm::vec3(cos(m_Rotation), sin(m_Rotation), 0.0f) * m_xOffset;

				newBullet = std::make_shared<Bullet>(m_NeanderthalSpear, shotPosition, m_BulletSize,
					glm::vec2(cos(m_Rotation + glm::radians(90.0)), sin(m_Rotation + glm::radians(90.0))), m_Rotation);
			}

			else
			{
				glm::vec3 shotPosition = m_Position;
				shotPosition += glm::vec3(cos(m_Rotation), sin(m_Rotation), 0.0f) * m_xOffset;

				newBullet = std::make_shared<Bullet>(m_WorldWarBullet, shotPosition, m_BulletSize,
					glm::vec2(cos(m_Rotation + glm::radians(90.0)), sin(m_Rotation + glm::radians(90.0))), m_Rotation);
			}

			newBullet->setSpeed(m_BulletSpeed);
			EntityManager::addPlayerBullet(newBullet);
			m_LastShot = 0.0f;
		}
	}

	m_LastShot += deltaTime;

	m_Animations[(int)m_WorldState][(int)m_CurrentAnimation].Play(deltaTime);

	m_CollisionBox = AABB(glm::vec2(m_Position.x - m_Scale.x / 2.0f + 15.0f, m_Position.y - m_Scale.y / 2.0f + 15.0f),
		glm::vec2(m_Position.x + m_Scale.x / 2.0f - 15.0f, m_Position.y + m_Scale.y / 2.0f - 15.0f));
}
