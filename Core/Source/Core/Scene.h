#pragma once

#include <string>

#include "GameController.h"

namespace Core
{
	class Window;

	class Scene
	{
	public:
		virtual ~Scene() = default;

		virtual bool Init() = 0;
		virtual void Update(uint32_t dt) = 0;
		virtual void Draw(Window& window) = 0;
		virtual const std::string& GetSceneName() const = 0;

		const GameController& GetGameController() const { return m_GameController; }
		GameController& GetGameController() { return m_GameController; }

	protected:
		GameController m_GameController;
	};
}