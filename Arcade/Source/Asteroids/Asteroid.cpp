#include "ArcadePCH.h"
#include "Asteroid.h"
#include "AsteroidConstants.h"

Asteroid::Asteroid()
	: m_SpriteName(""),
	m_BoundingCircle(),
	m_Velocity(),
	m_Size(AsteroidSize::None),
	m_RotationRate(0),
	m_Rotation(0),
	m_Hit(false),
	m_Destroyed(false),
	m_Split(false),
	m_SplitPoint1(Core::Vec2D::Zero),
	m_SplitPoint2(Core::Vec2D::Zero),
	m_SplitTime(0)
{
}

void Asteroid::Init(const Core::SpriteSheet& spriteSheet, const std::string& spriteName, AsteroidSize size, const Core::Vec2D& initialPos, const Core::Vec2D& velocity, float rotationRate)
{
	m_SpriteName = spriteName;
	m_Sprite.Init(Core::Application::Get().GetBasePath() + "AsteroidsAnimations.txt", spriteSheet);

	m_Velocity = velocity;
	const auto& sprite = spriteSheet.GetSprite(spriteName);
	m_RotationRate = rotationRate;
	m_BoundingCircle = Core::Circle(initialPos, static_cast<float>(sprite.width) / 2.f);
	m_SpawnPosition = initialPos;
	m_Size = size;
}

void Asteroid::Update(uint32_t dt)
{
	if (m_Destroyed)
		return;
	
	if (m_Hit)
	{
		m_Sprite.Update(dt);

		if (m_Split)
			m_SplitTime += dt;

		if (m_Sprite.IsFinishedPlayingAnimation())
		{
			m_Destroyed = true;
			return;
		}
	}

	m_BoundingCircle.MoveBy(m_Velocity * Core::MillisecondsToSeconds(dt));
	m_Rotation += m_RotationRate * Core::MillisecondsToSeconds(dt);
	if (m_Rotation >= Core::TWO_PI)
		m_Rotation -= Core::TWO_PI;
}

void Asteroid::Draw(Core::Window& window)
{
	if (m_Destroyed)
		return;

	if (!m_Hit)
	{
		const auto& sprite = m_Sprite.GetSpriteSheet()->GetSprite(m_SpriteName);

		Core::DrawTransform transform;
		transform.Pos = m_BoundingCircle.GetCenterPoint() - Core::Vec2D(static_cast<float>(sprite.width) / 2.f, static_cast<float>(sprite.height) / 2.f);
		transform.RotationAngle = m_Rotation;
		transform.Scale = 1.f;

		Core::ColorParams colorParams;
		colorParams.Alpha = 1.f;
		colorParams.Gradient.XParam = Core::GradientXParam::NoXGradient;
		colorParams.Gradient.YParam = Core::GradientYParam::NoYGradient;
		colorParams.BilinearFiltering = BILINEAR_FILTERING;
		colorParams.Overlay = Core::Color::White();

		Core::UVParams uvParams;
		window.Draw(*m_Sprite.GetSpriteSheet(), m_SpriteName, transform, colorParams, uvParams);
	}
	else
	{
		m_Sprite.Draw(window, BILINEAR_FILTERING);
		if (m_Split)
		{
			float alpha = static_cast<float>(m_Sprite.GetTotalAnimationTimeInMS() - m_SplitTime) / static_cast<float>(m_Sprite.GetTotalAnimationTimeInMS());
			DrawAsteroidSplit(window, alpha);
		}
	}
}

bool Asteroid::IsOutOfBounds(const Core::Vec2D& largestSprite) const
{
	if (GetPosition().GetX() > Core::Application::Get().GetWindow().GetWidth() + largestSprite.GetX()
		|| GetPosition().GetX() < -largestSprite.GetX()
		|| GetPosition().GetY() < -largestSprite.GetY()
		|| GetPosition().GetY() > Core::Application::Get().GetWindow().GetHeight() + largestSprite.GetY())
	{
		return !Core::IsEqual(GetPosition().GetX(), m_SpawnPosition.GetX()) && !Core::IsEqual(GetPosition().GetY(), m_SpawnPosition.GetY());
	}

	return false;
}

void Asteroid::HitByAmmo(const std::vector<Core::Vec2D>& intersections)
{
	assert(intersections.size() > 0 && "Not hit by anything!");

	m_Hit = true;
	m_Sprite.SetAnimation("explosion", false);
	const Core::Rectangle& bbox = m_Sprite.GetBoundingBox();
	m_Sprite.SetPosition(intersections[0] - Core::Vec2D(bbox.GetWidth() / 2, bbox.GetHeight() / 2));
	if (intersections.size() > 1)
	{
		m_Split = true;
		m_SplitPoint1 = intersections[0];
		m_SplitPoint2 = intersections[1];
		m_SplitTime = 0;

		const auto& sprite = m_Sprite.GetSpriteSheet()->GetSprite(m_SpriteName);
		m_PosAtSplitTime = m_BoundingCircle.GetCenterPoint() - Core::Vec2D(static_cast<float>(sprite.width) / 2.f, static_cast<float>(sprite.height) / 2.f);
		m_RotationAtSplit = m_Rotation;
	}
}

void Asteroid::DrawDebug(Core::Window& window)
{
	window.Draw(m_BoundingCircle, Core::Color::White());
	window.Draw(Core::Line(GetPosition(), GetPosition() + m_Velocity), Core::Color::White());
}

void Asteroid::DrawAsteroidSplit(Core::Window& window, float alpha)
{
	//Section 4 - Exercise 1
	//TODO: implement

	/*
		figure out how to get the mSplitPoint1 and mSplitPoint2 of the uvParams

		Helpful functions:
		ConvertWorldSpaceToUVSpace
		GetCounterClockwisePerpendicularUnitVector

	*/

	/*
	You can draw like this:

	DrawTransform transform;
	transform.pos = //? - one side of the asteroid
	transform.rotationAngle = mRotationAtSplit;
	transform.scale = 1.0f;

	ColorParams colorParams;
	colorParams.bilinearFiltering = BILINEAR_FILTERING;
	colorParams.overlay = Color::White();
	colorParams.alpha = alpha;

	UVParams uvParams;
	uvParams.mSplitPoint1 = //?
	uvParams.mSplitPoint2 = //?
	uvParams.mOrientation = INSIDE

	theScreen.Draw(
		*mSprite.GetSpriteSheet(),
		mSpriteName,
		transform,
		colorParams,
		uvParams);

	transform.pos = //? - other side of the asteroid
	uvParams.mOrientation = OUTSIDE;

	theScreen.Draw(
		*mSprite.GetSpriteSheet(),
		mSpriteName,
		transform,
		colorParams,
		uvParams);

	*/
}
