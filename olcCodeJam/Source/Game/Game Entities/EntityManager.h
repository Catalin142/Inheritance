#pragma once

#include "Enemy.h"
#include "Player.h"
#include "Wall.h"
#include "Bullet.h"

class EntityManager
{
	friend class Map;

public:
	static void Update(float deltaTime);

	static void addEnemy(const std::shared_ptr<Enemy>& enemy) { m_Enemies.push_back(enemy); }
	static void setPlayer(const std::shared_ptr<Player>& player) { m_Player = player; }
	static void addWall(const glm::vec3& pos, const glm::vec3& size);
	static void addPlayerBullet(const std::shared_ptr<Bullet>& bul) { m_PlayerBullets.push_back(bul); }
	static void addEnemyBullet(const std::shared_ptr<Bullet>& bul) { m_EnemyBullets.push_back(bul); }

	static bool isLevelComplete() { return m_Enemies.empty(); }
	static bool isGameOver() { return !m_Player->m_isAlive; }

	static int getNumberOfEnemies() { return m_Enemies.size(); }
private:
	static std::vector<std::shared_ptr<Enemy>> m_Enemies;
	static std::shared_ptr<Player> m_Player;
	static std::vector<Wall> m_Walls;
	static std::vector<std::shared_ptr<Bullet>> m_PlayerBullets;
	static std::vector<std::shared_ptr<Bullet>> m_EnemyBullets;

};