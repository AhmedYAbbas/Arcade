#include <iostream>
#include <assert.h>
#include <algorithm>

#include "Shapes/Line.h"
#include "Shapes/Triangle.h"
#include "Shapes/Rectangle.h"
#include "Shapes/Circle.h"
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

	void SDLWindow::OnUpdate()
	{
		while (m_Running)
		{
			SDL_Event sdlEvent;
			while (SDL_PollEvent(&sdlEvent))
			{
				switch (sdlEvent.type)
				{
					case SDL_QUIT:
						m_Running = false;
						break;
				}
			}

			Line line(Vec2D(0, 0), Vec2D(m_Width, m_Height));
			Triangle triangle(Vec2D(60, 10), Vec2D(10, 110), Vec2D(110, 110));
			Rectangle rect(Vec2D(m_Width / 2 - 25, m_Height / 2 - 25), 50, 50);
			Circle circle(Vec2D(m_Width / 2 + 50, m_Height / 2 + 50), 50);

			Draw(line, Color::Yellow());
			Draw(triangle, Color::Red(), true, Color::Red());
			Draw(rect, Color::Blue(), true, Color::Blue());
			Draw(circle, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));

			SwapBuffers();
		}
	}

	void SDLWindow::SwapBuffers()
	{
		assert(m_Window);
		if (m_Window)
		{
			ClearScreen();
			SDL_BlitScaled(m_BackBuffer.GetSurface(), nullptr, m_Surface, nullptr);
			SDL_UpdateWindowSurface(m_Window);
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
				FillPoly(triangle.GetPoints(), fillColor);
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
				FillPoly(rect.GetPoints(), fillColor);
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
				FillPoly(circlePoints, fillColor);
		}
	}

	void SDLWindow::Init(const WindowProps& props)
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

		m_Window = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width * m_Magnification, m_Height * m_Magnification, 0);
		if (m_Window)
		{
			m_Surface = SDL_GetWindowSurface(m_Window);
			SDL_PixelFormat* pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
			Color::InitColorFormat(pixelFormat);
			m_ClearColor = Color::Black();
			m_BackBuffer.Init(m_Width, m_Height, pixelFormat->format);
			m_BackBuffer.Clear(m_ClearColor);
		}
	}

	void SDLWindow::Shutdown()
	{
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
			SDL_FillRect(m_Surface, nullptr, m_ClearColor.GetPixelColor());
		}
	}

	void SDLWindow::FillPoly(const std::vector<Vec2D>& points, const Color& color)
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
							Draw(pixelX, pixelY, color);
					}
				}
			}
		}
	}
}