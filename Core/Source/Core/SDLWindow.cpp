#include "CorePCH.h"
#include "Shapes/Line.h"
#include "Shapes/Triangle.h"
#include "Shapes/Rectangle.h"
#include "Shapes/Circle.h"
#include "Graphics/BMPImage.h"
#include "Graphics/SpriteSheet.h"
#include "Graphics/BitmapFont.h"
#include "SDLWindow.h"
#include "Utility/Utils.h"

static std::vector<std::string> s_PreferredPixelFormats = {
		"SDL_PIXELFORMAT_ARGB8888",
		"SDL_PIXELFORMAT_RGBA8888",
		"SDL_PIXELFORMAT_BGRA8888"
};

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

	void SDLWindow::Update(uint32_t dt)
	{
		if (m_ScreenShakeTimer > 0)
		{
			m_ScreenShakeTimer -= static_cast<int>(dt);

			if (m_ScreenShakeTimer <= 0)
				m_ScreenShakeTimer = 0;

			//Section 2 - Exercise 1
			//TODO: implement
		}
		else
			m_ScreenShakeOffset = Vec2D::Zero;

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
				SDL_Rect destRect;
				destRect.x = static_cast<int>(std::round(m_ScreenShakeOffset.GetX()));
				destRect.y = static_cast<int>(std::round(m_ScreenShakeOffset.GetY()));

				destRect.w = m_Width * m_Magnification;
				destRect.h = m_Height * m_Magnification;

				if (SDL_LockTexture(m_BackgroundTexture, nullptr, (void**)&textureData, &texturePitch) >= 0)
				{
					SDL_Surface* surface = m_BackgroundBuffer.GetSurface();
					memcpy(textureData, surface->pixels, static_cast<size_t>(surface->w) * static_cast<size_t>(surface->h) * static_cast<size_t>(m_PixelFormat->BytesPerPixel));
					SDL_UnlockTexture(m_BackgroundTexture);
					SDL_RenderCopy(m_Renderer, m_BackgroundTexture, nullptr, nullptr);
				}

				if (SDL_LockTexture(m_Texture, nullptr, (void**)&textureData, &texturePitch) >= 0)
				{
					SDL_Surface* surface = m_BackBuffer.GetSurface();
					memcpy(textureData, surface->pixels, static_cast<size_t>(surface->w) * static_cast<size_t>(surface->h) * static_cast<size_t>(m_PixelFormat->BytesPerPixel));
					SDL_UnlockTexture(m_Texture);
					SDL_RenderCopy(m_Renderer, m_Texture, nullptr, nullptr);
				}

				SDL_RenderPresent(m_Renderer);
			}
			else
			{
				SDL_BlitScaled(m_BackBuffer.GetSurface(), nullptr, m_Surface, nullptr);
				SDL_UpdateWindowSurface(m_Window);
			}
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
			m_BackBuffer.SetPixel(static_cast<int>(point.GetX()), static_cast<int>(point.GetY()), color);
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

	void SDLWindow::Draw(const SpriteSheet& ss, const std::string& spriteName, const Vec2D& pos, float angle, float scale, const Color& overlayColor, float alpha, bool bilinearFilter)
	{
		Draw(ss.GetBMPImage(), ss.GetSprite(spriteName), pos, angle, scale, overlayColor, alpha, bilinearFilter);
	}

	void SDLWindow::Draw(const BitmapFont& font, const std::string& textLine, const Vec2D& pos, const Color overlayColor, float scale, float alpha, bool bilinearFilter)
	{
		DrawTransform transform;

		transform.Pos = pos;
		transform.RotationAngle = 0.0f;
		transform.Scale = scale;

		ColorParams colorParams;
		colorParams.Alpha = alpha;
		colorParams.BilinearFiltering = bilinearFilter;
		colorParams.Overlay = overlayColor;
		colorParams.Gradient.XParam = GradientXParam::NoXGradient;
		colorParams.Gradient.YParam = GradientYParam::NoYGradient;

		UVParams uvParams;

		Draw(font, textLine, transform, colorParams, uvParams);
	}

	void SDLWindow::DrawBackground(const BMPImage& image, const Sprite& sprite, const Vec2D& pos, const Color& overlayColor)
	{
		DrawTransform transform;
		transform.Pos = pos;
		transform.Scale = 1.f;
		transform.RotationAngle = 0.f;

		ColorParams colorParams;
		colorParams.Alpha = 1.f;
		colorParams.Gradient.XParam = GradientXParam::NoXGradient;
		colorParams.Gradient.YParam = GradientYParam::NoYGradient;
		//	colorParams.gradient.color1 = Color::White();
		//	colorParams.gradient.color2 = Color::Blue();

		colorParams.BilinearFiltering = false;
		colorParams.Overlay = overlayColor;

		UVParams uvParams;
		Draw(image, sprite, transform, colorParams, uvParams, DrawSurface::Background);
	}

	void SDLWindow::Draw(const BMPImage& image, const Sprite& sprite, const Vec2D& pos, float angle, float scale, const Color& overlayColor, float alpha, bool bilinearFilter)
	{
		DrawTransform transform;
		transform.Pos = pos;
		transform.Scale = scale;
		transform.RotationAngle = angle;

		ColorParams colorParams;
		colorParams.Alpha = alpha;
		colorParams.Gradient.XParam = GradientXParam::NoXGradient;
		colorParams.Gradient.YParam = GradientYParam::NoYGradient;
		colorParams.BilinearFiltering = bilinearFilter;
		colorParams.Overlay = overlayColor;

		UVParams uvParams;

		Draw(image, sprite, transform, colorParams, uvParams);
	}

	void SDLWindow::Draw(const BMPImage& image, const Sprite& sprite, const DrawTransform& transform, const ColorParams& colorParams, const UVParams& uvParams, DrawSurface drawSurface)
	{
		float normalizedOverlayColor[4];
		normalizedOverlayColor[0] = static_cast<float>(colorParams.Overlay.GetRed()) / 255.f;
		normalizedOverlayColor[1] = static_cast<float>(colorParams.Overlay.GetGreen()) / 255.f;
		normalizedOverlayColor[2] = static_cast<float>(colorParams.Overlay.GetBlue()) / 255.f;
		normalizedOverlayColor[3] = static_cast<float>(colorParams.Overlay.GetAlpha()) / 255.f;

		const std::vector<Color>& pixels = image.GetPixels();

		std::vector<Vec2D> points;

		Vec2D xAxis;
		Vec2D yAxis;

		float invXAxisLengthSq;
		float invYAxisLengthSq;

		GetObjectAxis(transform.Pos, sprite.width, sprite.height, transform.RotationAngle, transform.Scale, xAxis, yAxis, invXAxisLengthSq, invYAxisLengthSq, points);

		ScreenBuffer* screenBufferPtr = &m_BackBuffer;
		if (drawSurface == DrawSurface::Background)
			screenBufferPtr = &m_BackgroundBuffer;

		FillPolySprite(
			*screenBufferPtr,
			points,
			pixels,
			image.GetWidth(),
			normalizedOverlayColor,
			Vec2D(static_cast<float>(sprite.xPos), static_cast<float>(sprite.yPos)),
			Vec2D(static_cast<float>(sprite.width), static_cast<float>(sprite.height)),
			xAxis, yAxis,
			invXAxisLengthSq, invYAxisLengthSq,
			colorParams.Alpha,
			colorParams.BilinearFiltering, colorParams.Gradient, uvParams);
	}

	void SDLWindow::Draw(const SpriteSheet& ss, const std::string& spriteName, const DrawTransform& transform, const ColorParams& colorParams, const UVParams& uvParams)
	{
		Draw(ss.GetBMPImage(), ss.GetSprite(spriteName), transform, colorParams, uvParams);
	}

	void SDLWindow::Draw(const BitmapFont& font, const std::string& textLine, const DrawTransform& transform, const ColorParams& colorParams, const UVParams& uvParams)
	{
		uint32_t xPos = static_cast<uint32_t>(transform.Pos.GetX());

		const SpriteSheet& ss = font.GetSpriteSheet();

		for (char c : textLine)
		{
			if (c == ' ')
			{
				xPos += font.GetFontSpacingBetweenWords();
				continue;
			}

			Sprite sprite = ss.GetSprite(std::string(1, c));

			DrawTransform newTransform = transform;
			newTransform.Pos = Vec2D(static_cast<float>(xPos), transform.Pos.GetY());

			Draw(ss.GetBMPImage(), sprite, newTransform, colorParams, uvParams);

			xPos += sprite.width;

			xPos += font.GetFontSpacingBetweenLetters();
		}
	}

	void SDLWindow::ClearBackground()
	{
		m_BackgroundBuffer.Clear(m_ClearColor);
	}

	void SDLWindow::Shake(float power, float durationInSeconds)
	{
		m_ScreenShakeTimer = SecondsToMilliseconds(durationInSeconds);
		m_ScreenShakePower = power;
	}

	void SDLWindow::FillPolySprite(ScreenBuffer& screenBuffer, const std::vector<Vec2D>& points, const std::vector<Color>& imagePixels, uint32_t imageWidth, const float overlayColor[4], const Vec2D& spritePos, const Vec2D& spriteSize, const Vec2D& xAxis, const Vec2D& yAxis, const float invXAxisLengthSq, const float invYAxisLengthSq, float alpha, bool bilinearFilter, const GradientParams& gradient, const UVParams& uvParams)
	{
		if (points.size() > 0)
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

			for (int pixelY = static_cast<int>(std::roundf(top)); pixelY < static_cast<int>(std::roundf(bottom)); ++pixelY)
			{
				std::vector<float> nodeXVec;
				size_t j = points.size() - 1;

				for (size_t i = 0; i < points.size(); ++i)
				{
					float pointiY = points[i].GetY();
					float pointjY = points[j].GetY();
					if ((pointiY <= static_cast<float>(pixelY) && pointjY > static_cast<float>(pixelY)) || (pointjY <= static_cast<float>(pixelY) && pointiY > static_cast<float>(pixelY)))
					{
						float denom = pointjY - pointiY;
						if (IsEqual(denom, 0))
							continue;

						float x = points[i].GetX() + (pixelY - pointiY) / (denom) * (points[j].GetX() - points[i].GetX());
						nodeXVec.push_back(x);
					}
					j = i;
				}

				std::sort(nodeXVec.begin(), nodeXVec.end());
				for (size_t k = 0; k < nodeXVec.size(); k += 2)
				{
					if (nodeXVec[k] > right)
						break;

					if (k + 1 >= nodeXVec.size())
						break;

					if (nodeXVec[k + 1] > left)
					{
						if (nodeXVec[k] < left)
							nodeXVec[k] = left;

						if (nodeXVec[k + 1] > right)
							nodeXVec[k + 1] = right;

						for (int pixelX = static_cast<int>(std::roundf(nodeXVec[k])); pixelX < static_cast<int>(std::roundf(nodeXVec[k + 1])); ++pixelX)
						{
							Vec2D p(static_cast<float>(pixelX), static_cast<float>(pixelY));
							Vec2D uv = ConvertWorldSpaceToUVSpace(p, points[0], xAxis, yAxis, invXAxisLengthSq, invYAxisLengthSq);

							if (!bilinearFilter)
							{
								DrawUnfilteredPixel(
									screenBuffer,
									imagePixels,
									pixelX,
									pixelY,
									uv,
									imageWidth,
									spriteSize,
									spritePos,
									overlayColor,
									alpha,
									gradient,
									uvParams);
							}
							else
							{
								DrawBilinearFilteredPixel(
									screenBuffer,
									imagePixels,
									pixelX,
									pixelY,
									uv,
									imageWidth,
									spriteSize,
									spritePos,
									overlayColor,
									alpha,
									gradient,
									uvParams);
							}
						}
					}
				}
			}
		}
	}

	void SDLWindow::DrawBilinearFilteredPixel(ScreenBuffer& screenBuffer, const std::vector<Color>& imagePixels, int pixelX, int pixelY, const Vec2D& uv, uint32_t imageWidth, const Vec2D& spriteSize, const Vec2D& spritePos, const float overlayColor[4], float alpha, const GradientParams& gradient, const UVParams& uvParams)
	{
		float tx = 1.f + uv.GetX() * static_cast<float>(spriteSize.GetX() - 3.f);
		float ty = 1.f + uv.GetY() * static_cast<float>(spriteSize.GetY() - 3.f);

		uint32_t txInt = static_cast<uint32_t>(tx);
		uint32_t tyInt = static_cast<uint32_t>(ty);

		float fx = tx - static_cast<float>(txInt);
		float fy = ty - static_cast<float>(tyInt);


		unsigned int row = static_cast<unsigned int>(ty + spritePos.GetY());
		unsigned int col = static_cast<unsigned int>(tx + spritePos.GetX());

		auto pixelIndex = GetIndex(imageWidth, row, col);
		auto pixelIndex2 = GetIndex(imageWidth, row, col + 1);
		auto pixelIndex3 = GetIndex(imageWidth, row + 1, col);
		auto pixelIndex4 = GetIndex(imageWidth, row + 1, col + 1);

		if (pixelIndex < imagePixels.size() &&
			pixelIndex2 < imagePixels.size() &&
			pixelIndex3 < imagePixels.size() &&
			pixelIndex4 < imagePixels.size())
		{
			Color imageColor = imagePixels[pixelIndex];
			Color imageColor2 = imagePixels[pixelIndex2];
			Color imageColor3 = imagePixels[pixelIndex3];
			Color imageColor4 = imagePixels[pixelIndex4];

			Color result = Color::Lerp(Color::Lerp(imageColor, imageColor2, fx), Color::Lerp(imageColor3, imageColor4, fx), fy);

			float newOverlayColor[4] = {overlayColor[0], overlayColor[1], overlayColor[2], overlayColor[3]};

			Gradient(gradient, uv.GetX(), uv.GetY(), newOverlayColor);

			ClipUV(uv, uvParams, result);

			Color newColor = {
				static_cast<uint8_t>(result.GetRed() * newOverlayColor[0]),
				static_cast<uint8_t>(result.GetGreen() * newOverlayColor[1]),
				static_cast<uint8_t>(result.GetBlue() * newOverlayColor[2]),
				static_cast<uint8_t>(result.GetAlpha() * newOverlayColor[3] * alpha)};

			m_BackBuffer.SetPixel(pixelX, pixelY, newColor);
		}
	}

	void SDLWindow::DrawUnfilteredPixel(ScreenBuffer& screenBuffer, const std::vector<Color>& imagePixels, int pixelX, int pixelY, const Vec2D& uv, uint32_t imageWidth, const Vec2D& spriteSize, const Vec2D& spritePos, const float overlayColor[4], float alpha, const GradientParams& gradient, const UVParams& uvParams)
	{
		float tx = std::roundf(uv.GetX() * spriteSize.GetX());
		float ty = std::roundf(uv.GetY() * spriteSize.GetY());

		unsigned int row = static_cast<unsigned int>(ty + spritePos.GetY());
		unsigned int col = static_cast<unsigned int>(tx + spritePos.GetX());

		auto pixelIndex = GetIndex(imageWidth, row, col);
		if (pixelIndex < imagePixels.size())
		{
			float newOverlayColor[4] = {overlayColor[0], overlayColor[1], overlayColor[2], overlayColor[3]};
			Gradient(gradient, uv.GetX(), uv.GetY(), newOverlayColor);

			Color imageColor = imagePixels[pixelIndex];
			ClipUV(uv, uvParams, imageColor);

			Color newColor = {
				static_cast<uint8_t>(imageColor.GetRed() * newOverlayColor[0]),
				static_cast<uint8_t>(imageColor.GetGreen() * newOverlayColor[1]),
				static_cast<uint8_t>(imageColor.GetBlue() * newOverlayColor[2]),
				static_cast<uint8_t>(imageColor.GetAlpha() * newOverlayColor[3] * alpha)};

			m_BackBuffer.SetPixel(pixelX, pixelY, newColor);
		}
	}

	void SDLWindow::Gradient(const GradientParams& gradient, float u, float v, float overlayColor[4])
	{
		if (gradient.XParam == GradientXParam::NoXGradient && gradient.YParam == GradientYParam::NoYGradient)
			return;

		Color gradientOverlayColor = Color::White();
		Color xOverlay = Color::White();
		Color yOverlay = Color::White();

		//Section 3 - EXERCISE 2
		{

		}

		overlayColor[0] = static_cast<float>(gradientOverlayColor.GetRed()) / 255.f;
		overlayColor[1] = static_cast<float>(gradientOverlayColor.GetGreen()) / 255.f;
		overlayColor[2] = static_cast<float>(gradientOverlayColor.GetBlue()) / 255.f;
		overlayColor[3] = static_cast<float>(gradientOverlayColor.GetAlpha()) / 255.f;
	}

	void SDLWindow::ClipUV(const Vec2D& uv, const UVParams& uvParams, Color& imageColor)
	{
		//Section 4 - Exercise 1
		//TODO: implement

		//if(we shouldn't draw this part of the texture)
		//{
		//	imageColor = Color::ClearBlack();
		//}
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

			SDL_RendererInfo info;
			SDL_GetRendererInfo(m_Renderer, &info);
			int32_t foundIndex = -1;

			for (Uint32 i = 0; i < info.num_texture_formats; ++i)
			{
				auto iter = std::find(s_PreferredPixelFormats.begin(), s_PreferredPixelFormats.end(), std::string(SDL_GetPixelFormatName(info.texture_formats[i])));
				if (iter != s_PreferredPixelFormats.end())
				{
					foundIndex = i;
					break;
				}
			}

			assert(foundIndex != -1);
			m_PixelFormat = SDL_AllocFormat(info.texture_formats[foundIndex]);

			if (m_FastPath)
			{
				m_Texture = SDL_CreateTexture(m_Renderer, m_PixelFormat->format, SDL_TEXTUREACCESS_STREAMING, m_Width, m_Height);
				m_BackgroundTexture = SDL_CreateTexture(m_Renderer, m_PixelFormat->format, SDL_TEXTUREACCESS_STREAMING, m_Width, m_Height);
				SDL_SetTextureBlendMode(m_BackgroundTexture, SDL_BLENDMODE_NONE);
				SDL_SetTextureBlendMode(m_Texture, SDL_BLENDMODE_BLEND);
			}

			Color::InitColorFormat(m_PixelFormat);
			m_ClearColor = Color(rClear, gClear, bClear, aClear);

			m_BackBuffer.Init(m_Width, m_Height, m_PixelFormat->format);
			m_BackBuffer.Clear();
			m_BackgroundBuffer.Init(m_Width, m_Height, m_PixelFormat->format);
			m_BackgroundBuffer.Clear();
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
		
		if (m_BackgroundTexture)
		{
			SDL_DestroyTexture(m_BackgroundTexture);
			m_BackgroundTexture = nullptr;
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