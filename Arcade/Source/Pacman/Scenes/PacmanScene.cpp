#include "ArcadePCH.h"
#include "PacmanScene.h"

static const std::string SCORE_STR = "Score  ";
static const std::string PACMAN_LIFE_SPRITE_NAME = "pac_man_left_idle";
static constexpr size_t MAX_NUM_LIVES = 3;

bool PacmanScene::Init()
{
	m_PacmanSpriteSheet.Load("PacmanSprites");
	m_Pacman.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Pacman_animations.txt", Core::Vec2D::Zero, m_Pacman.Speed(), false);
	m_Level.Init(Core::Application::Get().GetBasePath() + "Pacman_level.txt", m_PacmanSpriteSheet);
	SetupGhosts();
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
	UpdatePacmanMovement();
	m_Pacman.Update(dt);

	for (size_t i = 0; i < m_Ghosts.size(); ++i)
	{
		if (static_cast<GhostName>(i) == GhostName::Blinky)
		{
			GhostAI& ghostAI = m_GhostAIs[i];
			auto direction = ghostAI.Update(dt, m_Pacman, m_Level, m_Ghosts);
			if (m_Ghosts[i].CanChangeDirection() && direction != m_Ghosts[i].GetMovementDirection())
				m_Ghosts[i].SetMovementDirection(direction);
		}

		m_Ghosts[i].Update(dt);
	}

	m_Level.Update(dt, m_Pacman, m_Ghosts);
	if (m_Level.IsLevelOver())
	{
		m_Level.IncreaseLevel();
		ResetLevel();
	}
}

void PacmanScene::Draw(Core::Window& window)
{
	m_Level.Draw(window);
	m_Pacman.Draw(window);

	for (auto& ghost : m_Ghosts)
		ghost.Draw(window);

	for (auto& ghostAI : m_GhostAIs)
		ghostAI.Draw(window);

	DrawScore(window);
	DrawLives(window);
}

const std::string& PacmanScene::GetSceneName() const
{
	static std::string sceneName = "Pacman!";
	return sceneName;
}

void PacmanScene::ResetGame()
{
	m_NumLives = MAX_NUM_LIVES;
	m_PressedDirection = PacmanMovement::None;
	m_Pacman.ResetScore();
	m_Level.ResetToFirstLevel();
	ResetLevel();
}

void PacmanScene::ResetLevel()
{
	m_Pacman.MoveTo(m_Level.GetPacmanSpawnLocation());
	m_Pacman.ResetToFirstAnimation();
}

void PacmanScene::UpdatePacmanMovement()
{
	if (m_PressedDirection != PacmanMovement::None)
	{
		if (!m_Level.WillCollide(m_Pacman.GetBoundingBox(), m_PressedDirection))
			m_Pacman.SetMovementDirection(m_PressedDirection);
	}
}

void PacmanScene::HandleGameControllerState(uint32_t dt, Core::InputState state, PacmanMovement direction)
{
	if (Core::GameController::IsPressed(state))
		m_PressedDirection = direction;
	else if (Core::GameController::IsReleased(state) && m_PressedDirection == direction)
		m_PressedDirection = PacmanMovement::None;
}

void PacmanScene::DrawScore(Core::Window& window)
{
	Core::Vec2D levelOffset = m_Level.GetLayoutOffset();
	Core::Rectangle highScoreRect(Core::Vec2D(0, 4), Core::Application::Get().GetWindow().GetWidth(), levelOffset.GetY());

	const auto& font = Core::Application::Get().GetFont();
	Core::Vec2D textDrawPosition;

	std::string scoreStr = std::to_string(m_Pacman.Score());
	textDrawPosition = font.GetDrawPosition(SCORE_STR + scoreStr, highScoreRect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);

	window.Draw(font, SCORE_STR + scoreStr, textDrawPosition);
}

void PacmanScene::DrawLives(Core::Window& window)
{
	constexpr uint32_t X_PAD = 1;
	auto sprite = m_PacmanSpriteSheet.GetSprite(PACMAN_LIFE_SPRITE_NAME);
	uint32_t xPos = X_PAD;
	for (size_t i = 0; i < m_NumLives; ++i)
	{
		window.Draw(m_PacmanSpriteSheet, PACMAN_LIFE_SPRITE_NAME, Core::Vec2D(xPos, Core::Application::Get().GetWindow().GetHeight() - sprite.height));
		xPos += X_PAD + sprite.width;
	}
}

void PacmanScene::SetupGhosts()
{
	const Core::Vec2D BLINKY_SCATTER_POS(Core::Application::Get().GetWindow().GetWidth() - 24, 0);
	const Core::Vec2D INKY_SCATTER_POS(Core::Application::Get().GetWindow().GetWidth(), Core::Application::Get().GetWindow().GetHeight());
	const Core::Vec2D PINKY_SCATTER_POS(24, 0);
	const Core::Vec2D CLYDE_SCATTER_POS(0, Core::Application::Get().GetWindow().GetHeight());

	m_Ghosts.resize(static_cast<int>(GhostName::Num));
	m_GhostAIs.resize(1);

	Ghost blinky;
	blinky.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Ghost_animations.txt", m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Blinky)], GHOST_MOVEMENT_SPEED, true, Core::Color::Red());
	blinky.SetMovementDirection(PacmanMovement::Left);
	m_Ghosts[static_cast<int>(GhostName::Blinky)] = blinky;

	auto blinkyAI = GhostAI();
	blinkyAI.Init(m_Ghosts[static_cast<int>(GhostName::Blinky)], blinky.GetBoundingBox().GetWidth(), BLINKY_SCATTER_POS, GhostName::Blinky);
	m_GhostAIs[static_cast<int>(GhostName::Blinky)] = blinkyAI;

	Ghost pinky;
	pinky.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Ghost_animations.txt", m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Pinky)], GHOST_MOVEMENT_SPEED, true, Core::Color::Pink());
	pinky.SetMovementDirection(PacmanMovement::Down);
	m_Ghosts[static_cast<int>(GhostName::Pinky)] = pinky;

	Ghost inky;
	inky.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Ghost_animations.txt", m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Inky)], GHOST_MOVEMENT_SPEED, true, Core::Color::Cyan());
	inky.SetMovementDirection(PacmanMovement::Up);
	m_Ghosts[static_cast<int>(GhostName::Inky)] = inky;

	Ghost clyde;
	clyde.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Ghost_animations.txt", m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Clyde)], GHOST_MOVEMENT_SPEED, true, Core::Color::Orange());
	clyde.SetMovementDirection(PacmanMovement::Up);
	m_Ghosts[static_cast<int>(GhostName::Clyde)] = clyde;
}
