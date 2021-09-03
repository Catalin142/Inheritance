#include "Animation.h"

void Animation::Init()
{
	unsigned int width = m_Sprite->getWidth();

	m_FrameWidth = (width / (float)m_NoOfFrames) / width;

	float offset = 0;

	for (unsigned int index = 0; index < m_NoOfFrames; index++)
	{
		float frameOffRight = offset + m_FrameWidth;
		std::array<glm::vec2, 4> frame{ {
			glm::vec2(offset,        0.0f),
			glm::vec2(frameOffRight, 0.0f),
			glm::vec2(frameOffRight, 1.0f),
			glm::vec2(offset,		 1.0f),
		} };

		m_Frames.push_back(frame);
		offset += m_FrameWidth;
	}

	m_CurrentFrame = m_Frames[0];
}

Animation::Animation(const std::shared_ptr<Texture> spr, unsigned int frames, float speed) :
	m_Sprite(spr), m_NoOfFrames(frames), m_Speed(speed)
{
	Init();
}

Animation::Animation(const std::string& spr, unsigned int frames, float speed) :
	m_Sprite(std::make_shared<Texture>(spr)), m_NoOfFrames(frames), m_Speed(speed)
{
	Init();
}

bool Animation::Play(float dt)
{
	m_Time += dt;

	if (m_Time > m_Speed)
	{
		m_Time = 0;

		if (m_FrameIndex == m_NoOfFrames - 1)
			m_FrameIndex = 0;
		else m_FrameIndex++;
		return true;
	}

	m_CurrentFrame = m_Frames[m_FrameIndex];
	return false;
}