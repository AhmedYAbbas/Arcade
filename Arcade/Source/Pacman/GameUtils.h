#pragma once

enum class PacmanMovement
{
	None = 0,
	Up,
	Left,
	Down,
	Right
};

Core::Vec2D GetMovementVector(PacmanMovement direction);
PacmanMovement GetOppositeDirection(PacmanMovement direction);
std::vector<PacmanMovement> GetPerpendicularMovements(PacmanMovement direction);