#pragma once

#include "Scenes/ButtonOptionsScene.h"

class PacmanStartScene : public ButtonOptionsScene
{
public:
	PacmanStartScene();

	virtual bool Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;
	virtual const std::string& GetSceneName() const override;
};