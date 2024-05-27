#pragma once

#include "GameUtils.h"

class Actor
{
public:
	virtual ~Actor() = default;

	virtual void Init(const Core::SpriteSheet& spriteSheet, const std::string& animationsPath, const Core::Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Core::Color& spriteColor = Core::Color::White());
	virtual void Update(uint32_t dt);
	virtual void Draw(Core::Window& window);

	virtual void Stop();
	Core::Rectangle GetEatingBoundingBox() const;

	inline bool IsFinishedAnimation() const { return m_Sprite.IsFinishedPlayingAnimation(); }
	inline const Core::Rectangle GetBoundingBox() const { return m_Sprite.GetBoundingBox(); }
	inline void MoveBy(const Core::Vec2D& delta) { m_Sprite.MoveBy(delta); }
	inline void MoveTo(const Core::Vec2D& pos) { m_Sprite.SetPosition(pos); }
	inline Core::Vec2D Position() const { m_Sprite.Position(); }
	inline PacmanMovement GetMovementDirection() const { return m_MovementDirection; }
	virtual inline void SetMovementDirection(PacmanMovement direction) { m_MovementDirection = direction; }
	inline const Core::Color& GetSpriteColor() const { return m_Sprite.GetColor(); }

protected:
	void SetAnimation(const std::string& animationName, bool looped);

	inline void ResetDelta() { m_Delta = Core::Vec2D::Zero; }
	inline void SetMovementSpeed(uint32_t movementSpeed) { m_MovementSpeed = movementSpeed; }

protected:
	Core::AnimatedSprite m_Sprite;

private:
	Core::Vec2D m_Delta;
	PacmanMovement m_MovementDirection;
	uint32_t m_MovementSpeed;
	bool m_UpdateSpriteOnUpdate;
};