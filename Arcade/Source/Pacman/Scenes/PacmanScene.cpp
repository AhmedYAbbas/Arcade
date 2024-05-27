#include "ArcadePCH.h"
#include "PacmanScene.h"

bool PacmanScene::Init()
{
	m_PacmanSpriteSheet.Load("PacmanSprites");
	m_Level.Init(Core::Application::Get().GetBasePath() + "Pacman_level.txt");
	m_Pacman.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Pacman_animations.txt", Core::Vec2D::Zero, m_Pacman.Speed(), false);
	ResetGame();

	Core::KeyboardButtonAction leftAction;
	leftAction.Key = Core::GameController::LeftKey();
	leftAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		HandleGameControllerState(dt, state, PacmanMovement::Left);
	};
	m_GameController.AddKeyboardButtonAction(leftAction);

	Core::KeyboardButtonAction rightAction;
	rightAction.Key = Core::GameController::RightKey();
	rightAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		HandleGameControllerState(dt, state, PacmanMovement::Right);
	};
	m_GameController.AddKeyboardButtonAction(rightAction);

	Core::KeyboardButtonAction upAction;
	upAction.Key = Core::GameController::UpKey();
	upAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		HandleGameControllerState(dt, state, PacmanMovement::Up);
	};
	m_GameController.AddKeyboardButtonAction(upAction);

	Core::KeyboardButtonAction downAction;
	downAction.Key = Core::GameController::DownKey();
	downAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		HandleGameControllerState(dt, state, PacmanMovement::Down);
	};
	m_GameController.AddKeyboardButtonAction(downAction);

	return true;
}

void PacmanScene::Update(uint32_t dt)
{
	m_Level.Update(dt);
	UpdatePacmanMovement();
	m_Pacman.Update(dt);
}

void PacmanScene::Draw(Core::Window& window)
{
	m_Level.Draw(window);
	m_Pacman.Draw(window);
}

const std::string& PacmanScene::GetSceneName() const
{
	static std::string sceneName = "Pacman!";
	return sceneName;
}

void PacmanScene::ResetGame()
{
	m_PressedDirection = PacmanMovement::None;
	m_Pacman.ResetScore();
}

void PacmanScene::UpdatePacmanMovement()
{
	if (m_PressedDirection != PacmanMovement::None)
		m_Pacman.SetMovementDirection(m_PressedDirection);
}

void PacmanScene::HandleGameControllerState(uint32_t dt, Core::InputState state, PacmanMovement direction)
{
	if (Core::GameController::IsPressed(state))
		m_PressedDirection = direction;
	else if (Core::GameController::IsReleased(state) && m_PressedDirection == direction)
		m_PressedDirection = PacmanMovement::None;
}
