#include "ArcadeScene.h"

ArcadeScene::ArcadeScene()
{
	Init();
}

bool ArcadeScene::Init()
{
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

	return true;
}

void ArcadeScene::Update(uint32_t dt)
{
}

void ArcadeScene::Draw(Core::Window& window)
{
	const Core::BitmapFont& font = Core::Application::Get().GetFont();
	Core::Rectangle rect(Core::Vec2D::Zero, Core::Application::Get().GetWindow().GetWidth(), Core::Application::Get().GetWindow().GetHeight());
	Core::Vec2D textDrawPosition;
	textDrawPosition = font.GetDrawPosition(GetSceneName(), rect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);
	window.Draw(font, GetSceneName(), textDrawPosition, Core::Color::Red());
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade";
	return sceneName;
}
