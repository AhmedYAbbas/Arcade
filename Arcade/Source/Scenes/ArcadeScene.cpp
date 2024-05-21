#include "ArcadeScene.h"

ArcadeScene::ArcadeScene()
{
	Init();
}

void ArcadeScene::Init()
{
	Core::KeyboardButtonAction keyboardAction;
	keyboardAction.Key = Core::GameController::ActionKey();
	keyboardAction.Action = [](uint32_t dt, Core::InputState state)
	{
		if (Core::GameController::IsPressed(state))
			std::cout << "Action button was pressed!" << std::endl;	
	};

	m_GameController.AddKeyboardButtonAction(keyboardAction);


	Core::MouseButtonAction mouseAction;
	mouseAction.MouseButton = Core::GameController::LeftMouseButton();
	mouseAction.MouseInputAction = [](Core::InputState state, const Core::MousePosition& position)
	{
		if (Core::GameController::IsPressed(state))
			std::cout << "Left mouse button was pressed!" << std::endl;
	};

	m_GameController.AddMouseButtonAction(mouseAction);
	m_GameController.SetMouseMovedAction([](const Core::MousePosition& mousePosition)
	{
		std::cout << "Mouse position x: " << mousePosition.X << ", y: " << mousePosition.Y << std::endl;
	});
}

void ArcadeScene::Update(uint32_t dt)
{
}

void ArcadeScene::Draw(Core::Window& window)
{
	// TODO: move to renderer/game
	Core::Line line(Core::Vec2D(0, 0), Core::Vec2D(window.GetWidth(), window.GetHeight()));
	Core::Triangle triangle(Core::Vec2D(60, 10), Core::Vec2D(10, 110), Core::Vec2D(110, 110));
	Core::Rectangle rect(Core::Vec2D(window.GetWidth() / 2 - 25, window.GetHeight() / 2 - 25), 50, 50);
	Core::Circle circle(Core::Vec2D(window.GetWidth() / 2 + 50, window.GetHeight() / 2 + 50), 50);

	// TODO: move to renderer/game
	window.Draw(line, Core::Color::Yellow());
	window.Draw(triangle, Core::Color::Red(), true, Core::Color::Red());
	window.Draw(rect, Core::Color::Blue(), true, Core::Color::Blue());
	window.Draw(circle, Core::Color(0, 255, 0, 150), true, Core::Color(0, 255, 0, 150));
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade Scene";
	return sceneName;
}
