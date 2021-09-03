#pragma once
#include "glm/glm.hpp"

class AABB
{
public:
	friend AABB operator/(const AABB& left, float value);

	friend bool intersect(const AABB& left, const AABB& right);
	friend bool contain(const AABB& left, const glm::vec2& right);

	AABB() = default;
	AABB(float _x, float _y, float _z, float _w);
	AABB(const glm::vec2& min, const glm::vec2& max);

	bool intersect(const AABB& other) const;
	bool contain(const glm::vec2& other) const;

	AABB& operator+=(float value);
	AABB& operator+=(glm::vec2 vec);

	bool m_IsActive = true;

private:
	glm::vec2 m_Min;
	glm::vec2 m_Max;

};
