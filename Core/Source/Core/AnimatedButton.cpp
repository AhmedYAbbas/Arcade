#include "CorePCH.h"
#include "AnimatedButton.h"
#include "Window.h"
#include "Application.h"

namespace Core
{
	constexpr int TOTAL_ANIMATION_TIME = 1000;
	constexpr bool BOUNCE_FULL_TEXT = false;

	AnimatedButton::AnimatedButton(const BitmapFont& bitmapFont, const Color& textColor, const Color& highlightColor)
		: Button(bitmapFont, textColor, highlightColor), m_OriginalColor(textColor), m_TotalAnimationTime(TOTAL_ANIMATION_TIME), m_AnimationTime(0), m_IsAnimating(false), m_ExecuteTriggered(false)
	{
	}

	void AnimatedButton::Init(const std::string& text, Vec2D topLeft, unsigned int width, unsigned height)
	{
		Button::Init(text, topLeft, width, height);

		if (!BOUNCE_FULL_TEXT)
			SetupLetterAnimationData(text);
	}

	void AnimatedButton::Draw(Window& window)
	{
		if (!IsHighlighted() || m_ExecuteTriggered)
			Button::Draw(window);
		else
			DrawButtonAnimation(window);
	}

	void AnimatedButton::Update(uint32_t dt)
	{
		if (IsHighlighted())
			m_HighlightColor.SetAlpha(static_cast<uint8_t>(std::round(255.f * Pulse(0.5f))));

		m_DrawPosition = m_BitmapFont.GetDrawPosition(m_Title, m_BoundingBox, BitmapFontXAlignment::Center, BitmapFontYAlignment::Center);
		m_TextColor = m_OriginalColor;

		if (IsHighlighted())
			m_TextColor = Color::White();

		if (m_IsAnimating && IsHighlighted() && !m_ExecuteTriggered)
		{
			float t = UpdateAnimationTime(dt);
			if (BOUNCE_FULL_TEXT)
			{
				const Vec2D endPosition = m_BitmapFont.GetDrawPosition(m_Title, m_BoundingBox, BitmapFontXAlignment::Center, BitmapFontYAlignment::Center);
				m_DrawPosition = Vec2D::Lerp(
					Vec2D(m_BoundingBox.GetTopLeftPoint().GetX(), endPosition.GetY()),
					endPosition,
					t,
					Ease::EaseOutBounce);
			}
			else
				UpdateLetterAnimationData();
		}

		if (m_ExecuteTriggered)
		{
			float t = UpdateAnimationTime(dt);
			if (IsGreaterThanOrEqual(t, 1.f))
			{
				Button::ExecuteAction();
				m_ExecuteTriggered = false;
				return;
			}

			auto HighPulse = [](float) -> float
			{
				return Pulse(10);
			};

			m_TextColor = Color::Lerp(m_OriginalColor, Color::Orange(), t, HighPulse);
		}
	}

	void AnimatedButton::ExecuteAction()
	{
		m_ExecuteTriggered = true;
		m_IsAnimating = true;
		m_AnimationTime = 0;
	}

	void AnimatedButton::SetHighlighted(bool highlighted)
	{
		Button::SetHighlighted(highlighted);
		m_IsAnimating = true;
		m_AnimationTime = 0;
	}

	float AnimatedButton::UpdateAnimationTime(uint32_t dt, float totalAnimTimeMod)
	{
		m_AnimationTime += static_cast<int>(dt);

		int totalAnimTime = static_cast<int>(m_TotalAnimationTime * totalAnimTimeMod);
		if (m_AnimationTime >= totalAnimTime)
		{
			m_AnimationTime = totalAnimTime;
			m_IsAnimating = false;
		}

		float elapsedSeconds = MillisecondsToSeconds(m_AnimationTime);
		float totalSeconds = MillisecondsToSeconds(totalAnimTime);

		return elapsedSeconds / totalSeconds;
	}

	void AnimatedButton::SetupLetterAnimationData(const std::string& text)
	{
		LetterAnimationData letterAnimData;
		for (size_t i = 0; i < text.size(); ++i)
		{
			letterAnimData.Letter = std::string(1, text[i]);
			letterAnimData.DrawPosition = m_DrawPosition;

			unsigned int previousLettersWidth = 0;
			for (int j = i; j > 0; --j)
			{
				previousLettersWidth += m_BitmapFont.GetSizeOf(std::string(1, text[j - 1])).Width;
			}
			letterAnimData.EndPosition = m_DrawPosition + Vec2D((i * m_BitmapFont.GetFontSpacingBetweenLetters()) + previousLettersWidth, 0);

			letterAnimData.StartPosition = Vec2D(m_BoundingBox.GetTopLeftPoint().GetX(), letterAnimData.EndPosition.GetY());

			float timeForLetterToAnimate = static_cast<float>(TOTAL_ANIMATION_TIME) / static_cast<float>(text.size());
			letterAnimData.Delay = (text.size() - 1 - i) * timeForLetterToAnimate;// + timeForLetterToAnimate;

			m_LetterAnimations.push_back(letterAnimData);
		}
	}

	void AnimatedButton::UpdateLetterAnimationData()
	{
		for (auto& letter : m_LetterAnimations)
		{
			float clampedT = Clamp(static_cast<float>(m_AnimationTime) / (static_cast<float>(TOTAL_ANIMATION_TIME)), 0.f, 1.f);
			letter.DrawPosition = Vec2D::Lerp(letter.StartPosition, letter.EndPosition, clampedT, Ease::EaseOutBack);
		}
	}

	void AnimatedButton::DrawButtonAnimation(Window& window)
	{
		DrawTransform transform;
		transform.Pos = m_DrawPosition;

		ColorParams colorParams;
		colorParams.Overlay = m_TextColor;

		colorParams.Gradient.XParam = GradientXParam::NoXGradient;
		colorParams.Gradient.YParam = GradientYParam::NoYGradient;
		colorParams.Gradient.Color1 = Color::Green();
		colorParams.Gradient.Color2 = Color::Red();

		UVParams uvParams;

		DrawHighlight(window);

		if (BOUNCE_FULL_TEXT)
			window.Draw(m_BitmapFont, m_Title, transform, colorParams, uvParams);
		else
		{
			for (const auto& letterAnimData : m_LetterAnimations)
			{
				transform.Pos = letterAnimData.DrawPosition;
				if (m_AnimationTime >= letterAnimData.Delay)
				{
					window.Draw(m_BitmapFont, letterAnimData.Letter, transform, colorParams, uvParams);
				}
				//window.Draw(letterAnimData.EndPosition, Color::Magenta());
				//window.Draw(letterAnimData.StartPosition, Color::Magenta());
				//window.Draw(Vec2D(104.f, 96.f), Color::Blue());
			}
		}
	}
}