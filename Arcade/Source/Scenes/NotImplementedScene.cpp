#include "ArcadePCH.h"
#include "NotImplementedScene.h"

bool NotImplementedScene::Init()
{
	Core::KeyboardButtonAction backAction;
	backAction.Key = Core::GameController::CancelKey();
	backAction.Action = [](uint32_t dt, Core::InputState state)
	{
		if (Core::GameController::IsPressed(state))
			Core::Application::Get().PopScene();
	};
	m_GameController.AddKeyboardButtonAction(backAction);

	return true;
}

void NotImplementedScene::Update(uint32_t dt)
{
}

void NotImplementedScene::Draw(Core::Window& window)
{
	const Core::BitmapFont& font = Core::Application::Get().GetFont();
	Core::Rectangle rect(Core::Vec2D::Zero, Core::Application::Get().GetWindow().GetWidth(), Core::Application::Get().GetWindow().GetHeight());
	Core::Vec2D textDrawPosition = font.GetDrawPosition(GetSceneName(), rect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);
	window.Draw(font, GetSceneName(), textDrawPosition, Core::Color::Red());
}

const std::string& NotImplementedScene::GetSceneName() const
{
	static std::string name = "Not Implemented!";
	return name;
}
