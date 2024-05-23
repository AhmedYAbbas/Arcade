#include <iostream>
#include <assert.h>
#include <algorithm>

#include "Shapes/Line.h"
#include "Shapes/Triangle.h"
#include "Shapes/Rectangle.h"
#include "Shapes/Circle.h"
#include "Graphics/BMPImage.h"
#include "Graphics/SpriteSheet.h"
#include "Graphics/BitmapFont.h"
#include "SDLWindow.h"

namespace Core
{
	Window* Window::Create(const WindowProps& props)
	{
		return new SDLWindow(props);
	}

	SDLWindow::SDLWindow(const WindowProps& props)
	{
		Init(props);
	}

	SDLWindow::~SDLWindow()
	{
		Shutdown();
	}

	void SDLWindow::Update()
	{
		SwapBuffers();
	}

	void SDLWindow::SwapBuffers()
	{
		assert(m_Window);
		if (m_Window)
		{
			ClearScreen();

			if (m_FastPath)
			{
				uint8_t* textureData = nullptr;
				int texturePitch = 0;

				if (SDL_LockTexture(m_Texture, nullptr, (void**)&textureData, &texturePitch) >= 0)
				{
					SDL_Surface* surface = m_BackBuffer.GetSurface();
					memcpy(textureData, surface->pixels, surface->w * surface->h * m_PixelFormat->BytesPerPixel);
					SDL_UnlockTexture(m_Texture);
					SDL_RenderCopy(m_Renderer, m_Texture, nullptr, nullptr);
					SDL_RenderPresent(m_Renderer);
				}
			}
			else
			{
				SDL_BlitScaled(m_BackBuffer.GetSurface(), nullptr, m_Surface, nullptr);
				SDL_UpdateWindowSurface(m_Window);
			}
			m_BackBuffer.Clear(m_ClearColor);
		}
	}

	void SDLWindow::Draw(int x, int y, const Color& color)
	{
		assert(m_Window);
		if (m_Window)
		{
			m_BackBuffer.SetPixel(x, y, color);
		}
	}

	void SDLWindow::Draw(const Vec2D& point, const Color& color)
	{
		assert(m_Window);
		if (m_Window)
		{
			m_BackBuffer.SetPixel(point.GetX(), point.GetY(), color);
		}
	}

	void SDLWindow::Draw(const Line& line, const Color& color)
	{
		assert(m_Window);
		if (m_Window)
		{
			int x0 = roundf(line.GetP0().GetX());
			int y0 = roundf(line.GetP0().GetY());
			int x1 = roundf(line.GetP1().GetX());
			int y1 = roundf(line.GetP1().GetY());

			int dx = x1 - x0;
			int dy = y1 - y0;

			signed const char ix((dx > 0) - (dx < 0));
			signed const char iy((dy > 0) - (dy < 0));

			dx = std::abs(dx) * 2;
			dy = std::abs(dy) * 2;

			Draw(x0, y0, color);

			if (dx >= dy)
			{
				int d = dy - dx / 2;
				while (x0 != x1)
				{
					if (d >= 0)
					{
						d -= dx;
						y0 += iy;
					}

					d += dy;
					x0 += ix;
					Draw(x0, y0, color);
				}
			}
			else
			{
				int d = dx - dy / 2;
				while (y0 != y1)
				{
					if (d >= 0)
					{
						d -= dy;
						x0 += ix;
					}

					d += dx;
					y0 += iy;
					Draw(x0, y0, color);
				}
			}
		}
	}

	void SDLWindow::Draw(const Triangle& triangle, const Color& color, bool fill, const Color& fillColor)
	{
		assert(m_Window);
		if (m_Window)
		{
			Line p0p1(triangle.GetP0(), triangle.GetP1());
			Line p1p2(triangle.GetP1(), triangle.GetP2());
			Line p2p0(triangle.GetP2(), triangle.GetP0());

			Draw(p0p1, color);
			Draw(p1p2, color);
			Draw(p2p0, color);

			if (fill)
				FillPoly(triangle.GetPoints(), [&](uint32_t x, uint32_t y) { return fillColor; });
		}
	}

	void SDLWindow::Draw(const Rectangle& rect, const Color& color, bool fill, const Color& fillColor)
	{
		assert(m_Window);
		if (m_Window)
		{
			std::vector<Vec2D> points = rect.GetPoints();
			Line p0p1(points[0], points[1]);
			Line p1p2(points[1], points[2]);
			Line p2p3(points[2], points[3]);
			Line p3p0(points[3], points[0]);

			Draw(p0p1, color);
			Draw(p1p2, color);
			Draw(p2p3, color);
			Draw(p3p0, color);

			if (fill)
				FillPoly(rect.GetPoints(), [&](uint32_t x, uint32_t y) { return fillColor; });
		}
	}

