#pragma once
#include "glm/glm.hpp"

#include "Game/Game Entities/Player.h"

class Camera
{
public:
	Camera(float width, float height, const glm::vec3& pos = { 0.0f, 0.0f, 0.0f });
	~Camera() = default;

	void onUpdate(const std::shared_ptr<Player>& player, float deltaTime);// sa urmareasca playerul
	glm::mat4 getMatrix();

	const glm::vec3& getPosition() const { return m_Position; }
	void setPosition(const glm::vec3& pos) { m_Position = pos; }

private:
	glm::mat4 m_Orthographic;
	glm::vec3 m_Position;

	float m_Width;
	float m_Height;
	float m_Speed = 365.0f;
};