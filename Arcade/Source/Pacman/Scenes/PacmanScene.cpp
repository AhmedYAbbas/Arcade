#include "ArcadePCH.h"
#include "PacmanScene.h"

static const std::string SCORE_STR = "Score  ";
static const std::string PACMAN_LIFE_SPRITE_NAME = "pac_man_left_idle";
static constexpr size_t MAX_NUM_LIVES = 3;
static constexpr uint32_t RELEASE_GHOST_TIMER = 5000;
static const std::string READY_STR = "Ready!";
static const std::string GAME_OVER_STR = "Game Over";
static constexpr uint32_t LEVEL_STARTING_TIME = 3000;

bool PacmanScene::Init()
{
	m_LevelStartingTimer = 0;
	m_ReleaseGhostTimer = 0;
	m_PacmanSpriteSheet.Load("PacmanSprites");
	m_Pacman.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Pacman_animations.txt", Core::Vec2D::Zero, m_Pacman.Speed(), false);
	m_Level.Init(Core::Application::Get().GetBasePath() + "Pacman_level.txt", m_PacmanSpriteSheet);

	m_StringRect = Core::Rectangle(Core::Vec2D(0, m_Level.GetInGameTextYPos()), Core::Application::Get().GetWindow().GetWidth(), m_Pacman.GetBoundingBox().GetHeight());

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

	Core::KeyboardButtonAction backAction;
	backAction.Key = Core::GameController::CancelKey();
	backAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		if (m_GameState == GameState::GameOver)
			Core::Application::Get().PopScene();
	};
	m_GameController.AddKeyboardButtonAction(backAction);

	return true;
}

void PacmanScene::Update(uint32_t dt)
{
	if (m_GameState == GameState::Starting)
	{
		m_Pacman.ResetGhostEatenMultiplier();
		m_LevelStartingTimer += dt;

		if (m_LevelStartingTimer >= LEVEL_STARTING_TIME)
		{
			m_LevelStartingTimer = 0;
			m_GameState = GameState::PlayGame;
		}
	}
	else if (m_GameState == GameState::PlayGame)
	{
		UpdatePacmanMovement();
		m_Pacman.Update(dt);

		m_ReleaseGhostTimer += dt;

		for (size_t i = 0; i < m_Ghosts.size(); ++i)
		{
			Ghost& ghost = m_Ghosts[i];
			GhostAI& ghostAI = m_GhostAIs[i];

			if (m_ReleaseGhostTimer >= RELEASE_GHOST_TIMER && ghostAI.IsInPen() && !ghost.IsReleased())
			{
				m_ReleaseGhostTimer = 0;
				ghost.ReleaseFromPen();
			}

			auto direction = ghostAI.Update(dt, m_Pacman, m_Level, m_Ghosts);
			if ((ghost.IsReleased() && ghost.CanChangeDirection()) || (ghost.IsReleased() && ghostAI.WantsToLeavePen() && direction != PacmanMovement::None))
			{
				if (direction != ghost.GetMovementDirection())
				{
					ghost.SetMovementDirection(direction);
					ghost.LockCanChangeDirection();
				}
			}

			ghost.Update(dt);
			if (ghost.IsVulnerable() && m_Pacman.GetEatingBoundingBox().Intersects(ghost.GetBoundingBox()))
			{
				ghost.EatenByPacman();
				m_Pacman.AteGhost(ghost.GetPoints());
			}
			else if (ghost.IsAlive() && ghost.GetEatingBoundingBox().Intersects(m_Pacman.GetBoundingBox()))
			{
				--m_NumLives;
				m_GameState = GameState::LostLife;
				m_Pacman.EatenByGhost();
				m_PressedDirection = PacmanMovement::None;
				m_Pacman.SetMovementDirection(PacmanMovement::None);
				return;
			}
		}

		m_Level.Update(dt, m_Pacman, m_Ghosts, m_GhostAIs);
		if (m_Level.IsLevelOver())
		{
			m_Level.IncreaseLevel();
			ResetLevel();
		}
	}
	else if (m_GameState == GameState::LostLife)
	{
		m_Pacman.Update(dt); // to update the death animation
		if (m_Pacman.IsFinishedAnimation())
		{
			if (m_NumLives > 0)
				ResetLevel();
			else
				m_GameState = GameState::GameOver;
		}
	}
}

