#include "ArcadePCH.h"
#include "ButtonOptionsScene.h"

ButtonOptionsScene::ButtonOptionsScene(const std::vector<std::string>& optionNames, const Core::Color& textColor)
	: m_HighlightedOption(0)
{
	const Core::BitmapFont& font = Core::Application::Get().GetFont();
	for (size_t i = 0; i < optionNames.size(); ++i)
	{
		m_Buttons.push_back(Core::Button(font, textColor));
		m_Buttons.back().SetButtonText(optionNames[i]);
	}

	if (!optionNames.empty())
		m_Buttons[m_HighlightedOption].SetHighlighted(true);
}

void ButtonOptionsScene::SetButtonActions(const std::vector<Core::Button::ButtonAction>& actions)
{
	for (size_t i = 0; i < m_Buttons.size(); ++i)
		m_Buttons[i].SetButtonAction(actions[i]);
}

bool ButtonOptionsScene::Init()
{
	Core::KeyboardButtonAction upAction;
	upAction.Key = Core::GameController::UpKey();
	upAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		if (Core::GameController::IsPressed(state))
			SetPreviousButtonHighlighted();
	};
	m_GameController.AddKeyboardButtonAction(upAction);

	Core::KeyboardButtonAction downAction;
	downAction.Key = Core::GameController::DownKey();
	downAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		if (Core::GameController::IsPressed(state))
			SetNextButtonHighlighted();
	};
	m_GameController.AddKeyboardButtonAction(downAction);

	Core::KeyboardButtonAction acceptAction;
	acceptAction.Key = Core::GameController::ActionKey();
	acceptAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		if (Core::GameController::IsPressed(state))
			ExecuteCurrentButtonAction();
	};
	m_GameController.AddKeyboardButtonAction(acceptAction);

	uint32_t height = Core::Application::Get().GetWindow().GetHeight();
	uint32_t width = Core::Application::Get().GetWindow().GetWidth();

	const Core::BitmapFont& font = Core::Application::Get().GetFont();
	Core::Size fontSize = font.GetSizeOf(m_Buttons[0].GetButtonText());
	const int BUTTON_PAD = 10;
	unsigned int buttonHeight = fontSize.Height + BUTTON_PAD * 2;
	uint32_t maxButtonWidth = fontSize.Width;

	for (const auto& button : m_Buttons)
	{
		Core::Size s = font.GetSizeOf(button.GetButtonText());
		if (s.Width > maxButtonWidth)
			maxButtonWidth = s.Width;
	}
	maxButtonWidth += BUTTON_PAD * 2;
	const uint32_t Y_PAD = 1;
	uint32_t yOffset = height / 2 - ((buttonHeight + Y_PAD) * static_cast<uint32_t>(m_Buttons.size())) / 2;

	for (auto& button : m_Buttons)
	{
		button.Init(Core::Vec2D(width / 2 - maxButtonWidth / 2, yOffset), maxButtonWidth, buttonHeight);
		yOffset += buttonHeight + Y_PAD;
	}
	m_Buttons[m_HighlightedOption].SetHighlighted(true);

	return true;
}

void ButtonOptionsScene::Update(uint32_t dt)
{
}

void ButtonOptionsScene::Draw(Core::Window& window)
{
	for (auto& button : m_Buttons)
		button.Draw(window);
}

void ButtonOptionsScene::SetNextButtonHighlighted()
{
	m_HighlightedOption = (m_HighlightedOption + 1) % m_Buttons.size();
	HighlightCurrentButton();
}

void ButtonOptionsScene::SetPreviousButtonHighlighted()
{
	--m_HighlightedOption;

	if (m_HighlightedOption < 0)
		m_HighlightedOption = static_cast<int>(m_Buttons.size() - 1);

	HighlightCurrentButton();
}

void ButtonOptionsScene::HighlightCurrentButton()
{
	for (auto& button : m_Buttons)
		button.SetHighlighted(false);

	m_Buttons[m_HighlightedOption].SetHighlighted(true);
}

void ButtonOptionsScene::ExecuteCurrentButtonAction()
{
	m_Buttons[m_HighlightedOption].ExecuteAction();
}
