#pragma once

#include <Core.h>

class NotImplementedScene : public Core::Scene
{
public:
	virtual bool Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Window& window) override;
	virtual const std::string& GetSceneName() const override;
};