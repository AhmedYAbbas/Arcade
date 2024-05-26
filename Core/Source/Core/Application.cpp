#include "CorePCH.h"

#include <SDL.h>

#include "Application.h"
#include "Scene.h"

namespace Core
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_InputController([&](uint32_t dt, InputState state) { m_Running = false; })
	{
		assert(!s_Instance);
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());

		if (!m_Font.Load("ArcadeFont"))
			std::cout << "Could not load arcade font!" << std::endl;
	}

	void Application::Run()
	{
		uint32_t lastTick = SDL_GetTicks();
		uint32_t dt = 10;
		uint32_t accumulator = 0;

		while (m_Running)
		{
			uint32_t currentTick = SDL_GetTicks();
			uint32_t frameTime = currentTick - lastTick;

			if (frameTime > 300)
				frameTime = 300;

			lastTick = currentTick;
			accumulator += frameTime;

			m_InputController.Update(dt);

			if (CurrentScene())
			{
				while (accumulator >= dt)
				{
					CurrentScene()->Update(dt);
					accumulator -= dt;
				}
				CurrentScene()->Draw(*m_Window);
			}

			m_Window->Update();
		}
	}

	void Application::PushScene(std::unique_ptr<Scene> scene)
	{
		assert(scene && "Can't push scene that is nullptr");
		if (scene)
		{
			scene->Init();
			m_InputController.SetGameController(scene->GetGameController());
			m_SceneStack.push_back(std::move(scene));
			SDL_SetWindowTitle(static_cast<SDL_Window*>(m_Window->GetNativeWindow()), CurrentScene()->GetSceneName().c_str());
		}
	}

	void Application::PopScene()
	{
		if (!m_SceneStack.empty())
			m_SceneStack.pop_back();

		if (CurrentScene())
		{
			m_InputController.SetGameController(CurrentScene()->GetGameController());
			SDL_SetWindowTitle(static_cast<SDL_Window*>(m_Window->GetNativeWindow()), CurrentScene()->GetSceneName().c_str());
		}
	}

	Scene* Application::CurrentScene()
	{
		if (m_SceneStack.empty())
			return nullptr;
		return m_SceneStack.back().get();
	}

	const std::string& Application::GetBasePath()
	{
		static std::string basePath = SDL_GetBasePath();
		return basePath;
	}
}
