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

		virtual void Init(const std::string& text, Vec2D topLeft, unsigned int width, unsigned height);
		virtual void Draw(Window& window);
		virtual void Update(uint32_t dt) {}
		virtual void ExecuteAction();

		inline void SetButtonText(const std::string& text) { m_Title = text; }
		inline const std::string& GetButtonText() const { return m_Title; }
		inline virtual void SetHighlighted(bool highlighted) { m_Highlighted = highlighted; }
		inline bool IsHighlighted() const { return m_Highlighted; }
		inline void SetButtonAction(ButtonAction action) { m_Action = action; }

	protected:
		void DrawHighlight(Core::Window& window);

	protected:
		Core::Vec2D m_DrawPosition;
		float m_TextScale;
		const BitmapFont& m_BitmapFont;
		std::string m_Title;
		Rectangle m_BoundingBox;

		bool m_Highlighted;
		ButtonAction m_Action;
		Color m_HighlightColor;
		Color m_TextColor;
	};
}