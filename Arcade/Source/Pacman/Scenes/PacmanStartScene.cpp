#include "ArcadePCH.h"
#include "PacmanStartScene.h"
#include "PacmanScene.h"
#include "Scenes/NotImplementedScene.h"

PacmanStartScene::PacmanStartScene()
	: ButtonOptionsScene({"Play Game", "High Scores"}, Core::Color::Yellow())
{
}

bool PacmanStartScene::Init()
{
	Core::KeyboardButtonAction backAction;
	backAction.Key = Core::GameController::CancelKey();
	backAction.Action = [](uint32_t dt, Core::InputState state)
	{
		if (Core::GameController::IsPressed(state))
			Core::Application::Get().PopScene();
	};
	m_GameController.AddKeyboardButtonAction(backAction);

	std::vector<Core::Button::ButtonAction> actions;
	actions.push_back([this]()
	{
		Core::Application::Get().PushScene(std::make_unique<PacmanScene>());
	});

	actions.push_back([this]()
	{
		Core::Application::Get().PushScene(std::make_unique<NotImplementedScene>());
	});

	SetButtonActions(actions);
	return ButtonOptionsScene::Init();
}

void PacmanStartScene::Update(uint32_t dt)
{
}

void PacmanStartScene::Draw(Core::Window& window)
{
	ButtonOptionsScene::Draw(window);
}

const std::string& PacmanStartScene::GetSceneName() const
{
	static std::string sceneName = "Pacman Start";
	return sceneName;
}
