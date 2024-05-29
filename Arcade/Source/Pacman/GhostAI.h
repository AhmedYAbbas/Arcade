#pragma once

#include <random>

#include "Ghost.h"
#include "GameUtils.h"

class Level;
class Pacman;

enum class GhostAIState
{
	Start = 0,
	InPen,
	ExitPen,
	Chase,
	Scatter,
	GoToPen
};

class GhostAI
{
public:
	GhostAI();

	void Init(Ghost& ghost, uint32_t lookAheadDistance, const Core::Vec2D scatterTarget, GhostName name);
	PacmanMovement Update(uint32_t dt, const Pacman& pacman, const Level& level, const std::vector<Ghost>& ghosts);
	void Draw(Core::Window& window);

private:
	void SetState(GhostAIState state);
	void ChangeTarget(Core::Vec2D target);
	const Core::Vec2D& GetChaseTarget(uint32_t dt, const Pacman& pacman, const Level& level, const std::vector<Ghost>& ghosts);

private:
	Core::Vec2D m_ScatterTarget;
	Core::Vec2D m_Target;
	uint32_t m_LookAheadDistance;
	Ghost* m_Ghost;
	GhostName m_Name;
	std::default_random_engine m_AIRandomGenerator;
	GhostAIState m_State;
	GhostAIState m_LastState;
	uint64_t m_Timer;
};