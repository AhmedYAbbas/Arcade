#pragma once

#include <memory>

#include "Window.h"

namespace Core
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() const { return *m_Window; }

	private:
		std::unique_ptr<Window> m_Window;

	private:
		static Application* s_Instance;
	}; 
}