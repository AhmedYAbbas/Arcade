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

		virtual void Update() override;

		void SwapBuffers();

		inline void SetClearColor(const Color& color) { m_ClearColor = color; }
		inline uint32_t GetWidth() const override { return m_Width; }
		inline uint32_t GetHeight() const override { return m_Height; }

		void Draw(int x, int y, const Color& color) override;
		void Draw(const Vec2D& point, const Color& color) override;
		void Draw(const Line& line, const Color& color) override;
		void Draw(const Triangle& triangle, const Color& color, bool fill = false, const Color& fillColor = Color::White()) override;
		void Draw(const Rectangle& rect, const Color& color, bool fill = false, const Color& fillColor = Color::White()) override;
		void Draw(const Circle& circle, const Color& color, bool fill = false, const Color& fillColor = Color::White()) override;
		virtual void Draw(const BMPImage& image, const Sprite& sprite, const Vec2D& pos) override;
		virtual void Draw(const SpriteSheet& ss, const std::string& spriteName, const Vec2D& pos) override;

		inline virtual void* GetNativeWindow() const override { return m_Window; }

	private:
		void Init(const WindowProps& props);
		void Shutdown();

		void ClearScreen();
		void FillPoly(const std::vector<Vec2D>& points, const Color& color);

	private:
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