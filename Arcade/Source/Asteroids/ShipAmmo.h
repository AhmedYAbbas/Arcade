#pragma once

#include "Core.h"

enum class AmmoType
{
	Missile = 0,
	Laser
};

class ShipAmmo
{
public:
	virtual ~ShipAmmo() = default;

	inline virtual size_t GetID() const { return m_ID; }
	inline virtual void SetID(size_t id) { m_ID = id; }

	virtual AmmoType GetType() const = 0;
	virtual bool IsOutOfBounds(int leftSide, int top, int screenWidth, int screenHeight) const = 0;
	virtual bool Intersects(const Core::Circle& circle, std::vector<Core::Vec2D>& intersections) const = 0;
	virtual const Core::Vec2D& GetVelocity() const = 0;
	virtual float Scale() const = 0;
	virtual void Update(uint32_t dt) = 0;
	virtual void Draw(Core::Window& window) = 0;
	virtual bool DisappearsOnHit() const = 0;

private:
	size_t m_ID;
};