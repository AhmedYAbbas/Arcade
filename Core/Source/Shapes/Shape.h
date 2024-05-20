#pragma once

#include <vector>

#include "Utility/Vec2D.h"

namespace Core
{
	class Shape
	{
	public:
		virtual ~Shape() = default;

		virtual Vec2D GetCenterPoint() const = 0;
		inline virtual const std::vector<Vec2D> GetPoints() const { return m_Points; }
		void MoveBy(const Vec2D& delta);

	protected:
		std::vector<Vec2D> m_Points;
	};
}