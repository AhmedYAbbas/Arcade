#include "BreakoutScene.h"

void BreakoutScene::Init()
{
	m_GameController.Clear();
	ResetGameScene();

	Core::KeyboardButtonAction leftKeyAction;
	leftKeyAction.Key = Core::GameController::LeftKey();
	leftKeyAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		if (Core::GameController::IsPressed(state))
			m_Paddle.SetMovementDirection(PaddleDirection::Left);
		else
			m_Paddle.UnsetMovementDirection(PaddleDirection::Left);
	};
	m_GameController.AddKeyboardButtonAction(leftKeyAction);
	
	Core::KeyboardButtonAction rightKeyAction;
	rightKeyAction.Key = Core::GameController::RightKey();
	rightKeyAction.Action = [this](uint32_t dt, Core::InputState state)
	{
		if (Core::GameController::IsPressed(state))
			m_Paddle.SetMovementDirection(PaddleDirection::Right);
		else
			m_Paddle.UnsetMovementDirection(PaddleDirection::Right);
	};
	m_GameController.AddKeyboardButtonAction(rightKeyAction);
	
}

void BreakoutScene::Update(uint32_t dt)
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
}

void BreakoutScene::Draw(Core::Window& window)
{
	m_Ball.Draw(window);
	m_Paddle.Draw(window);
	window.Draw(m_LevelBoundary.GetRectangle(), Core::Color::White());
}

const std::string& BreakoutScene::GetSceneName() const
{
	static std::string sceneName = "Breakout";
	return sceneName;
}

void BreakoutScene::ResetGameScene()
{
	Core::Rectangle paddleRect(Core::Vec2D(Core::Application::Get().GetWindow().GetWidth() / 2 - Paddle::s_PaddleWidth / 2, Core::Application::Get().GetWindow().GetHeight() - 3 * Paddle::s_PaddleHeight), Paddle::s_PaddleWidth, Paddle::s_PaddleHeight);
	Core::Rectangle levelBoundary(Core::Vec2D::Zero, Core::Application::Get().GetWindow().GetWidth(), Core::Application::Get().GetWindow().GetHeight());

	m_LevelBoundary = levelBoundary;
	m_Paddle.Init(paddleRect, levelBoundary);

	const Core::Vec2D INITIAL_BALL_VELOCITY {100.f, -100.f};
	m_Ball.SetVelocity(INITIAL_BALL_VELOCITY);
	m_Ball.MoveTo(Core::Vec2D(Core::Application::Get().GetWindow().GetWidth() / 2, Core::Application::Get().GetWindow().GetHeight() / 2));
}
