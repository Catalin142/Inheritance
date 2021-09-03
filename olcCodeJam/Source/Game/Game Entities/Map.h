#pragma once

#include "glm/glm.hpp"
#include "Entity.h"
#include "Player.h"

struct EnemyProps
{
	std::shared_ptr<Texture> m_EnemyFarFuture;
	std::shared_ptr<Texture> m_EnemyNeanderthal;
	std::shared_ptr<Texture> m_EnemyWorldWar;

	AnimationSet EnemyMonkeySet;
	AnimationSet EnemyNeanderthalSet;
	AnimationSet EnemyKnightSet;
	AnimationSet EnemyWW1Set;
	AnimationSet EnemyRobotSet;

	std::shared_ptr<Player> m_Player;
};

struct Node
{
	bool m_Visited = false;
	bool m_Obstacle = true;
	float m_GlobalGoal;
	float m_LocalGoal;

	glm::vec2 m_PositionOnMap;

	std::vector<Node*> m_Neighbours;
	Node* m_Parent;
};

struct MapTextures
{
	std::shared_ptr<Texture> m_Wall;
	std::shared_ptr<Texture> m_Floor;
};

class Map
{
	friend class EntityManager;
public:
	static void loadMap(const std::string& filepath);
	static void Draw();

	static std::vector<glm::vec3> findPath(const glm::vec3& form, const glm::vec3& to);
	static void setProps(EnemyProps props) { m_EnemyProps = props; }

	static void addTextures(WorldState state, const std::string& wall, const std::string& floor);

private:
	static int m_Width;
	static int m_Height;

	static std::string m_MapLayout;
	static std::string m_CurrentLevel;
	static char m_WallSymbol;

	static std::unordered_map<WorldState, MapTextures> m_Textures;
	static std::vector<glm::vec3> m_EnemyPositions;

	static EnemyProps m_EnemyProps;

	static Node* m_Nodes;

	static float m_Scale;

private:
	Map() = default;
	~Map() = default;
};