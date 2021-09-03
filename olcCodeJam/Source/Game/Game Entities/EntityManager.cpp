#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Engine/Graphics/Renderer.h"
#include "EntityManager.h"
#include "Map.h"

std::vector<std::shared_ptr<Enemy>> EntityManager::m_Enemies;
std::shared_ptr<Player> EntityManager::m_Player;
std::vector<Wall> EntityManager::m_Walls;
std::vector<std::shared_ptr<Bullet>> EntityManager::m_PlayerBullets;
std::vector<std::shared_ptr<Bullet>> EntityManager::m_EnemyBullets;


void EntityManager::Update(float deltaTime)
{
	bool finish = false;

	if (m_Player->m_isAlive)
	{
		m_Player->onUpdate(deltaTime);
		for (const auto& bullet : m_PlayerBullets)
		{
			bullet->onUpdate(deltaTime);

			for (const auto& enemy : m_Enemies)
				if (bullet->m_IsAlive)
				{
					if (bullet->m_CollisionBox.intersect(enemy->m_CollisionBox))
					{
						bullet->m_IsAlive = false;
						enemy->m_isAlive = false;
					}
				}
		}

		for (const auto& bullet : m_EnemyBullets)
		{
			bullet->onUpdate(deltaTime);

			if (bullet->m_CollisionBox.intersect(m_Player->m_CollisionBox))
			{
				bullet->m_IsAlive = false;
				finish = true;
			}
		}

		Renderer::drawEntity(m_Player);

		for (const auto& enemy : m_Enemies)
		{
			enemy->onUpdate(deltaTime);
			Renderer::drawEntity(enemy);

			if (m_Player->m_AttackCollisionBox.m_IsActive == true)
				if (enemy->m_CollisionBox.intersect(m_Player->m_AttackCollisionBox))
					enemy->m_isAlive = false;

			if (enemy->m_RayPoint.intersect(m_Player->m_CollisionBox))
			{
				enemy->m_ClearShot = true;
				enemy->m_Length = 0.0f;
			}

			if (enemy->m_AttackCollisionBox.m_IsActive == true)
				if (m_Player->m_CollisionBox.intersect(enemy->m_AttackCollisionBox))
					finish = true;
		}

		for (const auto& bullet : m_PlayerBullets)
			bullet->onUpdate(deltaTime);

		bool collide = false;
		for (const auto& wall : m_Walls)
		{
			if (!collide)
			{
				if (m_Player->m_CollisionPoint.intersect(wall.m_CollisionBox))
				{
					m_Player->m_CanWalk = false;
					collide = true;
				}
				else m_Player->m_CanWalk = true;
			}

			for (auto& bullet : m_PlayerBullets)
			{
				if (bullet->m_CollisionBox.intersect(wall.m_CollisionBox))
					bullet->m_IsAlive = false;
			}

			for (auto& enemybullet : m_EnemyBullets)
			{
				if (enemybullet->m_CollisionBox.intersect(wall.m_CollisionBox))
					enemybullet->m_IsAlive = false;
			}

			for (const auto& enemy : m_Enemies)
			{
				if (wall.m_CollisionBox.intersect(enemy->m_RayPoint))
				{
					enemy->m_ClearShot = false;
					enemy->m_Length = 0.0f;
				}
			}
		}
	}

	if (finish)
	{
		if (Entity::m_WorldState == WorldState::Far_Future)
			m_Player->m_isAlive = false;

		auto a = (int)Entity::m_WorldState + 1;
		if (a > int(WorldState::Far_Future))
			a = (int)WorldState::Far_Future;
		Entity::m_WorldState = (WorldState)a;
		Map::loadMap(Map::m_CurrentLevel);
	}

	m_PlayerBullets.erase(std::remove_if(m_PlayerBullets.begin(), m_PlayerBullets.end(), [](std::shared_ptr<Bullet>& bullet) {
		return (bullet->isAlive() == false);
		}), m_PlayerBullets.end());

	m_EnemyBullets.erase(std::remove_if(m_EnemyBullets.begin(), m_EnemyBullets.end(), [](std::shared_ptr<Bullet>& bullet) {
		return (bullet->isAlive() == false);
		}), m_EnemyBullets.end());

	m_Enemies.erase(std::remove_if(m_Enemies.begin(), m_Enemies.end(), [](std::shared_ptr<Enemy>& enemy) {
		return (enemy->isAlive() == false);
		}), m_Enemies.end());
}

void EntityManager::addWall(const glm::vec3& pos, const glm::vec3& size)
{
	Wall newWall;
	newWall.m_Position = pos;
	newWall.m_Scale = size;
	newWall.setCollisionBox();

	m_Walls.push_back(newWall);
}
