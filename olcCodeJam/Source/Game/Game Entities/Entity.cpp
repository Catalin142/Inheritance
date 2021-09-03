#include "Entity.h"

WorldState Entity::m_WorldState = WorldState::Monkey;

void Entity::addAnimationSet(WorldState state, const AnimationSet& set)
{
	if (m_Animations.find((int)state) == m_Animations.end())
		m_Animations[(int)state] = set;
}