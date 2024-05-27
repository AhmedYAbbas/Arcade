#include "ArcadePCH.h"
#include "GameUtils.h"

Core::Vec2D GetMovementVector(PacmanMovement direction)
{
	if (direction == PacmanMovement::Left)
		return Core::Vec2D::Left;
	else if (direction == PacmanMovement::Down)
		return Core::Vec2D::Down;
	else if (direction == PacmanMovement::Right)
		return Core::Vec2D::Right;
	else if (direction == PacmanMovement::Up)
		return Core::Vec2D::Up;

	return Core::Vec2D::Zero;
}

PacmanMovement GetOppositeDirection(PacmanMovement direction)
{
	if (direction == PacmanMovement::Left)
		return PacmanMovement::Right;
	else if (direction == PacmanMovement::Down)
		return PacmanMovement::Up;
	else if (direction == PacmanMovement::Right)
		return PacmanMovement::Left;
	else if (direction == PacmanMovement::Up)
		return PacmanMovement::Down;

	return PacmanMovement::None;
}

std::vector<PacmanMovement> GetPerpendicularMovements(PacmanMovement direction)
{
	std::vector<PacmanMovement> perpendicularDirections;

	if (direction == PacmanMovement::Down || direction == PacmanMovement::Up)
	{
		perpendicularDirections.push_back(PacmanMovement::Left);
		perpendicularDirections.push_back(PacmanMovement::Right);
	}
	else if (direction == PacmanMovement::Left || direction == PacmanMovement::Right)
	{
		perpendicularDirections.push_back(PacmanMovement::Up);
		perpendicularDirections.push_back(PacmanMovement::Down);
	}
	else if (direction == PacmanMovement::None)
	{
		perpendicularDirections.push_back(PacmanMovement::Up);
		perpendicularDirections.push_back(PacmanMovement::Left);
		perpendicularDirections.push_back(PacmanMovement::Down);
		perpendicularDirections.push_back(PacmanMovement::Right);
	}

	return perpendicularDirections;
}
