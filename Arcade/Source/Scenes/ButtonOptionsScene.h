#pragma once

#include <Core.h>

#include <string>
#include <vector>

class ButtonOptionsScene : public Core::Scene
{
public:
	ButtonOptionsScene(const std::vector<std::string>& optionNames, const Core::Color& textColor, bool animated);

	void SetButtonActions(const std::vector<Core::Button::ButtonAction>& actions);

	virtual bool Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;

private:
	void SetNextButtonHighlighted();
	void SetPreviousButtonHighlighted();
	void HighlightCurrentButton();
	void ExecuteCurrentButtonAction();

private:
	std::vector<std::unique_ptr<Core::Button>> m_Buttons;
	int m_HighlightedOption;
};