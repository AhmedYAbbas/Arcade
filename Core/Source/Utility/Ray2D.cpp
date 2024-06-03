#include "CorePCH.h"
#include "Ray2D.h"
#include "Utils.h"
#include "Shapes/Circle.h"

namespace Core
{
	Ray2D::Ray2D()
		: m_Origin(Vec2D::Zero), m_Velocity(Vec2D::Zero)
	{
	}

	void Ray2D::Init(const Vec2D& origin, const Vec2D& velocity)
	{
		m_Origin = origin;
		m_Velocity = velocity;
	}

	Line Ray2D::GetLineSegmentForTime(float t, Ease::EasingFunc func) const
	{
		if (t > 0)
		{
			Line line;
			Vec2D p2 = m_Origin + m_Velocity * func(t);

			line.SetP0(m_Origin);
			line.SetP1(p2);

			return line;
		}
		return Line();
	}

	bool Ray2D::Intersects(const Circle& circle, float& t1, float& t2) const
	{
		Vec2D OMinusC = m_Origin - circle.GetCenterPoint();
		float OMinusC2 = OMinusC.Dot(OMinusC);
		float Vel2 = m_Velocity.Dot(m_Velocity);
		float VelDotOMinusC = m_Velocity.Dot(OMinusC);
		float Radius2 = circle.GetRadius() * circle.GetRadius();

		float disc = VelDotOMinusC * VelDotOMinusC - Vel2 * (OMinusC2 - Radius2);
		if (disc < 0.0f)
			return false;

		float sqrtDisc = std::sqrt(disc);
		t1 = (-VelDotOMinusC - sqrtDisc) / Vel2;
		t2 = (-VelDotOMinusC + sqrtDisc) / Vel2;

		return true;
	}
}