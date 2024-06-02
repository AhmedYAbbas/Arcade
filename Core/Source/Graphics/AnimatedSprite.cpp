#include "CorePCH.h"
#include "Shapes/Rectangle.h"
#include "AnimatedSprite.h"
#include "Core/Window.h"

namespace Core
{
	AnimatedSprite::AnimatedSprite()
		: m_Position(Vec2D::Zero), m_SpriteSheet(nullptr), m_Angle(0.f), m_Scale(1.f), m_Alpha(1.f)
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

	void AnimatedSprite::Draw(Window& window, bool bilinearFilter, bool debug)
	{
		AnimationFrame frame = m_AnimationPlayer.GetCurrentAnimationFrame();
		Color frameColor = frame.FrameColor;

		if (!frame.FrameColorSet)
			frameColor = m_Color;

		if (debug)
			printf("frame: %s, frame number: %u\n", frame.Frame.c_str(), m_AnimationPlayer.GetCurrentFrameNumber());

		DrawTransform transform;
		transform.Pos = m_Position + frame.Offset;
		transform.Scale = m_Scale;
		transform.RotationAngle = m_Angle;

		ColorParams colorParams;
		colorParams.Alpha = m_Alpha;
		colorParams.BilinearFiltering = bilinearFilter;
		colorParams.Overlay = frameColor;
		colorParams.Gradient.XParam = GradientXParam::NoXGradient;
		colorParams.Gradient.YParam = GradientYParam::NoYGradient;

		UVParams uvParams;

		window.Draw(*m_SpriteSheet, frame.Frame, transform, colorParams, uvParams);

		//window.Draw(*m_SpriteSheet, frame.Frame, m_Position + frame.Offset, frameColor);

		if (!frame.Overlay.empty())
		{
			transform.Pos = m_Position;
			colorParams.Overlay = frame.OverlayColor;
			window.Draw(*m_SpriteSheet, frame.Overlay, transform, colorParams, uvParams);
			//window.Draw(*m_SpriteSheet, frame.Overlay, m_Position, frame.OverlayColor);
		}
	}

	void AnimatedSprite::SetAnimation(const std::string& animationName, bool looped)
	{
		if (m_AnimationPlayer.Play(animationName, looped))
			m_Size = m_AnimationPlayer.GetCurrentAnimation().GetSize();
	}

	void AnimatedSprite::Stop()
	{
		m_AnimationPlayer.Stop();
	}

	const Rectangle& AnimatedSprite::GetBoundingBox() const
	{
		m_BoundingBox.SetTopLeftPoint(m_Position);
		m_BoundingBox.SetBottomRightPoint(Vec2D(m_Position.GetX() + m_Size.GetX() - 1, m_Position.GetY() + m_Size.GetY() - 1));
		return m_BoundingBox;
	}

	uint32_t AnimatedSprite::GetTotalAnimationTimeInMS() const
	{
		if (m_AnimationPlayer.HasAnimation())
		{
			float msPerFrame = 1000.f / static_cast<float>(m_AnimationPlayer.GetCurrentAnimation().FPS());
			return static_cast<uint32_t>(msPerFrame) * static_cast<uint32_t>(m_AnimationPlayer.GetCurrentAnimation().NumFrames());
		}
		return 0;
	}
}