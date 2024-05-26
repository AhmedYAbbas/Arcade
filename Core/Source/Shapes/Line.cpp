#include "CorePCH.h"
#include "Line.h"

namespace Core
{
	Line::Line()
		: Line(Vec2D::Zero, Vec2D::Zero)
	{
	}

	Line::Line(float x0, float y0, float x1, float y1)
		: Line(Vec2D(x0, y0), Vec2D(x1, y1))
	{
	}

	Line::Line(const Vec2D& p0, const Vec2D& p1)
		: m_P0(p0), m_P1(p1)
	{
	}

	float Line::MinDistanceFrom(const Vec2D& p, bool limitToSegment) const
	{
		return p.Distance(ClosestPoint(p, limitToSegment));
	}

	Vec2D Line::ClosestPoint(const Vec2D& p, bool limitToSegment) const
	{
		Vec2D p0ToP = p - m_P0;
		Vec2D p0ToP1 = m_P1 - m_P0;

		float l2 = p0ToP1.Mag2();
		float dot = p0ToP.Dot(p0ToP1);
		float t = dot / l2;

		if (limitToSegment)
			t = std::fmax(0, std::fmin(1.0f, t));

		return m_P0 + p0ToP1 * t;
	}

	Vec2D Line::Midpoint() const
	{
		return Vec2D((m_P0.GetX() + m_P1.GetX()) / 2.0f, (m_P0.GetY() + m_P1.GetY()) / 2.0f);
	}

	float Line::Slope() const
	{
		float dx = m_P1.GetX() - m_P0.GetX();
		float dy = m_P1.GetY() - m_P0.GetY();

		if (fabsf(dx) < EPSILON)
			return 0;

		return dy / dx;
	}

	float Line::Length() const
	{
		return m_P1.Distance(m_P0);
	}

	bool operator==(const Line& line1, const Line& line2)
	{
		return line1.GetP0() == line2.GetP0() && line1.GetP1() == line2.GetP1();
	}
}