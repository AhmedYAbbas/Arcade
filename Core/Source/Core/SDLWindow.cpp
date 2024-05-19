#include <iostream>
#include <assert.h>

#include "Shapes/Line.h"
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
			Draw(line, Color::Yellow());
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
			m_BackBuffer.Clear();
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
			SDL_PixelFormat* pixelFormat = m_Surface->format;
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
	}
}