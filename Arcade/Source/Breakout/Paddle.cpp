#include "Paddle.h"

void Paddle::Init(const Core::Rectangle& rect, const Core::Rectangle& boundary)
{
	Excluder::Init(rect);
	m_Boundary = boundary;
	m_Direction = 0;
}

void Paddle::Update(uint32_t dt)
{
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
