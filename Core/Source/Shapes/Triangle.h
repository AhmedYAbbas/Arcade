#pragma once

#include "Shape.h"

namespace Core
{
	class Triangle : public Shape
	{
	public:
		Triangle();
		Triangle(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2);

		virtual Vec2D GetCenterPoint() const override;
		float Area() const;
		bool ContainsPoint(const Vec2D& p) const;

		inline void SetP0(const Vec2D& p0) { m_Points[0] = p0; }
		inline void SetP1(const Vec2D& p1) { m_Points[1] = p1; }
		inline void SetP2(const Vec2D& p2) { m_Points[2] = p2; }

		inline Vec2D GetP0() const { return m_Points[0]; }
		inline Vec2D GetP1() const { return m_Points[1]; }
		inline Vec2D GetP2() const { return m_Points[2]; }

	private:
		float Area(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2) const;
	};
}