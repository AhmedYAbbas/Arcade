#include "ArcadePCH.h"
#include "ArcadeScene.h"
#include "Breakout/Scenes/BreakoutScene.h"
#include "NotImplementedScene.h"

ArcadeScene::ArcadeScene()
	: ButtonOptionsScene({"Tetris", "Breakout", "Asteroids", "Pac-Man"}, Core::Color::Cyan())
{
	Init();
}

bool ArcadeScene::Init()
{
	std::vector<Core::Button::ButtonAction> actions;

	actions.push_back([this]()
	{
		Core::Application::Get().PushScene(GetScene(Game::Tetris));
	});

	actions.push_back([this]()
	{
		Core::Application::Get().PushScene(GetScene(Game::Breakout));
	});

	actions.push_back([this]()
	{
		Core::Application::Get().PushScene(GetScene(Game::Asteroids));
	});

	actions.push_back([this]()
	{
		Core::Application::Get().PushScene(GetScene(Game::Pacman));
	});

	SetButtonActions(actions);
	ButtonOptionsScene::Init();

	m_SpriteSheet.Load("PacmanSprites");
	m_AnimatedSprite.Init(Core::Application::Get().GetBasePath() + "Pacman_animations.txt", m_SpriteSheet);
	m_AnimatedSprite.SetAnimation("move_right", true);

	return true;
}

void ArcadeScene::Update(uint32_t dt)
{
	m_AnimatedSprite.Update(dt);
}

void ArcadeScene::Draw(Core::Window& window)
{
	ButtonOptionsScene::Draw(window);

	m_AnimatedSprite.Draw(window);
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade";
	return sceneName;
}

std::unique_ptr<Core::Scene> ArcadeScene::GetScene(Game game)
{
	switch (game)
	{
		case Game::Tetris:
			break;
		case Game::Breakout:
		{
			std::unique_ptr<BreakoutScene> breakoutScene = std::make_unique<BreakoutScene>();
			return breakoutScene;
			break;
		}
		case Game::Asteroids:
			break;
		case Game::Pacman:
			break;
	}
	std::unique_ptr<Core::Scene> notImplementedScene = std::make_unique<NotImplementedScene>();
	return notImplementedScene;
}
