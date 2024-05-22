#pragma once

#include <Core.h>

#include "Excluder.h"

class Ball;

class LevelBoundary
{
public:
	LevelBoundary() = default;
	LevelBoundary(const Core::Rectangle& boundary);

	bool HasCollided(const Ball& ball, Core::BoundaryEdge& edge);
	inline const Core::Rectangle& GetRectangle() const { return m_Includer.GetRectangle(); }

private:
	bool HasCollidedWithEdge(const Ball& ball, const Core::BoundaryEdge& edge) const;

private:
	Excluder m_Includer;
};