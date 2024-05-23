#pragma once

#include <memory>
#include <vector>

#include "Window.h"
#include "InputController.h"
#include "Graphics/BitmapFont.h"

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

		static const std::string& GetBasePath();
		inline const BitmapFont& GetFont() const { return m_Font; }

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		std::vector<std::unique_ptr<Scene>> m_SceneStack;
		InputController m_InputController;

		BitmapFont m_Font;

	private:
		static Application* s_Instance;
	}; 

	Application* CreateApplication();
}