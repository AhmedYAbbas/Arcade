#include "Triangle.h"

namespace Core
{
	Triangle::Triangle()
		: Triangle(Vec2D::Zero, Vec2D::Zero, Vec2D::Zero)
	{
	}

	Triangle::Triangle(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2)
	{
		m_Points.push_back(p0);
		m_Points.push_back(p1);
		m_Points.push_back(p2);
	}

	Vec2D Triangle::GetCenterPoint() const
	{
		return Vec2D((m_Points[0].GetX() + m_Points[1].GetX() + m_Points[2].GetX()) / 3.f, (m_Points[0].GetY() + m_Points[1].GetY() + m_Points[2].GetY()) / 3.f);
	}

	float Triangle::Area() const
	{
		return Area(GetP0(), GetP1(), GetP2());
	}

	bool Triangle::ContainsPoint(const Vec2D& p) const
	{
		float area = Area();
		float a1 = Area(p, GetP1(), GetP2());
		float a2 = Area(GetP0(), p, GetP2());
		float a3 = Area(GetP0(), GetP1(), p);

		return IsEqual(area, a1 + a2 + a3);
	}

	float Triangle::Area(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2) const
	{
		return std::fabsf(p0.GetX() * (p1.GetY() - p2.GetY()) + p1.GetX() * (p2.GetY() - p0.GetY()) + p2.GetX() * (p0.GetY() - p1.GetY()) / 2.f);
	}
}