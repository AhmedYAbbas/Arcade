#pragma once

#include "Shape.h"

namespace Core
{
	class Circle : public Shape
	{
	public:
		Circle();
		Circle(const Vec2D& center, float radius);

		inline virtual Vec2D GetCenterPoint() const override { return m_Points[0]; }

		inline float GetRadius() const { return m_Radius; }
		inline void SetRadius(float radius) { m_Radius = radius; }
		inline void MoveTo(const Vec2D& pos) { m_Points[0] = pos; }

		bool Intersects(const Circle& other) const;
		bool ContainsPoint(const Vec2D& p) const;

	private:
		float m_Radius;
	};
}
