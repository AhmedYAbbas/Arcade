#include "Shape.h"

namespace Core
{
	void Shape::MoveBy(const Vec2D& delta)
	{
		for (Vec2D& point : m_Points)
			point += delta;
	}
}