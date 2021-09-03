#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <list>

#include "Engine/Graphics/Texture.h"
#include "Map.h"
#include "EntityManager.h"

#include "Engine/Graphics/Renderer.h"

#include "Engine/Utils/Maths.h"

EnemyProps Map::m_EnemyProps;

int Map::m_Width;
int Map::m_Height;

std::string Map::m_MapLayout;
std::string Map::m_CurrentLevel;

char Map::m_WallSymbol;
std::unordered_map<WorldState, MapTextures> Map::m_Textures;

std::vector<glm::vec3> Map::m_EnemyPositions;

float Map::m_Scale = 100.0f;

Node* Map::m_Nodes;

void Map::loadMap(const std::string& filepath)
{
	m_CurrentLevel = filepath;
	int width;
	int height;

	std::ifstream stream(filepath);
	stream >> m_Width;
	stream >> m_Height;

	glm::vec3 m_PlayerPosition;

	std::string line;
	EntityManager::m_Enemies.clear();
	EntityManager::m_Walls.clear();
	EntityManager::m_EnemyBullets.clear();
	EntityManager::m_PlayerBullets.clear();
	m_EnemyPositions.clear();
	m_MapLayout.clear();

	while (std::getline(stream, line))
	{
		if (line[0] != 'f' && line[0] != 's')
			m_MapLayout += line;

		else if(line[0] == 's')
		{
			m_WallSymbol = line[2];
		}
	}

	if (m_Nodes)
		delete[] m_Nodes;
	m_Nodes = new Node[m_Height * m_Width];

	float posY = 100 * m_Height;
	for (int i = 0; i < m_Height; i++)
	{
		float posX = i;
		for (int j = 0; j < m_Width; j++)
		{
			if (m_MapLayout[i * m_Width + j] == m_WallSymbol)
			{
				EntityManager::addWall({ posX, posY, 0.0f }, { 100.0f, 100.0f, 1.0f });
				m_Nodes[i * m_Width + j].m_Obstacle = true;
			}
			else
			{
				if (m_MapLayout[i * m_Width + j] == 'P')
					m_EnemyPositions.push_back({ posX, posY, 0.0f });

				else if (m_MapLayout[i * m_Width + j] == 'C')
					m_PlayerPosition = { posX, posY, 0.0f };

				m_Nodes[i * m_Width + j].m_Obstacle = false;
			}

			m_Nodes[i * m_Width + j].m_PositionOnMap.x = j;
			m_Nodes[i * m_Width + j].m_PositionOnMap.y = m_Height - i;

			m_Nodes[i * m_Width + j].m_Parent = nullptr;
			m_Nodes[i * m_Width + j].m_Visited = false;

			if (i > 0)
				m_Nodes[i * m_Width + j].m_Neighbours.push_back(&m_Nodes[(i - 1) * m_Width + (j + 0)]);
			if (i < m_Height - 1)										  					  
				m_Nodes[i * m_Width + j].m_Neighbours.push_back(&m_Nodes[(i + 1) * m_Width + (j + 0)]);
			if (j > 0)													  					  
				m_Nodes[i * m_Width + j].m_Neighbours.push_back(&m_Nodes[(i + 0) * m_Width + (j - 1)]);
			if (j < m_Width - 1)										  					  
				m_Nodes[i * m_Width + j].m_Neighbours.push_back(&m_Nodes[(i + 0) * m_Width + (j + 1)]);

			posX += 100.0f;
		}
		posY -= 100.0f;
	}

	for (const auto& pos : m_EnemyPositions)
	{
		std::shared_ptr<Enemy> m_Enemy;
		m_Enemy = std::make_shared<Enemy>(pos.x, pos.y, glm::vec2(100.0f, 100.0f), glm::radians(float(rand() % 360)));

		m_Enemy->setPlayer(m_EnemyProps.m_Player);
		m_Enemy->addAnimationSet(WorldState::Monkey, m_EnemyProps.EnemyMonkeySet);
		m_Enemy->addAnimationSet(WorldState::Neanderthal, m_EnemyProps.EnemyNeanderthalSet);
		m_Enemy->addAnimationSet(WorldState::Knight, m_EnemyProps.EnemyKnightSet);
		m_Enemy->addAnimationSet(WorldState::World_War1, m_EnemyProps.EnemyWW1Set);
		m_Enemy->addAnimationSet(WorldState::Far_Future, m_EnemyProps.EnemyRobotSet);

		m_Enemy->setBullets(m_EnemyProps.m_EnemyFarFuture, m_EnemyProps.m_EnemyNeanderthal, m_EnemyProps.m_EnemyWorldWar);

		EntityManager::addEnemy(m_Enemy);
	}

	m_EnemyProps.m_Player->setPosition(m_PlayerPosition);
}

