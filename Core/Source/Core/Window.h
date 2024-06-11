#pragma once

#include <string>

#include "Graphics/Color.h"
#include "Utility/Vec2D.h"

namespace Core
{
	enum class UVOrientation
	{
		Inside = 0,
		Outside
	};

	struct UVParams
	{
		UVOrientation Orientation = UVOrientation::Inside;
		Vec2D SplitPoint1 = {1.f, 0.f};
		Vec2D SplitPoint2 = {1.f, 1.f};
	};

	enum class GradientXParam
	{
		NoXGradient = 0,
		LeftToRight,
		RightToLeft
	};
	
	enum class GradientYParam
	{
		NoYGradient = 0,
		BottomToTop,
		TopToBottom
	};

	struct GradientParams
	{
		GradientXParam XParam = GradientXParam::NoXGradient;
		GradientYParam YParam = GradientYParam::NoYGradient;

		Color Color1 = Color::White();
		Color Color2 = Color::White();
	};

	struct DrawTransform
	{
		Vec2D Pos = Vec2D::Zero;
		float Scale = 1.f;
		float RotationAngle = 0.f;
	};

	struct ColorParams
	{
		GradientParams Gradient = {};
		Color Overlay = Color::White();
		float Alpha = 1.f;
		bool BilinearFiltering = false;
	};

	enum class DrawSurface
	{
		Foreground = 0,
		Background
	};

	struct WindowProps
	{
		std::string Title;
		uint32_t Width, Height, Magnification;

		WindowProps(const std::string& title = "Arcade", uint32_t width = 224, uint32_t height = 288, uint32_t magnification = 3)
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
	class BitmapFont;
	class ScreenBuffer;

	struct Sprite;

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void Update(uint32_t dt) = 0;
		virtual void Draw(int x, int y, const Color& color) = 0;
		virtual void Draw(const Vec2D& point, const Color& color) = 0;
		virtual void Draw(const Line& line, const Color& color) = 0;
		virtual void Draw(const Triangle& triangle, const Color& color, bool fill = false, const Color& fillColor = Color::White()) = 0;
		virtual void Draw(const Rectangle& rect, const Color& color, bool fill = false, const Color& fillColor = Color::White()) = 0;
		virtual void Draw(const Circle& circle, const Color& color, bool fill = false, const Color& fillColor = Color::White()) = 0;

		virtual void Draw(const BMPImage& image, const Sprite& sprite, const Vec2D& pos, float angle = 0.0f, float scale = 1.0f, const Color& overlayColor = Color::White(), float alpha = 1.0f, bool bilinearFilter = false) = 0;
		virtual void Draw(const SpriteSheet& ss, const std::string& spriteName, const Vec2D& pos, float angle = 0.0f, float scale = 1.0f, const Color& overlayColor = Color::White(), float alpha = 1.0f, bool bilinearFilter = false) = 0;
		virtual void Draw(const BitmapFont& font, const std::string& textLine, const Vec2D& pos, const Color overlayColor = Color::White(), float scale = 1.0f, float alpha = 1.0f, bool bilinearFilter = false) = 0;
		virtual void DrawBackground(const BMPImage& image, const Sprite& sprite, const Vec2D& pos, const Color& overlayColor = Color::White()) = 0;

		virtual void Draw(const BMPImage& image, const Sprite& sprite, const DrawTransform& transform, const ColorParams& colorParams, const UVParams& uvParams, DrawSurface drawSurface = DrawSurface::Foreground) = 0;
		virtual void Draw(const SpriteSheet& ss, const std::string& spriteName, const DrawTransform& transform, const ColorParams& colorParams, const UVParams& uvParams) = 0;
		virtual void Draw(const BitmapFont& font, const std::string& textLine, const DrawTransform& transform, const ColorParams& colorParams, const UVParams& uvParams) = 0;

		virtual void ClearBackground() = 0;

		virtual void Shake(float power, float durationInSeconds) = 0;

		virtual void FillPolySprite(
			ScreenBuffer& screenBuffer,
			const std::vector<Vec2D>& points,
			const std::vector<Color>& imagePixels,
			uint32_t imageWidth,
			const float overlayColor[4],
			const Vec2D& spritePos,
			const Vec2D& spriteSize,
			const Vec2D& xAxis,
			const Vec2D& yAxis,
			const float invXAxisLengthSq,
			const float invYAxisLengthSq,
			float alpha,
			bool bilinearFilter,
			const GradientParams& gradient,
			const UVParams& uvParams) = 0;

		virtual void DrawBilinearFilteredPixel(
			ScreenBuffer& screenBuffer,
			const std::vector<Color>& imagePixels,
			int pixelX,
			int pixelY,
			const Vec2D& uv,
			uint32_t imageWidth,
			const Vec2D& spriteSize,
			const Vec2D& spritePos,
			const float overlayColor[4],
			float alpha,
			const GradientParams& gradient,
			const UVParams& uvParams) = 0;

		virtual void DrawUnfilteredPixel(
			ScreenBuffer& screenBuffer,
			const std::vector<Color>& imagePixels,
			int pixelX,
			int pixelY,
			const Vec2D& uv,
			uint32_t imageWidth,
			const Vec2D& spriteSize,
			const Vec2D& spritePos,
			const float overlayColor[4],
			float alpha,
			const GradientParams& gradient,
			const UVParams& uvParams) = 0;


		virtual void Gradient(const GradientParams& gradient, float u, float v, float overlayColor[4]) = 0;
		virtual void ClipUV(const Vec2D& uv, const UVParams& uvParams, Color& imageColor) = 0;

		virtual inline uint32_t GetWidth() const = 0;
		virtual inline uint32_t GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
