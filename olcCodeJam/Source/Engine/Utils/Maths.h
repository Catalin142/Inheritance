#pragma once
#include <cmath>
// functii ce nu e gasesc in glm

#include "glm/glm.hpp"

#define PI 3.14159265358979323846f

static float angle2Vec(const glm::vec2& left, const glm::vec2& right)
{
	return std::atan2(-(left.x - right.x), left.y - right.y);
}

static glm::vec3 moveTowards(const glm::vec3& current, const glm::vec3& target, float distanceDelta)
{
	glm::vec3 tract = target - current;
	float remainingDistance = glm::length(tract);
	if (remainingDistance <= distanceDelta || remainingDistance == 0.0f)
		return target;
	else return (current + tract / remainingDistance * distanceDelta);
}

static float distance(const glm::vec3& from, const glm::vec3& to)
{
	return sqrtf((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
}