void Map::Draw()
{
	float posY = 100 * m_Height;
	for (int i = 0; i < m_Height; i++)
	{
		float posX = i;
		for (int j = 0; j < m_Width; j++)
		{
			if (m_WallSymbol == m_MapLayout[i * m_Width + j])
				Renderer::drawQuad(m_Textures[Entity::m_WorldState].m_Wall, { 1.0f, 1.0f, 1.0f, 1.0f }, { posX, posY, 0.0f }, { m_Scale, m_Scale, 1.0f }, 0.0f);
			else
				Renderer::drawQuad(m_Textures[Entity::m_WorldState].m_Floor, { 1.0f, 1.0f, 1.0f, 1.0f }, { posX, posY, 0.0f }, { m_Scale, m_Scale, 1.0f }, 0.0f);
			posX += 100.0f;
		}
		posY -= 100.0f;
	}
}

std::vector<glm::vec3> Map::findPath(const glm::vec3& from, const glm::vec3& to)
{
	for (int i = 0; i < m_Height; i++)
		for (int j = 0; j < m_Width; j++)
		{
			m_Nodes[i * m_Width + j].m_Visited = false;
			m_Nodes[i * m_Width + j].m_GlobalGoal = INFINITY;
			m_Nodes[i * m_Width + j].m_LocalGoal = INFINITY;
			m_Nodes[i * m_Width + j].m_Parent = nullptr;
		}

	int fromX = from.x / m_Scale;
	int fromY = m_Height - from.y / m_Scale;

	int toX = to.x / m_Scale;
	int toY = m_Height - to.y / m_Scale;

	if (toX >= m_Width || toY >= m_Height)
		return std::vector<glm::vec3>();

	Node* currentNode = &m_Nodes[fromY * m_Width + fromX];
	currentNode->m_LocalGoal = 0.0f;
	currentNode->m_GlobalGoal = distance(from, to);

	Node* endNode = &m_Nodes[toY * m_Width + toX];

	std::list<Node*> NodesNotTested;
	NodesNotTested.push_back(currentNode);

	while (!NodesNotTested.empty() && currentNode != endNode)
	{
		if (currentNode == nullptr)
			return std::vector<glm::vec3>();

		NodesNotTested.sort([](const Node* lhs, const Node* rhs)
			{ return lhs->m_GlobalGoal < rhs->m_GlobalGoal; });

		while (!NodesNotTested.empty() && NodesNotTested.front()->m_Visited)
			NodesNotTested.pop_front();

		if (NodesNotTested.empty())
			break;

		currentNode = NodesNotTested.front();
		currentNode->m_Visited = true;

		for (auto& node : currentNode->m_Neighbours)
		{
			if (!node->m_Visited && node->m_Obstacle == false)
				NodesNotTested.push_back(node);

			float lowerGoal = currentNode->m_LocalGoal + 
				distance(currentNode->m_PositionOnMap, node->m_PositionOnMap);

			if (lowerGoal < node->m_LocalGoal)
			{
				node->m_Parent = currentNode;
				node->m_LocalGoal = lowerGoal;
				node->m_GlobalGoal = node->m_LocalGoal + 
					distance(node->m_PositionOnMap, endNode->m_PositionOnMap);
			}
		}
	}

	std::vector<glm::vec3> path;

	Node* node = endNode;
	while (node->m_Parent != nullptr)
	{
		path.push_back(glm::vec3(node->m_PositionOnMap * m_Scale, 0.0f));
		node = node->m_Parent;
	}

	return path;
}

void Map::addTextures(WorldState state, const std::string& wall, const std::string& floor)
{
	if (m_Textures.find(state) == m_Textures.end())
	{
		std::shared_ptr<Texture> w = std::make_shared<Texture>(wall);
		std::shared_ptr<Texture> f = std::make_shared<Texture>(floor);

		MapTextures maptex;
		maptex.m_Wall = w;
		maptex.m_Floor = f;

		m_Textures[state] = maptex;
	}
}