	void SDLWindow::Draw(const Circle& circle, const Color& color, bool fill, const Color& fillColor)
	{
		assert(m_Window);
		if (m_Window)
		{
			static constexpr unsigned int NUM_CIRCLE_SEGMENTS = 30;
			float angle = TWO_PI / static_cast<float>(NUM_CIRCLE_SEGMENTS);

			std::vector<Vec2D> circlePoints;
			std::vector<Line> lines;

			Vec2D p0 = Vec2D(circle.GetCenterPoint().GetX() + circle.GetRadius(), circle.GetCenterPoint().GetY());
			Vec2D p1 = p0;
			Line nextLineToDraw;

			for (unsigned int i = 0; i < NUM_CIRCLE_SEGMENTS; ++i)
			{
				p1.Rotate(angle, circle.GetCenterPoint());
				nextLineToDraw.SetP0(p0);
				nextLineToDraw.SetP1(p1);
				lines.push_back(nextLineToDraw);
				p0 = p1;
				circlePoints.push_back(p0);
			}

			for (const Line& line : lines)
				Draw(line, color);

			if (fill)
				FillPoly(circlePoints, [&](uint32_t x, uint32_t y) { return fillColor; });
		}
	}

	void SDLWindow::Draw(const SpriteSheet& ss, const std::string& spriteName, const Vec2D& pos, const Color overlayColor)
	{
		Draw(ss.GetBMPImage(), ss.GetSprite(spriteName), pos, overlayColor);
	}

	void SDLWindow::Draw(const BitmapFont& font, const std::string& textLine, const Vec2D& pos, const Color overlayColor)
	{
		uint32_t xPos = pos.GetX();
		const SpriteSheet& ss = font.GetSpriteSheet();
		for (const char& c : textLine)
		{
			if (c == ' ')
			{
				xPos += font.GetFontSpacingBetweenWords();
				continue;
			}

			Sprite sprite = ss.GetSprite(std::string(1, c));
			Draw(ss.GetBMPImage(), sprite, Vec2D(xPos, pos.GetY()), overlayColor);

			xPos += sprite.width;
			xPos += font.GetFontSpacingBetweenLetters();
		}
	}

	void SDLWindow::Draw(const BMPImage& image, const Sprite& sprite, const Vec2D& pos, const Color overlayColor)
	{
		float rVal = static_cast<float>(overlayColor.GetRed()) / 255.f;
		float gVal = static_cast<float>(overlayColor.GetGreen()) / 255.f;
		float bVal = static_cast<float>(overlayColor.GetBlue()) / 255.f;
		float aVal = static_cast<float>(overlayColor.GetAlpha()) / 255.f;

		uint32_t width = sprite.width;
		uint32_t height = sprite.height;

		const std::vector<Color>& pixel = image.GetPixels();

		auto topLeft = pos;
		auto topRight = pos + Vec2D(width, 0);
		auto bottomLeft = pos + Vec2D(0, height);
		auto bottomRight = pos + Vec2D(width, height);

		std::vector<Vec2D> points {topLeft, bottomLeft, bottomRight, topRight};

		Vec2D xAxis = topRight - topLeft;
		Vec2D yAxis = bottomLeft - topLeft;

		const float invXAxisLengthSq = 1.f / xAxis.Mag2();
		const float invYAxisLengthSq = 1.f / yAxis.Mag2();

		FillPoly(points, [&](uint32_t px, uint32_t py)
		{
			Vec2D p(px, py);
			Vec2D d = p - topLeft;

			float u = invXAxisLengthSq * d.Dot(xAxis);
			float v = invYAxisLengthSq * d.Dot(yAxis);

			u = Clamp(u, 0.f, 1.f);
			v = Clamp(v, 0.f, 1.f);

			float tx = std::roundf(u * static_cast<float>(sprite.width));
			float ty = std::roundf(v * static_cast<float>(sprite.height));

			Color imageColor = pixel[GetIndex(image.GetWidth(), ty + sprite.yPos, tx + sprite.xPos)];
			Color newColor(imageColor.GetRed() * rVal, imageColor.GetGreen() * gVal, imageColor.GetBlue() * bVal, imageColor.GetAlpha() * aVal);
			return newColor;

		});
	}

