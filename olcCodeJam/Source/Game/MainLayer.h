#include <memory>

#include "Engine/Core/Layer.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Renderer.h"

#include "Engine/Utils/Camera.h"
#include "Engine/Utils/FontManager.h"

#include "Game Entities/EntityManager.h"

#include "Engine/Utils/Input.h"
#include "Game/Game Entities/Map.h"

class MainLayer : public Layer
{
public:
	MainLayer() = default;
	~MainLayer() = default;

	virtual void onAttach() override 
	{
		srand(time(NULL));

		m_Camera = std::make_shared<Camera>(1600.0f, 900.0f, glm::vec3(100.0f, 100.0f, 1.0f));

		m_Player = std::make_shared<Player>(200.0f, 300.0f, glm::vec2(100.0f, 100.0f));
		// Enemy-----------------------------

		EnemyMonkeySet[(int)AnimationState::WALK] =  Animation("Resources/Texture/Enemies/Monkey/Walk.png", 5, 0.1f);
		EnemyMonkeySet[(int)AnimationState::IDLE] =  Animation("Resources/Texture/Enemies/Monkey/Idle.png", 4, 0.17f);
		EnemyMonkeySet[(int)AnimationState::MELEE] = Animation("Resources/Texture/Enemies/Monkey/Attack.png", 3, 0.1f);

		EnemyNeanderthalSet[(int)AnimationState::WALK] =  Animation("Resources/Texture/Enemies/Neanderthal/Walk.png", 4, 0.1f);
		EnemyNeanderthalSet[(int)AnimationState::IDLE] =  Animation("Resources/Texture/Enemies/Neanderthal/Idle.png", 1, 0.1000000f);
		EnemyNeanderthalSet[(int)AnimationState::MELEE] = Animation("Resources/Texture/Enemies/Neanderthal/Attack.png", 4, 0.1f);
		EnemyNeanderthalSet[(int)AnimationState::ATTACK] =Animation("Resources/Texture/Enemies/Neanderthal/Attack.png", 4, 0.1f);
		
		EnemyKnightSet[(int)AnimationState::WALK] = Animation("Resources/Texture/Enemies/Knight/Walk.png", 4, 0.1f);
		EnemyKnightSet[(int)AnimationState::IDLE] = Animation("Resources/Texture/Enemies/Knight/Idle.png", 2, 0.40f);
		EnemyKnightSet[(int)AnimationState::MELEE] =Animation("Resources/Texture/Enemies/Knight/Attack.png", 4, 0.1f);

		EnemyWW1Set[(int)AnimationState::WALK] =  Animation("Resources/Texture/Enemies/WW1/Walk.png", 4, 0.1f);
		EnemyWW1Set[(int)AnimationState::IDLE] =  Animation("Resources/Texture/Enemies/WW1/Idle.png", 2, 0.1f);
		EnemyWW1Set[(int)AnimationState::MELEE] = Animation("Resources/Texture/Enemies/WW1/Melee.png", 4, 0.1f);
		EnemyWW1Set[(int)AnimationState::ATTACK] =Animation("Resources/Texture/Enemies/WW1/Attack.png", 2, 0.08f);

		EnemyRobotSet[(int)AnimationState::WALK] = Animation("Resources/Texture/Enemies/Robot/Walk.png", 3, 0.05f);
		EnemyRobotSet[(int)AnimationState::IDLE] =  Animation("Resources/Texture/Enemies/Robot/Idle.png", 1, 0.100000f);
		EnemyRobotSet[(int)AnimationState::ATTACK] = Animation("Resources/Texture/Enemies/Robot/Attack.png", 2, 0.06f);


		// Player----------------------------
		PlayerMonkeySet[(int)AnimationState::WALK] = Animation("Resources/Texture/Player/Monkey/Walk.png", 5, 0.1f);
		PlayerMonkeySet[(int)AnimationState::IDLE] = Animation("Resources/Texture/Player/Monkey/Idle.png", 4, 0.17f);
		PlayerMonkeySet[(int)AnimationState::MELEE] = Animation("Resources/Texture/Player/Monkey/Attack.png", 3, 0.1f);
		m_Player->addAnimationSet(WorldState::Monkey, PlayerMonkeySet);

		PlayerNeanderthalSet[(int)AnimationState::WALK] = Animation("Resources/Texture/Player/Neanderthal/Walk.png", 4, 0.1f);
		PlayerNeanderthalSet[(int)AnimationState::IDLE] = Animation("Resources/Texture/Player/Neanderthal/Idle.png", 1, 0.1000000f);
		PlayerNeanderthalSet[(int)AnimationState::ATTACK] = Animation("Resources/Texture/Player/Neanderthal/Attack.png", 4, 0.1f);
		PlayerNeanderthalSet[(int)AnimationState::MELEE] = Animation("Resources/Texture/Player/Neanderthal/Attack.png", 4, 0.1f);
		m_Player->addAnimationSet(WorldState::Neanderthal, PlayerNeanderthalSet);

		PlayerKnightSet[(int)AnimationState::WALK] = Animation("Resources/Texture/Player/Knight/Walk.png", 4, 0.1f);
		PlayerKnightSet[(int)AnimationState::IDLE] = Animation("Resources/Texture/Player/Knight/Idle.png", 2, 0.40f);
		PlayerKnightSet[(int)AnimationState::MELEE] = Animation("Resources/Texture/Player/Knight/Attack.png", 4, 0.1f);
		m_Player->addAnimationSet(WorldState::Knight, PlayerKnightSet);

		PlayerWW1Set[(int)AnimationState::WALK] =  Animation("Resources/Texture/Player/WW1/Walk.png", 4, 0.1f);
		PlayerWW1Set[(int)AnimationState::IDLE] =  Animation("Resources/Texture/Player/WW1/Idle.png", 2, 0.1f);
		PlayerWW1Set[(int)AnimationState::MELEE] = Animation("Resources/Texture/Player/WW1/Melee.png", 4, 0.1f);
		PlayerWW1Set[(int)AnimationState::ATTACK] =Animation("Resources/Texture/Player/WW1/Attack.png", 2, 0.08f);
		m_Player->addAnimationSet(WorldState::World_War1, PlayerWW1Set);

		PlayerRobotSet[(int)AnimationState::WALK] =  Animation("Resources/Texture/Player/Robot/Walk.png", 4, 0.1f);
		PlayerRobotSet[(int)AnimationState::IDLE] =  Animation("Resources/Texture/Player/Robot/Idle.png", 1, 0.100000f);
		PlayerRobotSet[(int)AnimationState::ATTACK] =Animation("Resources/Texture/Player/Robot/Attack.png", 2, 0.08f);
		m_Player->addAnimationSet(WorldState::Far_Future, PlayerRobotSet);

		// Lume ..................................

		Map::addTextures(WorldState::Monkey, "Resources/Texture/Enviroment/Monkey/Wall.png", "Resources/Texture/Enviroment/Monkey/Floor.png");
		Map::addTextures(WorldState::Neanderthal, "Resources/Texture/Enviroment/Neanderthal/Wall.png", "Resources/Texture/Enviroment/Neanderthal/Floor.png");
		Map::addTextures(WorldState::Knight, "Resources/Texture/Enviroment/Knight/Wall.png", "Resources/Texture/Enviroment/Knight/Floor.png");
		Map::addTextures(WorldState::World_War1, "Resources/Texture/Enviroment/WW1/Wall.png", "Resources/Texture/Enviroment/WW1/Floor.png");
		Map::addTextures(WorldState::Far_Future, "Resources/Texture/Enviroment/Robot/Wall.png", "Resources/Texture/Enviroment/Robot/Floor.png");

		m_EnemyFarFuture = std::make_shared<Texture>("Resources/Texture/Enemies/Robot/Bullet.png");
		m_EnemyNeanderthal = std::make_shared<Texture>("Resources/Texture/Enemies/Neanderthal/Spear.png");
		m_EnemyWorldWar = std::make_shared<Texture>("Resources/Texture/Enemies/WW1/Bullet.png");

		EntityManager::setPlayer(m_Player);

		EnemyProps props;
		props.m_Player = m_Player;
		props.EnemyKnightSet = EnemyKnightSet;
		props.EnemyMonkeySet = EnemyMonkeySet;
		props.EnemyNeanderthalSet = EnemyNeanderthalSet;
		props.EnemyRobotSet = EnemyRobotSet;
		props.EnemyWW1Set = EnemyWW1Set;
		props.m_EnemyFarFuture = m_EnemyFarFuture;
		props.m_EnemyNeanderthal = m_EnemyNeanderthal;
		props.m_EnemyWorldWar = m_EnemyWorldWar;

		Map::setProps(props);
		std::string levelPath = "Resources/Levels/Level" + std::to_string(m_CurrentLevel) + ".txt";
		Map::loadMap(levelPath);

		FontManager::addFont("Font", "Resources/Fonts/Joystix.ttf", 120.0f);
	}

