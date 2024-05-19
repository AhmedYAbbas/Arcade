#include <SDL.h>
#include <assert.h>

#include "Application.h"

namespace Core
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		assert(!s_Instance);
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	void Application::Run()
	{
		m_Window->OnUpdate();
	}
}