void PacmanScene::Draw(Core::Window& window)
{
	m_Level.Draw(window);
	m_Pacman.Draw(window);

	for (auto& ghost : m_Ghosts)
		ghost.Draw(window);

	const auto& font = Core::Application::Get().GetFont();
	Core::Vec2D textDrawPosition;
	DrawScore(window, font, textDrawPosition);
	DrawText(window, font, textDrawPosition);
	
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
	m_ReleaseGhostTimer = 0;
	m_GameState = GameState::Starting;
	m_Pacman.MoveTo(m_Level.GetPacmanSpawnLocation());
	m_Pacman.ResetToFirstAnimation();

	for (auto& ghost : m_Ghosts)
		ghost.ResetToFirstPosition();

	m_Ghosts[static_cast<int>(GhostName::Blinky)].ReleaseFromPen();
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

void PacmanScene::DrawScore(Core::Window& window, const Core::BitmapFont& font, Core::Vec2D& textDrawPosition)
{
	Core::Vec2D levelOffset = m_Level.GetLayoutOffset();
	Core::Rectangle highScoreRect(Core::Vec2D(0, 4), Core::Application::Get().GetWindow().GetWidth(), levelOffset.GetY());

	std::string scoreStr = std::to_string(m_Pacman.Score());
	textDrawPosition = font.GetDrawPosition(SCORE_STR + scoreStr, highScoreRect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);

	window.Draw(font, SCORE_STR + scoreStr, textDrawPosition);
}

void PacmanScene::DrawText(Core::Window& window, const Core::BitmapFont& font, Core::Vec2D& textDrawPosition)
{
	if (m_GameState == GameState::Starting)
	{
		textDrawPosition = font.GetDrawPosition(READY_STR, m_StringRect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);
		window.Draw(font, READY_STR, textDrawPosition, Core::Color::Yellow());
	}
	else if (m_GameState == GameState::GameOver)
	{
		textDrawPosition = font.GetDrawPosition(GAME_OVER_STR, m_StringRect, Core::BitmapFontXAlignment::Center, Core::BitmapFontYAlignment::Center);
		window.Draw(font, GAME_OVER_STR, textDrawPosition, Core::Color::Yellow());
	}
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
	m_GhostAIs.resize(static_cast<int>(GhostName::Num));

	Ghost blinky;
	blinky.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Ghost_animations.txt", m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Blinky)], GHOST_MOVEMENT_SPEED, true, Core::Color::Red());
	blinky.SetMovementDirection(PacmanMovement::Left);
	m_Ghosts[static_cast<int>(GhostName::Blinky)] = blinky;

	auto blinkyAI = GhostAI();
	blinkyAI.Init(m_Ghosts[static_cast<int>(GhostName::Blinky)], blinky.GetBoundingBox().GetWidth(), BLINKY_SCATTER_POS, m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Pinky)], m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Blinky)], GhostName::Blinky);
	m_GhostAIs[static_cast<int>(GhostName::Blinky)] = blinkyAI;

	Ghost pinky;
	pinky.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Ghost_animations.txt", m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Pinky)], GHOST_MOVEMENT_SPEED, true, Core::Color::Pink());
	pinky.SetMovementDirection(PacmanMovement::Down);
	m_Ghosts[static_cast<int>(GhostName::Pinky)] = pinky;

	auto pinkyAI = GhostAI();
	pinkyAI.Init(m_Ghosts[static_cast<int>(GhostName::Pinky)], pinky.GetBoundingBox().GetWidth(), PINKY_SCATTER_POS, m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Pinky)], m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Blinky)], GhostName::Pinky);
	m_GhostAIs[static_cast<int>(GhostName::Pinky)] = pinkyAI;

	Ghost inky;
	inky.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Ghost_animations.txt", m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Inky)], GHOST_MOVEMENT_SPEED, true, Core::Color::Cyan());
	inky.SetMovementDirection(PacmanMovement::Up);
	m_Ghosts[static_cast<int>(GhostName::Inky)] = inky;

	auto inkyAI = GhostAI();
	inkyAI.Init(m_Ghosts[static_cast<int>(GhostName::Inky)], inky.GetBoundingBox().GetWidth(), INKY_SCATTER_POS, m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Pinky)], m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Blinky)], GhostName::Inky);
	m_GhostAIs[static_cast<int>(GhostName::Inky)] = inkyAI;

	Ghost clyde;
	clyde.Init(m_PacmanSpriteSheet, Core::Application::Get().GetBasePath() + "Ghost_animations.txt", m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Clyde)], GHOST_MOVEMENT_SPEED, true, Core::Color::Orange());
	clyde.SetMovementDirection(PacmanMovement::Up);
	m_Ghosts[static_cast<int>(GhostName::Clyde)] = clyde;

	auto clydeAI = GhostAI();
	clydeAI.Init(m_Ghosts[static_cast<int>(GhostName::Clyde)], clyde.GetBoundingBox().GetWidth(), CLYDE_SCATTER_POS, m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Pinky)], m_Level.GetGhostSpwanPoints()[static_cast<int>(GhostName::Blinky)], GhostName::Clyde);
	m_GhostAIs[static_cast<int>(GhostName::Clyde)] = clydeAI;

	for (size_t i = 0; i < m_GhostAIs.size(); ++i)
		m_Ghosts[i].SetGhostDelegate(m_GhostAIs[i]);
}
