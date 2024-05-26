#include "CorePCH.h"
#include "AnimatedSprite.h"
#include "Shapes/Rectangle.h"
#include "Core/Window.h"

namespace Core
{
	AnimatedSprite::AnimatedSprite()
		: m_Position(Vec2D::Zero), m_SpriteSheet(nullptr)
	{
	}

	void AnimatedSprite::Init(const std::string& animationFilePath, const SpriteSheet& spriteSheet, const Color& color)
	{
		m_AnimationPlayer.Init(animationFilePath);
		m_SpriteSheet = &spriteSheet;
		m_Color = color;
	}

	void AnimatedSprite::Update(uint32_t dt)
	{
		m_AnimationPlayer.Update(dt);
	}

	void AnimatedSprite::Draw(Window& window)
	{
		AnimationFrame frame = m_AnimationPlayer.GetCurrentAnimationFrame();
		Color frameColor = frame.FrameColor;

		if (!frame.FrameColorSet)
			frameColor = m_Color;

		window.Draw(*m_SpriteSheet, frame.Frame, m_Position + frame.Offset, frameColor);

		if (!frame.Overlay.empty())
			window.Draw(*m_SpriteSheet, frame.Overlay, m_Position, frame.OverlayColor);
	}

	void AnimatedSprite::SetAnimation(const std::string& animationName, bool looped)
	{
		m_AnimationPlayer.Play(animationName, looped);
	}

	Vec2D AnimatedSprite::Size() const
	{
		return m_AnimationPlayer.GetCurrentAnimationFrame().Size;
	}

	void AnimatedSprite::Pause()
	{
		m_AnimationPlayer.Stop();
	}

	const Rectangle AnimatedSprite::GetBoundingBox() const
	{
		return Rectangle(m_Position, Size().GetX(), Size().GetY());
	}
}