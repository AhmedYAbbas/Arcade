#pragma once

#include <Core.h>

#include "Breakout/Paddle.h"
#include "Breakout/Ball.h"
#include "Breakout/LevelBoundary.h"
#include "Breakout/BreakoutGameLevel.h"

enum class BreakoutGameState
{
	Play = 0,
	Serve,
	GameOver
};

class BreakoutScene : public Core::Scene
{
	virtual bool Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;
	virtual const std::string& GetSceneName() const override;

private:
	void ResetGameScene(size_t toLevel = 0);
	BreakoutGameLevel& GetCurrentLevel() { return m_Levels[m_CurrentLevel]; }
	void SetToServeState();
	bool IsBallPassedCutoffY() const;
	void ReduceLifeByOne();
	inline bool IsGameOver() const { return m_Lives < 0; }

private:
	Ball m_Ball;
	Paddle m_Paddle;
	LevelBoundary m_LevelBoundary;
	std::vector<BreakoutGameLevel> m_Levels;
	size_t m_CurrentLevel;
	BreakoutGameState m_GameState;
	int m_Lives = 3;
	float m_YCutoff;
};