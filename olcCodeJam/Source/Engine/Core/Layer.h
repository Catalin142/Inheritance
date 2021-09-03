#pragma once

class Layer
{
	friend class Application;

public:
	Layer() = default;
	virtual ~Layer() = default;

	virtual void onAttach() = 0;
	virtual void onUpdate(float deltaTime) = 0;

private:
	bool m_isActive = true;
};