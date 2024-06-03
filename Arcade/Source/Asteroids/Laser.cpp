#include "ArcadePCH.h"
#include "Laser.h"

Laser::Laser(const Core::Vec2D& origin, const Core::Vec2D& velocity, const Core::Color& color1, const Core::Color& color2, bool cutting, Ease::EasingFunc easingFunction)
	: m_Color1(color1), m_Color2(color2), m_EasingFunc(easingFunction), m_Time(0), m_Cutting(cutting)
{
	m_LaserRay.Init(origin, velocity);
}

AmmoType Laser::GetType() const
{
	return AmmoType::Laser;
}

bool Laser::IsOutOfBounds(int leftSide, int top, int screenWidth, int screenHeight) const
{
	constexpr int LASER_FUDGE_FACTOR = 50;
	leftSide -= LASER_FUDGE_FACTOR;
	top -= LASER_FUDGE_FACTOR;
	screenWidth += LASER_FUDGE_FACTOR;
	screenHeight += LASER_FUDGE_FACTOR;

	Core::Vec2D laserEndPosition = Core::MillisecondsToSeconds(m_Time) * m_LaserRay.GetVelocity() + m_LaserRay.GetOrigin();
	return (laserEndPosition.GetX() > screenWidth || laserEndPosition.GetX() < leftSide ||
		laserEndPosition.GetY() > screenHeight || laserEndPosition.GetY() < top);
}

bool Laser::Intersects(const Core::Circle& circle, std::vector<Core::Vec2D>& intersections) const
{
	float t1 = -1.0f;
	float t2 = -1.0f;

	bool intersects = m_LaserRay.Intersects(circle, t1, t2);

	if (!intersects)
		return false;

	float currentTime = Core::MillisecondsToSeconds(m_Time);
	float intersectTime1 = -1.0f;
	float intersectTime2 = -1.0f;
	int numIntersections = 0;

	if (t1 > 0 && t2 > 0)
	{
		intersectTime1 = std::min(t1, t2);
		intersectTime2 = std::max(t1, t2);
	}
	else if (t1 > 0 && t2 < 0)
		intersectTime1 = t1;
	else if (t2 > 0 && t1 < 0)
		intersectTime1 = t2;

	if (intersectTime1 > 0 && currentTime > intersectTime1)
	{
		intersections.push_back(intersectTime1 * m_LaserRay.GetVelocity() + m_LaserRay.GetOrigin());
		++numIntersections;
	}

	if (intersectTime2 > 0 && ((numIntersections > 0 && m_Cutting) || currentTime > intersectTime2))
	{
		if (m_Cutting)
		{
			intersections.push_back(intersectTime2 * m_LaserRay.GetVelocity() + m_LaserRay.GetOrigin());
			++numIntersections;
		}
	}

	return numIntersections > 0;
}

const Core::Vec2D& Laser::GetVelocity() const
{
	return m_LaserRay.GetVelocity();
}

float Laser::Scale() const
{
	return 1.f;
}

void Laser::Update(uint32_t dt)
{
	m_Time += dt;
}

void Laser::Draw(Core::Window& window)
{
	float t = Core::Pulse(5);
	Core::Color c = Core::Color::Lerp(m_Color1, m_Color2, t);

	Core::Line laser = m_LaserRay.GetLineSegmentForTime(Core::MillisecondsToSeconds(m_Time), m_EasingFunc);
	if (m_Cutting)
	{
		Core::Vec2D offset = (laser.GetP1() - laser.GetP0()).GetCounterClockwisePerpendicularUnitVector();

		Core::Line laserLeft = Core::Line(laser.GetP0() - offset, laser.GetP1() - offset);
		Core::Line laserRight = Core::Line(laser.GetP0() + offset, laser.GetP1() + offset);

		window.Draw(laser, Core::Color::White());
		window.Draw(laserLeft, c);
		window.Draw(laserRight, c);
	}
	else
		window.Draw(laser, c);
}

bool Laser::DisappearsOnHit() const
{
	return !m_Cutting;
}
