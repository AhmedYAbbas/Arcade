#pragma once

#include "Buttton.h"

namespace Core
{
	class AnimatedButton : public Button
	{
	public:
		AnimatedButton(const BitmapFont& bitmapFont, const Color& textColor, const Color& highlightColor = Color::White());

		virtual void Init(const std::string& text, Vec2D topLeft, unsigned int width, unsigned height) override;
		virtual void Draw(Window& window) override;
		virtual void Update(uint32_t dt) override;
		virtual void ExecuteAction() override;

		virtual void SetHighlighted(bool highlighted) override;

	private:
		float UpdateAnimationTime(uint32_t dt, float totalAnimTimeMod = 1.f);
		void SetupLetterAnimationData(const std::string& text);
		void UpdateLetterAnimationData();
		void DrawButtonAnimation(Window& window);

	private:
		Color m_OriginalColor;
		int m_TotalAnimationTime;
		int m_AnimationTime;
		bool m_IsAnimating;

		bool m_ExecuteTriggered;

		struct LetterAnimationData
		{
			std::string Letter;
			Vec2D EndPosition;
			Vec2D StartPosition;
			int Delay;

			Vec2D DrawPosition;
		};

		std::vector<LetterAnimationData> m_LetterAnimation;
	};
}