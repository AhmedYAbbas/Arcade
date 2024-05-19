#pragma once

#include "Utility/Vec2D.h"

namespace Core
{
	class Line
	{
	public:
		Line();
		Line(float x0, float y0, float x1, float y1);
		Line(const Vec2D& p0, const Vec2D& p1);

		inline const Vec2D& GetP0() const { return m_P0; }
		inline const Vec2D& GetP1() const { return m_P1; }

		inline void SetP0(const Vec2D& p0) { m_P0 = p0; }
		inline void SetP1(const Vec2D& p1) { m_P1 = p1; }

		float MinDistanceFrom(const Vec2D& p, bool limitToSegment = false) const;

		Vec2D ClosestPoint(const Vec2D& p, bool limitToSegment = false) const;

		Vec2D Midpoint() const;
		float Slope() const;
		float Length() const;

	private:
		Vec2D m_P0;
		Vec2D m_P1;
	};

	bool operator==(const Line& line1, const Line& line2);
}