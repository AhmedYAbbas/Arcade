#pragma once

#include <string>

namespace Core
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width, Height, Magnification;

		WindowProps(const std::string& title = "Arcade", uint32_t width = 224, uint32_t height = 288, uint32_t magnification = 2)
			: Title(title), Width(width), Height(height), Magnification(magnification)
		{
		}
	};

	class Window
	{
	public:
		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		static Window* Create(const WindowProps& props = WindowProps());
	};
}
