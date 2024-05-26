#include "ArcadePCH.h"
#include "Ball.h"

Ball::Ball()
	: Ball(Core::Vec2D::Zero, 5.f)
{
}

Ball::Ball(const Core::Vec2D& pos, float radius)
	: m_BoundingBox(pos - Core::Vec2D(radius, radius), radius * 2.f, radius * 2.f), m_Velocity(Core::Vec2D::Zero)
{
}

void Ball::Update(uint32_t dt)
{
	m_BoundingBox.MoveBy(m_Velocity * Core::MillisecondsToSeconds(dt));
}

void Ball::Draw(Core::Window& window)
{
	Core::Circle circle(m_BoundingBox.GetCenterPoint(), GetRadius());
	window.Draw(circle, Core::Color::Red(), true, Core::Color::Red());
}

void Ball::MakeFlushWithEdge(const Core::BoundaryEdge& edge, Core::Vec2D& pointOnEdge, bool limitToEdge)
{
	if (edge.Normal == Core::Vec2D::Down)
		m_BoundingBox.MoveTo(Core::Vec2D(m_BoundingBox.GetTopLeftPoint().GetX(), edge.Edge.GetP0().GetY() + edge.Normal.GetY()));
	else if (edge.Normal == Core::Vec2D::Up)
		m_BoundingBox.MoveTo(Core::Vec2D(m_BoundingBox.GetTopLeftPoint().GetX(), edge.Edge.GetP0().GetY() - m_BoundingBox.GetHeight()));
	else if (edge.Normal == Core::Vec2D::Right)
		m_BoundingBox.MoveTo(Core::Vec2D(edge.Edge.GetP0().GetX() + edge.Normal.GetX(), m_BoundingBox.GetTopLeftPoint().GetY()));
	else if (edge.Normal == Core::Vec2D::Left)
		m_BoundingBox.MoveTo(Core::Vec2D(edge.Edge.GetP0().GetX() - m_BoundingBox.GetWidth(), m_BoundingBox.GetTopLeftPoint().GetY()));

	pointOnEdge = edge.Edge.ClosestPoint(m_BoundingBox.GetCenterPoint(), limitToEdge);
}

void Ball::MoveTo(const Core::Vec2D& point)
{
	m_BoundingBox.MoveTo(point - Core::Vec2D(GetRadius(), GetRadius()));
}

void Ball::Bounce(const Core::BoundaryEdge& edge)
{
	Core::Vec2D pointOnEdge;
	MakeFlushWithEdge(edge, pointOnEdge, false);
	m_Velocity = m_Velocity.Reflect(edge.Normal);
}
