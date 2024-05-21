#pragma once

#include <memory>
#include <vector>

#include "Window.h"
#include "InputController.h"

namespace Core
{
	class Scene;

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() const { return *m_Window; }

		void PushScene(std::unique_ptr<Scene> scene);
		void PopScene();
		Scene* CurrentScene();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		std::vector<std::unique_ptr<Scene>> m_SceneStack;
		InputController m_InputController;

	private:
		static Application* s_Instance;
	}; 

	Application* CreateApplication();
}