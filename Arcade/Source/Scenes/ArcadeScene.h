#pragma once

#include <Core.h>

#include "ButtonOptionsScene.h"

enum class Game
{
	Tetris,
	Breakout,
	Asteroids,
	Pacman
};

class ArcadeScene : public ButtonOptionsScene
{
public:
	ArcadeScene();

	virtual bool Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;
	virtual const std::string& GetSceneName() const override;

private:
	std::unique_ptr<Core::Scene> GetScene(Game game);

	Core::AnimatedSprite m_AnimatedSprite;
	Core::SpriteSheet m_SpriteSheet;
};