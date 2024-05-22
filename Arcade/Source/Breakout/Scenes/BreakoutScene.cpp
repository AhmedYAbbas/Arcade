#include "BreakoutScene.h"

void BreakoutScene::Init()
{
	std::cout << "Breakout Game Init();" << std::endl;
}

void BreakoutScene::Update(uint32_t dt)
{
	std::cout << "Breakout Game Update();" << std::endl;
}

void BreakoutScene::Draw(Core::Window& window)
{
	std::cout << "Breakout Game Draw();" << std::endl;
}

const std::string& BreakoutScene::GetSceneName() const
{
	static std::string sceneName = "Breakout";
	return sceneName;
}
