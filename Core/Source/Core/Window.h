#pragma once

#include <string>

#include "Graphics/Color.h"

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

	class Vec2D;
	class Line;
	class Triangle;
	class Rectangle;
	class Circle;
	class BMPImage;
	class SpriteSheet;
	struct Sprite;

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void Update() = 0;
		virtual void Draw(int x, int y, const Color& color) = 0;
		virtual void Draw(const Vec2D& point, const Color& color) = 0;
		virtual void Draw(const Line& line, const Color& color) = 0;
		virtual void Draw(const Triangle& triangle, const Color& color, bool fill = false, const Color& fillColor = Color::White()) = 0;
		virtual void Draw(const Rectangle& rect, const Color& color, bool fill = false, const Color& fillColor = Color::White()) = 0;
		virtual void Draw(const Circle& circle, const Color& color, bool fill = false, const Color& fillColor = Color::White()) = 0;
		virtual void Draw(const BMPImage& image, const Sprite& sprite, const Vec2D& pos) = 0;
		virtual void Draw(const SpriteSheet& ss, const std::string& spriteName, const Vec2D& pos) = 0;

		virtual inline uint32_t GetWidth() const = 0;
		virtual inline uint32_t GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
