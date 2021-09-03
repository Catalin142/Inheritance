#include "AABB.h"

namespace glm {
	bool operator<(const glm::vec2& l, const glm::vec2& r)
	{
		return (l.x < r.x && l.y < r.y);
	}
	bool operator>(const glm::vec2& l, const glm::vec2& r)
	{
		return (l.x > r.x && l.y > r.y);
	}
	glm::vec2 operator/(const glm::vec2& l, float value)
	{
		return glm::vec2(l.x / value, l.y / value);
	}
}

AABB operator/(const AABB& left, float value)
{
	return AABB(left.m_Min / 2, left.m_Max / 2);
}

bool intersect(const AABB& left, const AABB& right)
{
	return (left.m_Max > right.m_Min && left.m_Min < right.m_Max) || (left.m_Max < right.m_Min&& left.m_Min > right.m_Max);
}

bool contain(const AABB& left, const glm::vec2& right)
{
	return (left.m_Max < right&& left.m_Min > right);
}

AABB::AABB(float _x, float _y, float _z, float _w) : m_Min(_x, _y), m_Max(_z, _w) { }
AABB::AABB(const glm::vec2& min, const glm::vec2& max) : m_Min(min), m_Max(max) { }

bool AABB::intersect(const AABB& other) const
{
	return (m_Max > other.m_Min && m_Min < other.m_Max)
		|| (m_Max < other.m_Min && m_Min > other.m_Max);
}

bool AABB::contain(const glm::vec2& other) const
{
	return (m_Max > other && m_Min < other);
}

AABB& AABB::operator+=(float value)
{
	this->m_Min += value;
	this->m_Max += value;

	return *this;
}

AABB& AABB::operator+=(glm::vec2 vec)
{
	m_Min.x += vec.x;
	m_Min.y += vec.y;
	m_Max.x += vec.x;
	m_Max.y += vec.y;

	return *this;
}