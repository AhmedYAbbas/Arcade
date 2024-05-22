#pragma once

#include <Core.h>

#include "Breakout/Paddle.h"

class BreakoutScene : public Core::Scene
{
	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;
	virtual const std::string& GetSceneName() const override;

private:
	void ResetGameScene();

private:
	Paddle m_Paddle;
};