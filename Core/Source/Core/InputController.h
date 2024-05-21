#pragma once

#include "InputAction.h"
#include "GameController.h"

namespace Core
{
	class GameController;

	class InputController
	{
	public:
		InputController(const KeyboardInputAction& quitAction);

		void Update(uint32_t dt);
		void SetGameController(const GameController& controller);

	private:
		KeyboardInputAction m_QuitAction;
		const GameController* m_GameController;
	};
}