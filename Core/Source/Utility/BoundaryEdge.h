#pragma once

#include "Vec2D.h"
#include "Shapes/Line.h"

namespace Core
{
	enum EdgeType
	{
		Bottom = 0,
		Top,
		Left,
		Right,
		Num
	};

	struct BoundaryEdge
	{
		Vec2D Normal;
		Line Edge;
	};
}