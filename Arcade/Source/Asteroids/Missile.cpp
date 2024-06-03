#include "ArcadePCH.h"
#include "Missile.h"
#include "AsteroidConstants.h"

Missile::Missile(const Core::SpriteSheet& spriteSheet, const Core::Vec2D& initialPos, const Core::Vec2D& vel, float yaw, float scale)
	: m_Velocity(vel), m_Yaw(yaw), m_Scale(scale)
{
	m_Sprite.Init(Core::Application::Get().GetBasePath() + "AsteroidsAnimations.txt", spriteSheet);
	m_Sprite.SetAnimation("missile", true);

	m_BoundingCircle = Core::Circle(initialPos, m_Sprite.GetBoundingBox().GetWidth() / 2 - 1);
	m_BoundingCircle.MoveTo(initialPos);
}

AmmoType Missile::GetType() const
{
	return AmmoType::Missile;
}

bool Missile::IsOutOfBounds(int leftSide, int top, int screenWidth, int screenHeight) const
{
	Core::Vec2D position = m_BoundingCircle.GetCenterPoint();
	float width = GetWidth();

	return position.GetX() > screenWidth + width || position.GetX() < leftSide - width || position.GetY() < top - width || position.GetY() > screenHeight + width;
}

bool Missile::Intersects(const Core::Circle& circle, std::vector<Core::Vec2D>& intersections) const
{
	bool intersects = m_BoundingCircle.Intersects(circle);

	if (intersects)
		intersections.push_back(m_BoundingCircle.GetCenterPoint());

	return intersects;
}

const Core::Vec2D& Missile::GetVelocity() const
{
	return m_Velocity;
}

float Missile::Scale() const
{
	return m_Scale;
}

void Missile::Update(uint32_t dt)
{
	m_Sprite.Update(dt);
	m_BoundingCircle.MoveBy(m_Velocity * Core::MillisecondsToSeconds(dt));
}

void Missile::Draw(Core::Window& window)
{
	const auto& sprite = m_Sprite.GetSpriteSheet()->GetSprite("missile_1");

	m_Sprite.SetAngle(m_Yaw);
	m_Sprite.SetPosition(m_BoundingCircle.GetCenterPoint() - Core::Vec2D(static_cast<float>(sprite.width) / 2.f, static_cast<float>(sprite.height) / 2.f) * m_Scale);
	m_Sprite.SetScale(m_Scale);
	m_Sprite.Draw(window, BILINEAR_FILTERING);
}

bool Missile::DisappearsOnHit() const
{
	return true;
}

void Missile::DrawDebug(Core::Window& window)
{
	window.Draw(m_BoundingCircle, Core::Color::Red());
	window.Draw(Core::Line(m_BoundingCircle.GetCenterPoint(), m_Velocity + m_BoundingCircle.GetCenterPoint()), Core::Color::Red());
}
