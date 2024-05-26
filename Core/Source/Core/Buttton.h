#pragma once

#include <string>
#include <functional>

#include "Shapes/Rectangle.h"
#include "Graphics/Color.h"
#include "Graphics/BitmapFont.h"
#include "InputAction.h"

namespace Core
{
	class Window;

	class Button
	{
	public:
		using ButtonAction = std::function<void(void)>;

		Button(const BitmapFont& bitmapFont, const Color& textColor, const Color& highlightColor = Color::White());

		void Init(Vec2D topLeft, unsigned int width, unsigned int height);
		void Draw(Window& window);
		void ExecuteAction();

		inline void SetButtonText(const std::string& text) { m_Title = text; }
		inline const std::string& GetButtonText() const { return m_Title; }
		inline void SetHighlighted(bool highlighted) { m_Highlighted = highlighted; }
		inline bool IsHighlighted() const { return m_Highlighted; }
		inline void SetButtonAction(ButtonAction action) { m_Action = action; }

	private:
		const BitmapFont& m_BitmapFont;
		std::string m_Title;
		Rectangle m_BoundingBox;

		bool m_Highlighted;
		ButtonAction m_Action;
		Color m_HighlightColor;
		Color m_TextColor;
	};
}