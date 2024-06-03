#include "ArcadePCH.h"
#include "PowerUp.h"
#include "AsteroidConstants.h"

static constexpr float PULSE_TIME = 1000.f;

PowerUp::PowerUp()
	: m_SpriteSheet(nullptr), m_SpriteName(""), m_BBox(), m_BoundingCircle(), m_Velocity(Core::Vec2D::Zero), m_LifeTime(0), m_Effect(nullptr)
{
}

void PowerUp::Init(const Core::SpriteSheet& spriteSheet, const std::string& spriteName, const Core::Vec2D& initialPos, const Core::Vec2D& velocity, float lifeTime, PowerUpEffect effect)
{
	m_SpriteSheet = &spriteSheet;
	m_SpriteName = spriteName;
	const auto& sprite = spriteSheet.GetSprite(spriteName);
	float spriteWidth = static_cast<float>(sprite.width);
	float spriteHeight = static_cast<float>(sprite.height);

	m_BoundingCircle = Core::Circle(initialPos, spriteWidth / 2.f);
	m_Velocity = velocity;
	m_LifeTime = Core::SecondsToMilliseconds(lifeTime);
	m_Effect = effect;

	m_BBox = Core::Rectangle(Core::Vec2D(initialPos.GetX() - spriteWidth / 2.f, initialPos.GetY() - spriteHeight / 2.f), static_cast<uint32_t>(spriteWidth), static_cast<uint32_t>(spriteHeight));
}

bool PowerUp::Update(uint32_t dt)
{
	if (m_LifeTime > 0)
	{
		m_LifeTime -= dt;
		if (m_LifeTime <= 0)
			m_LifeTime = 0;

		m_BoundingCircle.MoveBy(m_Velocity * Core::MillisecondsToSeconds(dt));
		m_BBox.MoveBy(m_Velocity * Core::MillisecondsToSeconds(dt));
	}
	return IsActive();
}

void PowerUp::Draw(Core::Window& window)
{
	if (IsActive())
	{
		float alpha = 1.0f;
		if (Core::IsLessThanOrEqual(static_cast<float>(m_LifeTime), PULSE_TIME))
			alpha = Core::Pulse(5.0f);

		const auto& sprite = m_SpriteSheet->GetSprite(m_SpriteName);

		Core::DrawTransform transform;
		transform.Pos = m_BoundingCircle.GetCenterPoint() - Core::Vec2D(std::roundf(static_cast<float>(sprite.width / 2)), std::roundf(static_cast<float>(sprite.height / 2)));
		transform.Scale = 1.f;
		transform.RotationAngle = 0.f;

		Core::ColorParams colorParams;
		colorParams.Alpha = alpha;
		colorParams.BilinearFiltering = BILINEAR_FILTERING;
		colorParams.Gradient.XParam = Core::GradientXParam::NoXGradient;
		colorParams.Gradient.YParam = Core::GradientYParam::NoYGradient;
		colorParams.Overlay = Core::Color::White();

		Core::UVParams uvParams;
		window.Draw(*m_SpriteSheet, m_SpriteName, transform, colorParams, uvParams);
	}
}

void PowerUp::ExecuteEffect()
{
	m_Effect();
	m_LifeTime = 0;
}

void PowerUp::SetPosition(const Core::Vec2D& newPosition)
{
	m_BoundingCircle.MoveTo(newPosition);
	m_BBox.MoveTo(Core::Vec2D(newPosition.GetX() - m_BoundingCircle.GetRadius(), newPosition.GetY() - m_BoundingCircle.GetRadius()));
}
