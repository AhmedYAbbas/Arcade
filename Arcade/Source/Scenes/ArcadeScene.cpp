#include "ArcadeScene.h"

ArcadeScene::ArcadeScene()
{
	Init();
}

void ArcadeScene::Init()
{
	m_TempSS.Load("ArcadeFont");

	Core::KeyboardButtonAction keyboardAction;
	keyboardAction.Key = Core::GameController::ActionKey();
	keyboardAction.Action = [](uint32_t dt, Core::InputState state)
	{
		//if (Core::GameController::IsPressed(state))
			//std::cout << "Action button was pressed!" << std::endl;	
	};

	m_GameController.AddKeyboardButtonAction(keyboardAction);


	Core::MouseButtonAction mouseAction;
	mouseAction.MouseButton = Core::GameController::LeftMouseButton();
	mouseAction.MouseInputAction = [](Core::InputState state, const Core::MousePosition& position)
	{
		//if (Core::GameController::IsPressed(state))
			//std::cout << "Left mouse button was pressed!" << std::endl;
	};

	m_GameController.AddMouseButtonAction(mouseAction);
	m_GameController.SetMouseMovedAction([](const Core::MousePosition& mousePosition)
	{
		//std::cout << "Mouse position x: " << mousePosition.X << ", y: " << mousePosition.Y << std::endl;
	});
}

void ArcadeScene::Update(uint32_t dt)
{
}

void ArcadeScene::Draw(Core::Window& window)
{
	window.Draw(m_TempSS, "!", Core::Vec2D::Zero);
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade Scene";
	return sceneName;
}
