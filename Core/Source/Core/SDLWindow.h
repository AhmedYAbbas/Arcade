#pragma once

#include <vector>
#include <SDL.h>

#include "Window.h"
#include "Graphics/Color.h"
#include "Graphics/ScreenBuffer.h"

namespace Core
{
	class Vec2D;
	class Line;
	class Triangle;
	class Rectangle;
	class Circle;

	class SDLWindow : public Window
	{
	public:
		SDLWindow(const WindowProps& props);
		virtual ~SDLWindow();

		virtual void OnUpdate() override;

		void SwapBuffers();

		inline void SetClearColor(const Color& color) { m_ClearColor = color; }
		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		void Draw(int x, int y, const Color& color);
		void Draw(const Vec2D& point, const Color& color);
		void Draw(const Line& line, const Color& color);
		void Draw(const Triangle& triangle, const Color& color, bool fill = false, const Color& fillColor = Color::White());
		void Draw(const Rectangle& rect, const Color& color, bool fill = false, const Color& fillColor = Color::White());
		void Draw(const Circle& circle, const Color& color, bool fill = false, const Color& fillColor = Color::White());

	private:
		void Init(const WindowProps& props);
		void Shutdown();

		void ClearScreen();
		void FillPoly(const std::vector<Vec2D>& points, const Color& color);

	private:
		bool m_Running = true;

		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Magnification;

		Color m_ClearColor;
		ScreenBuffer m_BackBuffer;

		SDL_Window* m_Window;
		SDL_Surface* m_Surface;
	};
}