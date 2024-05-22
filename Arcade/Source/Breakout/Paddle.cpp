#include "Paddle.h"
#include "Ball.h"

void Paddle::Init(const Core::Rectangle& rect, const Core::Rectangle& boundary)
{
	Excluder::Init(rect);
	m_Boundary = boundary;
	m_Direction = 0;
}

void Paddle::Update(uint32_t dt, Ball& ball)
{
	if (GetRectangle().ContainsPoint(ball.GetPosition()))
	{
		Core::Vec2D pointOnEdge;
		ball.MakeFlushWithEdge(GetEdge(Core::EdgeType::Bottom), pointOnEdge, true);
	}

	if (m_Direction != 0)
	{
		Core::Vec2D dir;
		if ((m_Direction & (PaddleDirection::Left | PaddleDirection::Right)) == (PaddleDirection::Left | PaddleDirection::Right))
			dir = Core::Vec2D::Zero;
		else if (m_Direction == PaddleDirection::Left)
			dir = Core::Vec2D::Left;
		else
			dir = Core::Vec2D::Right;

		Core::Vec2D dx = dir * m_Velocity * Core::MillisecondsToSeconds(dt);
		MoveBy(dx);

		const Core::Rectangle& rect = GetRectangle();
		if (Core::IsGreaterThanOrEqual(m_Boundary.GetTopLeftPoint().GetX(), rect.GetTopLeftPoint().GetX()))
			MoveTo(Core::Vec2D(m_Boundary.GetTopLeftPoint().GetX(), rect.GetTopLeftPoint().GetY()));
		else if (Core::IsGreaterThanOrEqual(rect.GetBottomRightPoint().GetX(), m_Boundary.GetBottomRightPoint().GetX()))
			MoveTo(Core::Vec2D(m_Boundary.GetBottomRightPoint().GetX() - rect.GetWidth(), rect.GetTopLeftPoint().GetY()));
	}
}

void Paddle::Draw(Core::Window& window)
{
	window.Draw(GetRectangle(), Core::Color::Blue(), true, Core::Color::Blue());
}

bool Paddle::Bounce(Ball& ball)
{
	Core::BoundaryEdge edge;
	if (HasCollided(ball.GetBoundingBox(), edge))
	{
		Core::Vec2D pointOnEdge;
		ball.MakeFlushWithEdge(edge, pointOnEdge, true);

		if (edge.Edge == GetEdge(Core::EdgeType::Top).Edge)
		{
			float edgeLength = edge.Edge.Length();
			assert(!Core::IsEqual(edgeLength, 0));
			float tx = (pointOnEdge.GetX() - edge.Edge.GetP0().GetX()) / edgeLength;

			const float CORNER_BOUNCE_AMOUT = 0.2f;
			if (((tx <= CORNER_BOUNCE_AMOUT) && ball.GetVelocity().GetX() > 0.f) || (tx >= (1.f - CORNER_BOUNCE_AMOUT) && ball.GetVelocity().GetX() < 0.f))
			{
				ball.SetVelocity(-ball.GetVelocity());
				return true;
			}
		}
		ball.SetVelocity(ball.GetVelocity().Reflect(edge.Normal));
		return true;
	}
	return false;
}
