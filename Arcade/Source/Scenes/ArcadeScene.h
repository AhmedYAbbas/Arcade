#pragma once

#include <Core.h>

class ArcadeScene : public Core::Scene
{
public:
	ArcadeScene();

	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;
	virtual const std::string& GetSceneName() const override;

private:
	Core::SpriteSheet m_TempSS;
};