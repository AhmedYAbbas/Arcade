#pragma once

#include "Pacman/Level.h"
#include "Pacman/Pacman.h"
#include "Pacman/Ghost.h"
#include "Pacman/GhostAI.h"

class PacmanScene : public Core::Scene
{
public:
	virtual bool Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;
	virtual const std::string& GetSceneName() const override;

private:
	void ResetGame();
	void ResetLevel();
	void UpdatePacmanMovement();
	void HandleGameControllerState(uint32_t dt, Core::InputState state, PacmanMovement direction);
	void DrawScore(Core::Window& window);
	void DrawLives(Core::Window& window);
	void SetupGhosts();

private:
	PacmanMovement m_PressedDirection;
	Core::SpriteSheet m_PacmanSpriteSheet;
	Pacman m_Pacman;
	Level m_Level;
	size_t m_NumLives;
	std::vector<Ghost> m_Ghosts;
	std::vector<GhostAI> m_GhostAIs;
};