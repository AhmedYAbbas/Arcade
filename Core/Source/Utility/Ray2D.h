#pragma once

#include "Vec2D.h"
#include "Easings.h"
#include "Shapes/Line.h"

namespace Core
{
	class Circle;

	class Ray2D
	{
	public:
		Ray2D();

		void Init(const Vec2D& origin, const Vec2D& velocity);

		Line GetLineSegmentForTime(float t, Ease::EasingFunc func = Ease::EaseLinear) const;

		bool Intersects(const Circle& circle, float& t1, float& t2) const;

		inline const Vec2D& GetVelocity() const { return m_Velocity; }
		inline const Vec2D& GetOrigin() const { return m_Origin; }

	private:
		Vec2D m_Origin;
		Vec2D m_Velocity;
	};
}