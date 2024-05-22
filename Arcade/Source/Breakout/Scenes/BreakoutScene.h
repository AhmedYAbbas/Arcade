#pragma once

#include <Core.h>

#include "Breakout/Paddle.h"
#include "Breakout/Ball.h"
#include "Breakout/LevelBoundary.h"

class BreakoutScene : public Core::Scene
{
	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;
	virtual const std::string& GetSceneName() const override;

private:
	void ResetGameScene();

private:
	Ball m_Ball;
	Paddle m_Paddle;
	LevelBoundary m_LevelBoundary;
};