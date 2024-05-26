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
		void Draw(Window& window);

		void SetAnimation(const std::string& animationName, bool looped);
		Vec2D Size() const;
		void Pause();
		const Rectangle GetBoundingBox() const;

		inline Vec2D Position() const { return m_Position; }
		inline void SetPosition(const Vec2D& pos) { m_Position = pos; }
		inline void MoveBy(const Vec2D& delta) { m_Position += delta; }
		inline bool IsFinishedPlayingAnimation() const { return m_AnimationPlayer.IsFinishedPlaying(); }
		inline const Color& GetColor() const { return m_Color; }
		const SpriteSheet* GetSpriteSheet() const { return m_SpriteSheet; }

	private:
		const SpriteSheet* m_SpriteSheet;
		AnimationPlayer m_AnimationPlayer;
		Vec2D m_Position;
		Color m_Color;
	};
}