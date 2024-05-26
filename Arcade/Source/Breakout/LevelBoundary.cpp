#include "ArcadePCH.h"
#include "LevelBoundary.h"
#include "Ball.h"

LevelBoundary::LevelBoundary(const Core::Rectangle& boundary)
{
	m_Includer.Init(boundary, true);
}

bool LevelBoundary::HasCollided(const Ball& ball, Core::BoundaryEdge& edge)
{
	for (int i = 0; i < Core::EdgeType::Num; ++i)
	{
		const Core::BoundaryEdge& collisionEdge = m_Includer.GetEdge(static_cast<Core::EdgeType>(i));
		if (HasCollidedWithEdge(ball, collisionEdge))
		{
			edge = collisionEdge;
			return true;
		}
	}
	return false;
}

bool LevelBoundary::HasCollidedWithEdge(const Ball& ball, const Core::BoundaryEdge& edge) const
{
	return edge.Edge.MinDistanceFrom(ball.GetPosition()) < ball.GetRadius();
}
