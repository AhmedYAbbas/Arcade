#pragma once

#include "ShipAmmo.h"

class Laser : public ShipAmmo
{
public:
	Laser(const Core::Vec2D& origin, const Core::Vec2D& velocity, const Core::Color& color1, const Core::Color& color2, bool cutting, Ease::EasingFunc easingFunction = Ease::EaseLinear);

	virtual AmmoType GetType() const override;
	virtual bool IsOutOfBounds(int leftSide, int top, int screenWidth, int screenHeight) const override;
	virtual bool Intersects(const Core::Circle& circle, std::vector<Core::Vec2D>& intersections) const override;
	virtual const Core::Vec2D& GetVelocity() const override;
	virtual float Scale() const override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;
	virtual bool DisappearsOnHit() const override;

private:
	Core::Ray2D m_LaserRay;
	Core::Color m_Color1;
	Core::Color m_Color2;
	uint32_t m_Time;
	bool m_Cutting;
	Ease::EasingFunc m_EasingFunc;
};