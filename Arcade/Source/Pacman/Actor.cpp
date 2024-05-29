#include "ArcadePCH.h"
#include "Actor.h"

void Actor::Init(const Core::SpriteSheet& spriteSheet, const std::string& animationsPath, const Core::Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Core::Color& spriteColor)
{
	m_MovementDirection = PacmanMovement::None;
	m_Sprite.Init(animationsPath, spriteSheet, spriteColor);
	m_UpdateSpriteOnUpdate = updateSpriteOnMovement;
	m_MovementSpeed = movementSpeed;
	m_Delta = Core::Vec2D::Zero;
	m_Sprite.SetPosition(initialPos);
}

void Actor::Update(uint32_t dt)
{
	if (m_MovementDirection != PacmanMovement::None)
	{
		Core::Vec2D delta = GetMovementVector(m_MovementDirection) * m_MovementSpeed;
		m_Delta += delta * Core::MillisecondsToSeconds(dt);
		if (std::fabsf(m_Delta.GetX()) >= 1)
		{
			int dx = static_cast<int>(std::fabsf(m_Delta.GetX()));
			if (m_Delta.GetX() < 0.f)
			{
				m_Sprite.MoveBy(Core::Vec2D(-dx, 0));
				m_Delta.SetX(m_Delta.GetX() + dx);
			}
			else
			{
				m_Sprite.MoveBy(Core::Vec2D(dx, 0));
				m_Delta.SetX(m_Delta.GetX() - dx);
			}
		}
		else if (std::fabsf(m_Delta.GetY()) >= 1)
		{
			int dy = static_cast<int>(std::fabsf(m_Delta.GetY()));
			if (m_Delta.GetY() < 0.f)
			{
				m_Sprite.MoveBy(Core::Vec2D(0, -dy));
				m_Delta.SetY(m_Delta.GetY() + dy);
			}
			else
			{
				m_Sprite.MoveBy(Core::Vec2D(0, dy));
				m_Delta.SetY(m_Delta.GetY() - dy);
			}
		}
		m_Sprite.Update(dt);
	}
	if (m_UpdateSpriteOnUpdate && m_MovementDirection == PacmanMovement::None)
		m_Sprite.Update(dt);
}

void Actor::Draw(Core::Window& window)
{
	m_Sprite.Draw(window);
}

void Actor::Stop()
{
	SetMovementDirection(PacmanMovement::None);
	m_Sprite.Stop();
}

Core::Rectangle Actor::GetEatingBoundingBox() const
{
	return Core::Rectangle::Inset(GetBoundingBox(), Core::Vec2D(3, 3));
}

void Actor::SetAnimation(const std::string& animationName, bool looped)
{
	m_Sprite.SetAnimation(animationName, looped);
}
