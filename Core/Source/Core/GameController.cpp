#include <SDL.h>

#include "GameController.h"

namespace Core
{
	GameController::GameController()
		:	m_MouseMovedAction(nullptr)
	{
	}

	const KeyboardInputAction GameController::GetKeyboardInputAction(InputKey key) const
	{
		for (const auto& keyboardButtonAction : m_KeyboardButtonActions)
		{
			if (key == keyboardButtonAction.Key)
				return keyboardButtonAction.Action;
		}
		return [](uint32_t, InputState){};
	}

	void GameController::AddKeyboardButtonAction(const KeyboardButtonAction& buttonAction)
	{
		m_KeyboardButtonActions.push_back(buttonAction);
	}

	void GameController::Clear()
	{
		m_KeyboardButtonActions.clear();
		m_MouseButtonActions.clear();
		m_MouseMovedAction = nullptr;
	}

	bool GameController::IsPressed(InputState state)
	{
		return state == SDL_PRESSED;
	}

	bool GameController::IsReleased(InputState state)
	{
		return state == SDL_RELEASED;
	}

	InputKey GameController::ActionKey()
	{
		return static_cast<InputKey>(SDLK_a);
	}

	InputKey GameController::CancelKey()
	{
		return static_cast<InputKey>(SDLK_s);
	}

	InputKey GameController::LeftKey()
	{
		return static_cast<InputKey>(SDLK_LEFT);
	}

	InputKey GameController::RightKey()
	{
		return static_cast<InputKey>(SDLK_RIGHT);
	}

	InputKey GameController::UpKey()
	{
		return static_cast<InputKey>(SDLK_UP);
	}

	InputKey GameController::DownKey()
	{
		return static_cast<InputKey>(SDLK_DOWN);
	}

	const MouseInputAction GameController::GetMouseInputAction(MouseButton button) const
	{
		for (const auto& mouseButtonAction : m_MouseButtonActions)
		{
			if (button == mouseButtonAction.MouseButton)
				return mouseButtonAction.MouseInputAction;
		}
		return [](InputState, const MousePosition&) {};
	}

	void GameController::AddMouseButtonAction(const MouseButtonAction& mouseButtonAction)
	{
		m_MouseButtonActions.push_back(mouseButtonAction);
	}

	MouseButton GameController::LeftMouseButton()
	{
		return static_cast<MouseButton>(SDL_BUTTON_LEFT);
	}

	MouseButton GameController::RightMouseButton()
	{
		return static_cast<MouseButton>(SDL_BUTTON_RIGHT);
	}
}