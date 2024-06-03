#pragma once

#include "ShipAmmo.h"

class Missile : public ShipAmmo
{
public:
	Missile(const Core::SpriteSheet& spriteSheet, const Core::Vec2D& initialPos, const Core::Vec2D& vel, float yaw, float scale);

	virtual AmmoType GetType() const override;
	virtual bool IsOutOfBounds(int leftSide, int top, int screenWidth, int screenHeight) const override;
	virtual bool Intersects(const Core::Circle& circle, std::vector<Core::Vec2D>& intersections) const override;
	virtual const Core::Vec2D& GetVelocity() const override;
	virtual float Scale() const override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;
	virtual bool DisappearsOnHit() const override;

	inline float GetWidth() const { return m_BoundingCircle.GetRadius() * 2.f; }

private:
	void DrawDebug(Core::Window& window);

private:
	Core::AnimatedSprite m_Sprite;
	Core::Circle m_BoundingCircle;
	Core::Vec2D m_Velocity;
	float m_Yaw;
	float m_Scale;
};