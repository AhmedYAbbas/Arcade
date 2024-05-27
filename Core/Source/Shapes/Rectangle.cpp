#include "CorePCH.h"
#include "Rectangle.h"

namespace Core
{
	Rectangle::Rectangle()
		: Rectangle(Vec2D::Zero, Vec2D::Zero)
	{
	}

	Rectangle::Rectangle(const Vec2D& topLeft, unsigned int width, unsigned int height)
	{
		m_Points.push_back(topLeft);
		m_Points.push_back(Vec2D(topLeft.GetX() + width - 1, topLeft.GetY() + height - 1));
	}

	Rectangle::Rectangle(const Vec2D& topLeft, const Vec2D& bottomRight)
	{
		m_Points.push_back(topLeft);
		m_Points.push_back(bottomRight);
	}

	Rectangle Rectangle::Inset(const Rectangle& rect, const Vec2D& insets)
	{
		return Rectangle(rect.GetTopLeftPoint() + insets, rect.GetWidth() - 2 * insets.GetX(), rect.GetHeight() - 2 * insets.GetY());
	}

	Vec2D Rectangle::GetCenterPoint() const
	{
		return Vec2D(GetTopLeftPoint().GetX() + GetWidth() / 2.f, GetTopLeftPoint().GetY() + GetHeight() / 2.f);
	}

	const std::vector<Vec2D> Rectangle::GetPoints() const
	{
		std::vector<Vec2D> points;
		points.push_back(m_Points[0]);
		points.push_back(Vec2D(m_Points[1].GetX(), m_Points[0].GetY()));
		points.push_back(m_Points[1]);
		points.push_back(Vec2D(m_Points[0].GetX(), m_Points[1].GetY()));
		return points;
	}

	float Rectangle::GetWidth() const
	{
		return GetBottomRightPoint().GetX() - GetTopLeftPoint().GetX() + 1;
	}

	float Rectangle::GetHeight() const
	{
		return GetBottomRightPoint().GetY() - GetTopLeftPoint().GetY() + 1;
	}

	void Rectangle::MoveTo(const Vec2D& pos)
	{
		float width = GetWidth();
		float height = GetHeight();

		SetTopLeftPoint(pos);
		SetBottomRightPoint(Vec2D(pos.GetX() + width - 1, pos.GetY() + height - 1));
	}

	bool Rectangle::Intersects(const Rectangle& other) const
	{
		return !(other.GetBottomRightPoint().GetX() < GetTopLeftPoint().GetX() ||
			other.GetTopLeftPoint().GetX() > GetBottomRightPoint().GetX() ||
			other.GetBottomRightPoint().GetY() < GetTopLeftPoint().GetY() ||
			other.GetTopLeftPoint().GetY() > GetBottomRightPoint().GetY());
	}

	bool Rectangle::ContainsPoint(const Vec2D p) const
	{
		bool withinX = p.GetX() >= GetTopLeftPoint().GetX() && p.GetX() <= GetBottomRightPoint().GetX();
		bool withinY = p.GetY() >= GetTopLeftPoint().GetY() && p.GetY() <= GetBottomRightPoint().GetY();
		return withinX && withinY;
	}
}