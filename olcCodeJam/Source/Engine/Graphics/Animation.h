#pragma once

#include <vector>
#include <array>
#include <memory>

#include "Texture.h"
#include "glm/glm.hpp"

class Animation
{
public:
	Animation() = default;
	Animation(const std::shared_ptr<Texture> spr, unsigned int frames, float speed = 0);
	Animation(const std::string & sprpth, unsigned int frames, float speed = 0);
	~Animation() = default;

	bool Play(float deltatime);
	void setSpeed(float speed) { m_Speed = speed; }

	const int& getFrameIndex() const { return m_FrameIndex; }
	std::shared_ptr<Texture>& getSprite() { return m_Sprite; }
	std::array<glm::vec2, 4>& getCurrentFrame() { return m_CurrentFrame; }
	const unsigned int getNoFrames() { return m_NoOfFrames; }
	const float getFrameWidth() { return m_FrameWidth; }

private:
	std::shared_ptr<Texture> m_Sprite;

	unsigned int m_NoOfFrames;
	float m_FrameWidth;

	float m_Speed;

	std::vector<std::array<glm::vec2, 4>> m_Frames;
	std::array<glm::vec2, 4> m_CurrentFrame;

	int m_FrameIndex = 0;
	float m_Time = 0;

private:
	void Init();

};