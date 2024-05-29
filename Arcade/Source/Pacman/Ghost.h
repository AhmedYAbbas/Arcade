#pragma once

#include "Actor.h"
#include "GameUtils.h"

enum class GhostName
{
	Blinky = 0,
	Pinky,
	Inky,
	Clyde,
	Num
};

enum class GhostState
{
	Alive = 0,
	Vulnerable,
	VulnerableEnding,
	Dead
};


class Ghost : public Actor
{
public:
	Ghost();

	virtual void Init(const Core::SpriteSheet& spriteSheet, const std::string& animationsPath, const Core::Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Core::Color& spriteColor = Core::Color::White()) override;
	virtual void Update(uint32_t dt) override;

	virtual void Stop() override;
	virtual void SetMovementDirection(PacmanMovement direction) override;

	void SetStateToVulnerable();
	void EatenByPacman();
	void ResetToFirstPosition();

	inline bool IsDead() const { return m_State == GhostState::Dead; }
	inline bool IsVulnerable() const { return m_State == GhostState::Vulnerable || m_State == GhostState::VulnerableEnding; }
	inline bool IsAlive() const { return m_State == GhostState::Alive; }
	inline uint32_t GetPoints() const { return m_Points; }
	inline void LockCanChangeDirection() { m_CanChangeDirection = false; }
	inline bool CanChangeDirection() const { return m_CanChangeDirection; }

public:
	static constexpr uint32_t VULNERABILITY_TIME = 6000;
	static constexpr uint32_t VULNERABILITY_ENDING_TIME = 4000;

private:
	void SetGhostState(GhostState state);

private:
	uint32_t m_VulnerabilityTimer;
	uint32_t m_Points;
	GhostState m_State;
	bool m_CanChangeDirection;
	Core::Vec2D m_InitialPos;
};