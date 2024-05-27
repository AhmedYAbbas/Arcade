#include "ArcadePCH.h"
#include "Pacman.h"

void Pacman::Init(const Core::SpriteSheet& spriteSheet, const std::string& animationsPath, const Core::Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Core::Color& spriteColor)
{
	Actor::Init(spriteSheet, animationsPath, initialPos, movementSpeed, updateSpriteOnMovement, spriteColor);

	ResetToFirstAnimation();
	ResetScore();
	ResetGhostEatenMultiplier();
	m_IsDying = false;
}

void Pacman::Update(uint32_t dt)
{
	if (m_IsDying)
	{
		m_Sprite.Update(dt);
		m_IsDying = !IsFinishedAnimation();
		return;
	}

	Actor::Update(dt);
}

void Pacman::SetMovementDirection(PacmanMovement direction)
{
	PacmanMovement currentDirection = GetMovementDirection();
	if (direction == PacmanMovement::Left && currentDirection != PacmanMovement::Left)
	{
		SetAnimation("move_left", true);
		ResetDelta();
	}
	else if (direction == PacmanMovement::Right && currentDirection != PacmanMovement::Right)
	{
		SetAnimation("move_right", true);
		ResetDelta();
	}
	else if (direction == PacmanMovement::Down && currentDirection != PacmanMovement::Down)
	{
		SetAnimation("move_down", true);
		ResetDelta();
	}
	else if (direction == PacmanMovement::Up && currentDirection != PacmanMovement::Up)
	{
		SetAnimation("move_up", true);
		ResetDelta();
	}

	Actor::SetMovementDirection(direction);
}

void Pacman::ResetToFirstAnimation()
{
	SetAnimation("move_left", true);
	Stop();
}

void Pacman::EatenByGhost()
{
	SetAnimation("death", false);
	m_IsDying = true;
	ResetGhostEatenMultiplier();
}

void Pacman::ResetScore()
{
	m_Score = 0;
}

void Pacman::AteItem(uint32_t value)
{
	AddToScore(value);
}

void Pacman::AteGhost(uint32_t value)
{
	AddToScore(value * m_GhostMultiplier);
}

void Pacman::AddToScore(uint32_t value)
{
	m_Score += value;
}
