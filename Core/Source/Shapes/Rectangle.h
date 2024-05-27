#pragma once

#include "Shape.h"

namespace Core
{
	class Rectangle : public Shape
	{
	public:
		Rectangle();
		Rectangle(const Vec2D& topLeft, unsigned int width, unsigned int height);
		Rectangle(const Vec2D& topLeft, const Vec2D& bottomRight);

		static Rectangle Inset(const Rectangle& rect, const Vec2D& insets);

		virtual Vec2D GetCenterPoint() const override;
		virtual const std::vector<Vec2D> GetPoints() const override;

		inline void SetTopLeftPoint(const Vec2D& topLeft) { m_Points[0] = topLeft; }
		inline void SetBottomRightPoint(const Vec2D& bottomRight) { m_Points[1] = bottomRight; }

		Vec2D GetTopLeftPoint() const { return m_Points[0]; }
		Vec2D GetBottomRightPoint() const { return m_Points[1]; }

		float GetWidth() const;
		float GetHeight() const;

		void MoveTo(const Vec2D& pos);

		bool Intersects(const Rectangle& other) const;
		bool ContainsPoint(const Vec2D p) const;
	};
}