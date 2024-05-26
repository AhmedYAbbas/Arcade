#include "CorePCH.h"

#include <SDL.h>

#include "InputController.h"
#include "GameController.h"

namespace Core
{
	InputController::InputController(const KeyboardInputAction& quitAction)
		: m_QuitAction(quitAction), m_GameController(nullptr)
	{
	}

	void InputController::Update(uint32_t dt)
	{
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent))
		{
			switch (sdlEvent.type)
			{
				case SDL_QUIT:
					m_QuitAction(dt, SDL_PRESSED);
					break;
				case SDL_MOUSEMOTION:
					if (m_GameController)
					{
						if (const MouseMovedAction& mouseMoved = m_GameController->GetMouseMovedAction())
						{
							MousePosition position;
							position.X = sdlEvent.motion.x;
							position.Y = sdlEvent.motion.y;
							mouseMoved(position);
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					if (m_GameController)
					{
						const MouseInputAction& action = m_GameController->GetMouseInputAction(static_cast<MouseButton>(sdlEvent.button.button));

						MousePosition position;
						position.X = sdlEvent.button.x;
						position.Y = sdlEvent.button.y;
						action(static_cast<InputState>(sdlEvent.button.state), position);
					}
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					if (m_GameController)
					{
						const KeyboardInputAction& action = m_GameController->GetKeyboardInputAction(sdlEvent.key.keysym.sym);
						action(dt, static_cast<InputState>(sdlEvent.key.state));
					}
					break;
			}
		}
	}

	void InputController::SetGameController(const GameController& controller)
	{
		m_GameController = &controller;
	}
}