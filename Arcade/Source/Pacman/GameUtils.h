#pragma once

static constexpr int GHOST_MOVEMENT_SPEED = 45;
static constexpr int GHOST_VULNERABLE_MOVEMENT_SPEED = 25;
static constexpr int GHOST_BACK_TO_PEN_SPEED = 100;

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
std::vector<PacmanMovement> GetOtherDirections(PacmanMovement direction);