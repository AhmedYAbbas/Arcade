#include "ArcadePCH.h"
#include "Ghost.h"

static constexpr uint32_t NUM_POINTS_FOR_GHOST = 200;

Ghost::Ghost()
	: m_Points(0), m_InitialPos(Core::Vec2D::Zero)
{
}

void Ghost::Init(const Core::SpriteSheet& spriteSheet, const std::string& animationsPath, const Core::Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Core::Color& spriteColor)
{
	Actor::Init(spriteSheet, animationsPath, initialPos, movementSpeed, updateSpriteOnMovement, spriteColor);
	m_InitialPos = initialPos;
	m_Points = NUM_POINTS_FOR_GHOST;
	ResetToFirstPosition();
}

void Ghost::Update(uint32_t dt)
{
	Core::Vec2D position = Position();
	Actor::Update(dt);
	m_CanChangeDirection = position != Position();

	if (IsVulnerable())
	{
		m_VulnerabilityTimer += dt;
		if (m_State == GhostState::Vulnerable && m_VulnerabilityTimer >= VULNERABILITY_TIME)
			SetGhostState(GhostState::VulnerableEnding);

		if (m_State == GhostState::VulnerableEnding && m_VulnerabilityTimer >= VULNERABILITY_ENDING_TIME)
			SetGhostState(GhostState::Alive);
	}
}

void Ghost::Stop()
{
	SetMovementDirection(PacmanMovement::None);
}

void Ghost::SetMovementDirection(PacmanMovement direction)
{
	Actor::SetMovementDirection(direction);

	PacmanMovement movementDir = GetMovementDirection();
	if (m_State == GhostState::Alive)
	{
		if (movementDir == PacmanMovement::Right)
			SetAnimation("ghost_movement_right", true);
		else if (movementDir == PacmanMovement::Up)
			SetAnimation("ghost_movement_up", true);
		else if (movementDir == PacmanMovement::Down)
			SetAnimation("ghost_movement_down", true);
		else if (movementDir == PacmanMovement::Left)
			SetAnimation("ghost_movement_left", true);

	}
	else if (m_State == GhostState::Dead)
	{
		if (movementDir == PacmanMovement::Right)
			SetAnimation("ghost_dead_right", true);
		else if (movementDir == PacmanMovement::Up)
			SetAnimation("ghost_dead_up", true);
		else if (movementDir == PacmanMovement::Down)
			SetAnimation("ghost_dead_down", true);
		else if (movementDir == PacmanMovement::Left)
			SetAnimation("ghost_dead_left", true);
	}
}

void Ghost::SetStateToVulnerable()
{
	if (m_State != GhostState::Dead)
		SetGhostState(GhostState::Vulnerable);
}

void Ghost::EatenByPacman()
{
	SetGhostState(GhostState::Dead);
}

void Ghost::ResetToFirstPosition()
{
	m_Sprite.SetPosition(m_InitialPos);
	SetMovementDirection(PacmanMovement::None);
	m_VulnerabilityTimer = 0;
	SetGhostState(GhostState::Alive);
	m_CanChangeDirection = true;
}

void Ghost::SetGhostState(GhostState state)
{
	m_State = state;
	switch (m_State)
	{
		case GhostState::Alive:
			SetMovementDirection(GetMovementDirection());
			SetMovementSpeed(GHOST_MOVEMENT_SPEED);
			break;
		case GhostState::Vulnerable:
			m_Sprite.SetAnimation("ghost_vulnerable", true);
			m_VulnerabilityTimer = 0;
			SetMovementSpeed(GHOST_VULNERABLE_MOVEMENT_SPEED);
			break;
		case GhostState::VulnerableEnding:
			m_Sprite.SetAnimation("ghost_vulnerable_ending", true);
			m_VulnerabilityTimer = 0;
			break;
		case GhostState::Dead:
			SetMovementDirection(GetMovementDirection());
			SetMovementSpeed(GHOST_BACK_TO_PEN_SPEED);
			break;
		default:
			break;
	}
}
