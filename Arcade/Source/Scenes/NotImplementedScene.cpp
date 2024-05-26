#include "NotImplementedScene.h"

bool NotImplementedScene::Init()
{
	return false;
}

void NotImplementedScene::Update(uint32_t dt)
{
}

void NotImplementedScene::Draw(Window& window)
{
}

const std::string& NotImplementedScene::GetSceneName() const
{
	static std::string name = "Not Implemented!";
	return name;
}
