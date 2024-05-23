#include "BreakoutScene.h"

const float INITIAL_BALL_SPEED = 100.f;
const int MAX_LIVES = 3;

void BreakoutScene::Init()
{
	m_GameController.Clear();
	ResetGameScene();

	Core::KeyboardButtonAction serverKeyAction;
	serverKeyAction.Key = Core::GameController::ActionKey();
	serverKeyAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		if (m_GameState == BreakoutGameState::Serve)
		{
			if (Core::GameController::IsPressed(state))
			{
				m_GameState = BreakoutGameState::Play;
				if (m_Paddle.IsMovingLeft())
					m_Ball.SetVelocity(Core::Vec2D(-INITIAL_BALL_SPEED, -INITIAL_BALL_SPEED));
				else
				{
					m_Ball.SetVelocity(Core::Vec2D(INITIAL_BALL_SPEED, -INITIAL_BALL_SPEED));
				}
			}
		}
		else if (m_GameState == BreakoutGameState::GameOver)
		{
			if (Core::GameController::IsPressed(state))
				ResetGameScene();
		}
			
	};
	m_GameController.AddKeyboardButtonAction(serverKeyAction);

	Core::KeyboardButtonAction leftKeyAction;
	leftKeyAction.Key = Core::GameController::LeftKey();
	leftKeyAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		if (m_GameState == BreakoutGameState::Play || m_GameState == BreakoutGameState::Serve)
		{
			if (Core::GameController::IsPressed(state))
				m_Paddle.SetMovementDirection(PaddleDirection::Left);
			else
				m_Paddle.UnsetMovementDirection(PaddleDirection::Left);
		}
	};
	m_GameController.AddKeyboardButtonAction(leftKeyAction);
	
	Core::KeyboardButtonAction rightKeyAction;
	rightKeyAction.Key = Core::GameController::RightKey();
	rightKeyAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		if (m_GameState == BreakoutGameState::Play || m_GameState == BreakoutGameState::Serve)
		{
			if (Core::GameController::IsPressed(state))
				m_Paddle.SetMovementDirection(PaddleDirection::Right);
			else
				m_Paddle.UnsetMovementDirection(PaddleDirection::Right);
	}
	};
	m_GameController.AddKeyboardButtonAction(rightKeyAction);
	
}

void BreakoutScene::Update(uint32_t dt)
{
	if (m_GameState == BreakoutGameState::Serve)
	{
		m_Paddle.Update(dt, m_Ball);
		SetToServeState();
	}
	else if (m_GameState == BreakoutGameState::Play)
	{
		m_Ball.Update(dt);
		m_Paddle.Update(dt, m_Ball);

		if (m_Paddle.Bounce(m_Ball))
			return;

		Core::BoundaryEdge edge;
		if (m_LevelBoundary.HasCollided(m_Ball, edge))
		{
			m_Ball.Bounce(edge);
			return;
		}

		GetCurrentLevel().Update(dt, m_Ball);

		if (IsBallPassedCutoffY())
		{
			ReduceLifeByOne();
			if (!IsGameOver())
				SetToServeState();
			else
				m_GameState = BreakoutGameState::GameOver;
		}
		else if (GetCurrentLevel().IsLevelComplete())
		{
			m_CurrentLevel = (m_CurrentLevel + 1) % m_Levels.size();
			ResetGameScene(m_CurrentLevel);
		}
	}
}

void BreakoutScene::Draw(Core::Window& window)
{
	m_Ball.Draw(window);
	m_Paddle.Draw(window);
	GetCurrentLevel().Draw(window);

	window.Draw(m_LevelBoundary.GetRectangle(), Core::Color::White());

	Core::Circle lifeCircle(Core::Vec2D(7, Core::Application::Get().GetWindow().GetHeight() - 10), 5);
	for (int i = 0; i < m_Lives; ++i)
	{
		window.Draw(lifeCircle, Core::Color::Red(), true, Core::Color::Red());
		lifeCircle.MoveBy(Core::Vec2D(17, 0));
	}
}

const std::string& BreakoutScene::GetSceneName() const
{
	static std::string sceneName = "Breakout";
	return sceneName;
}

void BreakoutScene::ResetGameScene(size_t toLevel)
{
	m_Levels = BreakoutGameLevel::LoadLevelsFromFile(Core::Application::GetBasePath() + "BreakoutLevels.txt");
	m_CurrentLevel = toLevel;

	m_YCutoff = Core::Application::Get().GetWindow().GetHeight() - 2 * Paddle::s_PaddleHeight;
	m_Lives = MAX_LIVES;

	Core::Rectangle paddleRect(Core::Vec2D(Core::Application::Get().GetWindow().GetWidth() / 2 - Paddle::s_PaddleWidth / 2, Core::Application::Get().GetWindow().GetHeight() - 3 * Paddle::s_PaddleHeight), Paddle::s_PaddleWidth, Paddle::s_PaddleHeight);
	Core::Rectangle levelBoundary(Core::Vec2D::Zero, Core::Application::Get().GetWindow().GetWidth(), Core::Application::Get().GetWindow().GetHeight());

	m_LevelBoundary = levelBoundary;
	m_Paddle.Init(paddleRect, levelBoundary);

	m_Ball.SetVelocity(Core::Vec2D(INITIAL_BALL_SPEED, -INITIAL_BALL_SPEED));
	m_Ball.MoveTo(Core::Vec2D(Core::Application::Get().GetWindow().GetWidth() / 2, Core::Application::Get().GetWindow().GetHeight() / 2));

	SetToServeState();
}

void BreakoutScene::SetToServeState()
{
	m_GameState = BreakoutGameState::Serve;
	m_Ball.Stop();

	m_Ball.MoveTo(Core::Vec2D(m_Paddle.GetRectangle().GetCenterPoint().GetX(), m_Paddle.GetRectangle().GetTopLeftPoint().GetY() - m_Ball.GetRadius() - 1));
}

bool BreakoutScene::IsBallPassedCutoffY() const
{
	return m_Ball.GetPosition().GetY() > m_YCutoff;;
}

void BreakoutScene::ReduceLifeByOne()
{
	if (m_Lives >= 0)
		--m_Lives;
}