	void SDLWindow::Init(const WindowProps& props, bool fast)
	{
		if (SDL_Init(SDL_INIT_VIDEO))
		{
			std::cout << "Error SDL_Init Failed" << std::endl;
			return;
		}

		m_Title = props.Title;
		m_Width = props.Width;
		m_Height = props.Height;
		m_Magnification = props.Magnification;
		m_FastPath = fast;

		m_Window = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width * m_Magnification, m_Height * m_Magnification, 0);
		if (m_Window)
		{
			uint8_t rClear = 0;
			uint8_t gClear = 0;
			uint8_t bClear = 0;
			uint8_t aClear = 255;

			if (m_FastPath)
			{
				m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (!m_Renderer)
				{
					std::cout << "SDL_CreateRenderer failed!" << std::endl;
					return;
				}
				SDL_SetRenderDrawColor(m_Renderer, rClear, gClear, bClear, aClear);
			}
			else
				m_Surface = SDL_GetWindowSurface(m_Window);

			m_PixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

			if (m_FastPath)
				m_Texture = SDL_CreateTexture(m_Renderer, m_PixelFormat->format, SDL_TEXTUREACCESS_STREAMING, m_Width, m_Height);

			Color::InitColorFormat(m_PixelFormat);
			m_ClearColor = Color(rClear, gClear, bClear, aClear);
			m_BackBuffer.Init(m_Width, m_Height, m_PixelFormat->format);
			m_BackBuffer.Clear(m_ClearColor);
		}
	}

	void SDLWindow::Shutdown()
	{
		if (m_PixelFormat)
		{
			SDL_FreeFormat(m_PixelFormat);
			m_PixelFormat = nullptr;
		}

		if (m_Texture)
		{
			SDL_DestroyTexture(m_Texture);
			m_Texture = nullptr;
		}

		if (m_Renderer)
		{
			SDL_DestroyRenderer(m_Renderer);
			m_Renderer = nullptr;
		}

		if (m_Window)
		{
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
		}

		SDL_Quit();
	}

	void SDLWindow::ClearScreen()
	{
		assert(m_Window);
		if (m_Window)
		{
			if (m_FastPath)
				SDL_RenderClear(m_Renderer);
			else
				SDL_FillRect(m_Surface, nullptr, m_ClearColor.GetPixelColor());
		}
	}

	void SDLWindow::FillPoly(const std::vector<Vec2D>& points, const FillPolyFunc& func)
	{
		if (!points.empty())
		{
			float top = points[0].GetY();
			float bottom = points[0].GetY();
			float right = points[0].GetX();
			float left = points[0].GetX();

			for (size_t i = 1; i < points.size(); ++i)
			{
				if (points[i].GetY() < top)
					top = points[i].GetY();

				if (points[i].GetY() > bottom)
					bottom = points[i].GetY();

				if (points[i].GetX() < left)
					left = points[i].GetX();

				if (points[i].GetX() > right)
					right = points[i].GetX();
			}

			for (int pixelY = top; pixelY < bottom; ++pixelY)
			{
				std::vector<float> nodeXVec;
				size_t j = points.size() - 1;
				for (size_t i = 0; i < points.size(); ++i)
				{
					float pointiY = points[i].GetY();
					float pointjY = points[j].GetY();
					if ((pointiY <= static_cast<float>(pixelY) && pointjY > static_cast<float>(pixelY)) ||
						(pointjY <= static_cast<float>(pixelY) && pointiY > static_cast<float>(pixelY)))
					{
						float denom = pointjY - pointiY;
						if (IsEqual(denom, 0))
							continue;

						float x = points[i].GetX() + (pixelY - pointiY) / denom * (points[j].GetX() - points[i].GetX());
						nodeXVec.push_back(x);
					}
					j = i;
				}
				std::sort(nodeXVec.begin(), nodeXVec.end());

				for (size_t k = 0; k < nodeXVec.size(); k += 2)
				{
					if (nodeXVec[k] > right)
						break;

					if (nodeXVec[k + 1] > left)
					{
						if (nodeXVec[k] < left)
							nodeXVec[k] = left;
						if (nodeXVec[k + 1] > right)
							nodeXVec[k + 1] = right;

						for (int pixelX = nodeXVec[k]; pixelX < nodeXVec[k + 1]; ++pixelX)
							Draw(pixelX, pixelY, func(pixelX, pixelY));
					}
				}
			}
		}
	}
}