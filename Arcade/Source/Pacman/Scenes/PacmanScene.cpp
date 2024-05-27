#include "ArcadePCH.h"
#include "PacmanScene.h"

bool PacmanScene::Init()
{
	return m_Level.Init(Core::Application::Get().GetBasePath() + "Pacman_level.txt");
}

void PacmanScene::Update(uint32_t dt)
{
	m_Level.Update(dt);
}

void PacmanScene::Draw(Core::Window& window)
{
	m_Level.Draw(window);
}

const std::string& PacmanScene::GetSceneName() const
{
	static std::string sceneName = "Pacman!";
	return sceneName;
}
