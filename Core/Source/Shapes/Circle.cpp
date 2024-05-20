#include "Circle.h"

namespace Core
{
	Circle::Circle()
		: Circle(Vec2D::Zero, 0.f)
	{
	}

	Circle::Circle(const Vec2D& center, float radius)
		: m_Radius(radius)
	{
		m_Points.push_back(center);
	}

	bool Circle::Intersects(const Circle& other) const
	{
		return GetCenterPoint().Distance(other.GetCenterPoint()) < (m_Radius + other.m_Radius);
	}

	bool Circle::ContainsPoint(const Vec2D& p) const
	{
		return IsLessThanOrEqual(GetCenterPoint().Distance(p), m_Radius);
	}
}