#pragma once

#include "Actor.h"

class Pacman : public Actor
{
public:
	virtual void Init(const Core::SpriteSheet& spriteSheet, const std::string& animationsPath, const Core::Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Core::Color& spriteColor = Core::Color::White()) override;
	virtual void Update(uint32_t dt) override;

	virtual void SetMovementDirection(PacmanMovement direction) override;

	void ResetToFirstAnimation();
	void EatenByGhost();
	void ResetScore();
	void AteItem(uint32_t value);
	void AteGhost(uint32_t value);

	inline void ResetGhostEatenMultiplier() { m_GhostMultiplier = 1; }
	inline uint32_t Score() const { return m_Score; }
	inline uint32_t Speed() const { return m_MovementSpeed; }

private:
	void AddToScore(uint32_t value);

private:
	uint32_t m_MovementSpeed = 50;
	uint32_t m_Score;
	uint32_t m_GhostMultiplier;
	bool m_IsDying;
};