#include "ArcadePCH.h"
#include "ArcadeScene.h"
#include "NotImplementedScene.h"
#include "Breakout/Scenes/BreakoutScene.h"
#include "Pacman/Scenes/PacmanStartScene.h"
#include "Asteroids/Scenes/AsteroidsScene.h"

ArcadeScene::ArcadeScene()
	: ButtonOptionsScene({"Tetris", "Breakout", "Asteroids", "Pac-Man"}, Core::Color::Cyan(), true)
{
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

	return true;
}

void ArcadeScene::Update(uint32_t dt)
{
	ButtonOptionsScene::Update(dt);
}

void ArcadeScene::Draw(Core::Window& window)
{
	ButtonOptionsScene::Draw(window);
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
		{
			std::unique_ptr<AsteroidsScene> asteroidsScene = std::make_unique<AsteroidsScene>();
			return asteroidsScene;
			break;
		}
		case Game::Pacman:
		{
			std::unique_ptr<PacmanStartScene> pacmanStartScene = std::make_unique<PacmanStartScene>();
			return pacmanStartScene;
			break;
		}
	}
	std::unique_ptr<Core::Scene> notImplementedScene = std::make_unique<NotImplementedScene>();
	return notImplementedScene;
}
