#pragma once

#include "glm/glm.hpp"

#include <unordered_map>
#include "Engine/Graphics/Animation.h"
#include "Engine/Utils/AABB.h"

enum class WorldState : int
{
	Monkey, 
	Neanderthal,
	Knight,
	World_War1,
	Far_Future,
};

enum class AnimationState : int
{
	IDLE,
	WALK,
	ATTACK,
	MELEE,
};

using AnimationSet = std::unordered_map<int, Animation>;

class Entity
{
	friend class EntityManager;
	friend class Renderer;

public:
	Entity(float x, float y, const glm::vec2& size) : m_Position(glm::vec3(x, y, 0.0f)), m_Scale(glm::vec3(size, 1.0f)) {}
	virtual ~Entity() = default;

	virtual void onUpdate(float deltaTime) { }
	void addAnimationSet(WorldState state, const AnimationSet& set);

	void setTarget(const glm::vec2& target) { m_LookTarget = target; }
	const glm::vec3& getPosition() const { return m_Position; }

	bool& isAlive() { return m_isAlive; }

	static WorldState m_WorldState;

protected:
	std::unordered_map<int, AnimationSet> m_Animations; // animatie[Monkey]->walk
	AnimationState m_CurrentAnimation = AnimationState::IDLE;

	AABB m_CollisionBox;

	glm::vec3 m_Position;
	glm::vec3 m_DefaultPosition;
	glm::vec3 m_Scale;
	glm::vec4 m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	float m_Rotation = 0.0f;
	float m_Speed = 400.0f;

	glm::vec2 m_LookTarget = glm::vec2(0.0f, 0.0f);

	glm::vec3 m_Velocity;

	bool m_isAlive = true;
	bool m_Melee = true;

	AABB m_AttackCollisionBox;
	glm::vec3 m_AttackScale = glm::vec3(50.0f, 30.0f, 1.0f);

	float m_ShotCooldown = 0.5f;
	float m_LastShot = m_ShotCooldown;
	float m_BulletSpeed = 300.0f;

	glm::vec3 m_ShootPosition;
	glm::vec3 m_BulletSize;
	float m_xOffset = 0.0f;
	float m_LastShotOffset = 1.0f;
};