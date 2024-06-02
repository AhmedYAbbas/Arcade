#pragma once

#include "AnimationPlayer.h"
#include "Color.h"
#include "Utility/Vec2D.h"

namespace Core
{
	class Rectangle;
	class Window;
	class SpriteSheet;

	class AnimatedSprite
	{
	public:
		AnimatedSprite();

		void Init(const std::string& animationFilePath, const SpriteSheet& spriteSheet, const Color& color = Color::White());
		void Update(uint32_t dt);
		void Draw(Window& window, bool bilinearFilter = false, bool debug = false);

		void SetAnimation(const std::string& animationName, bool looped);
		void Stop();
		const Rectangle& GetBoundingBox() const;

		inline Vec2D Position() const { return m_Position; }
		inline void SetPosition(const Vec2D& pos) { m_Position = pos; }
		inline void MoveBy(const Vec2D& delta) { m_Position += delta; }
		inline bool IsFinishedPlayingAnimation() const { return m_AnimationPlayer.IsFinishedPlaying(); }
		inline const Color& GetColor() const { return m_Color; }
		inline const SpriteSheet* GetSpriteSheet() const { return m_SpriteSheet; }

		inline void PrintFrame() { m_AnimationPlayer.PrintFrame(); }
		inline Vec2D Size() const { return m_Size; }
		inline float GetAngle() const { return m_Angle; }
		inline void SetAngle(float angle) { m_Angle = angle; }
		inline Vec2D GetCenterPos() const { return m_BoundingBox.GetCenterPoint(); }

		inline void SetScale(float scale) { m_Scale = scale; }
		inline float GetScale() const { return m_Scale; }
		inline void SetAlpha(float alpha) { m_Alpha = alpha; }
		inline float GetAlpha() const { return m_Alpha; }

		uint32_t GetTotalAnimationTimeInMS() const;

	private:
		const SpriteSheet* m_SpriteSheet;
		AnimationPlayer m_AnimationPlayer;
		Vec2D m_Position;
		Color m_Color;

		Vec2D m_Size;
		mutable Rectangle m_BoundingBox;
		float m_Angle;
		float m_Scale;
		float m_Alpha;
	};
}