	virtual void onUpdate(float deltaTime) override
	{
		Renderer::clear(0.0f, 0.0f, 0.0f);
		 
		if (!m_Intro)
		{
			Renderer::beginScene(glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f));

			Renderer::drawString("There are enemies on your territory and you",
				FontManager::getFont("Font"), { 100.0f, 700.0f, 0.0f },
				{ 40.0f, 40.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
			Renderer::drawString("have to eliminate them.",
				FontManager::getFont("Font"), { 0.0f, 650.0f, 0.0f },
				{ 40.0f, 40.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

			Renderer::drawString("If you fail on your quest a future generation",
				FontManager::getFont("Font"), { 100.0f, 600.0f, 0.0f },
				{ 40.0f, 40.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
			Renderer::drawString("will try to finish it.",
				FontManager::getFont("Font"), { 0.0f, 550.0f, 0.0f },
				{ 40.0f, 40.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });


			Renderer::drawString("Press enter ->",
				FontManager::getFont("Font"), { 900.0f, 200.0f, 0.0f },
				{ 40.0f, 40.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

			Renderer::endScene();

			if (isPressed(GLFW_KEY_ENTER))
				m_Intro = true;
		}

		else
		{
			m_Player->setTarget(getMouseWorld(m_Camera));

			Renderer::beginScene(m_Camera);
			Map::Draw();

			if (!m_Pause)
				EntityManager::Update(deltaTime);
			Renderer::endScene();

			Renderer::beginScene(glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f));
			if (m_Pause)
			{
				Renderer::drawString("pause", FontManager::getFont("Font"), { 1600.0f / 2.0f - 200.0f, 900.0f / 2.0f - 25.0f, 0.0f },
					{ 100.0f, 100.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
 				Renderer::drawString("Made by Popa Catalin", FontManager::getFont("Font"), { 0, 0, 0.0f }, { 50.0f, 50.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
			}

			if (!EntityManager::isGameOver())
			{
				if (m_CurrentLevel <= m_NoOfLevels)
				{
					Renderer::drawString("Level " + std::to_string(m_CurrentLevel) + " - " + std::to_string(m_NoOfLevels),
						FontManager::getFont("Font"), { 0.0f, 900.0f - 50.0f, 0.0f },
						{ 50.0f, 50.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

					Renderer::drawString("Enemies remaining: " + std::to_string(EntityManager::getNumberOfEnemies()),
						FontManager::getFont("Font"), { 600.0f, 900.0f - 50.0f, 0.0f },
						{ 50.0f, 50.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
				}
			}

			else
			{
				Renderer::drawString("Try again - Press R",
					FontManager::getFont("Font"), { 400.0f, 900.0f / 2.0f - 50.0f, 0.0f },
					{ 50.0f, 50.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

				if (isPressed(GLFW_KEY_R))
				{
					Entity::m_WorldState = WorldState::Monkey;
					Map::loadMap("Resources/Levels/Level1.txt");
					m_Player->isAlive() = true;
					m_CurrentLevel = 1;
				}

			}

			if (EntityManager::isLevelComplete())
			{
				if (m_CurrentLevel > m_NoOfLevels)
				{
					Renderer::drawString("You finished the game - Press R ",
						FontManager::getFont("Font"), { 100.0f, 900.0f / 2.0f - 50.0f, 0.0f },
						{ 50.0f, 50.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

					if (isPressed(GLFW_KEY_R))
					{
						Entity::m_WorldState = WorldState::Monkey;
						Map::loadMap("Resources/Levels/Level1.txt");
						m_CurrentLevel = 1;
					}
				}
				else
				{
					m_CurrentLevel++;

					if (m_CurrentLevel <= m_NoOfLevels)
					{
						std::string levelPath = "Resources/Levels/Level" + std::to_string(m_CurrentLevel) + ".txt";
						Map::loadMap(levelPath);
						glm::vec3 m_CamPos;
						m_CamPos.x = m_Player->getPosition().x - 1600.0f / 2.0f;
						m_CamPos.y = m_Player->getPosition().y - 900.0f / 2.0f;
						m_CamPos.z = 0.0f;
						m_Camera->setPosition(m_CamPos);
					}
				}
			}

			Renderer::endScene();
			m_Camera->onUpdate(m_Player, deltaTime);

			bool state = isPressed(GLFW_KEY_ESCAPE);
			if (state != escState && state == true)
			{
				m_Pause = !m_Pause;
				escState = true;
			}

			if (state == false)
				escState = state;
		}
	}

private:
	std::shared_ptr<Player> m_Player;

	std::shared_ptr<Texture> m_EnemyFarFuture;
	std::shared_ptr<Texture> m_EnemyNeanderthal;
	std::shared_ptr<Texture> m_EnemyWorldWar;
	
	std::shared_ptr<Camera> m_Camera;
	
	std::shared_ptr<Font> m_Font;

	bool escState = false;
	bool m_Pause = false;
	bool m_Intro = false;

	int m_NoOfLevels = 8;
	int m_CurrentLevel = 1;

	AnimationSet EnemyMonkeySet;
	AnimationSet EnemyNeanderthalSet;
	AnimationSet EnemyKnightSet;
	AnimationSet EnemyWW1Set;
	AnimationSet EnemyRobotSet;
	
	AnimationSet PlayerMonkeySet;
	AnimationSet PlayerNeanderthalSet;
	AnimationSet PlayerKnightSet;
	AnimationSet PlayerWW1Set;
	AnimationSet PlayerRobotSet;
};