#include "ArcadePCH.h"
#include "Ship.h"
#include "AsteroidConstants.h"
#include "Missile.h"
#include "Laser.h"

static constexpr float ANGLE_PER_SECOND = Core::PI;
static constexpr float SHIP_SPEED = 80;
static constexpr float MISSILE_SPEED = 150;
static constexpr float LASER_SPEED = 800;
static constexpr float GROW_SCALE = 1.5f;
static constexpr uint32_t GROW_TRANSITION_TIME = 1000;
static constexpr uint32_t STAY_BIG_SIZE_TIME = 7000 - GROW_TRANSITION_TIME * 2; // in milliseconds
static constexpr uint32_t LASER_USAGE_TIME = 10000; //in milliseconds

Ship::Ship()
	: m_ShipYawMovement(AsteroidsShipYawMovement::ShipHeadingNone),
	m_Yaw(0),
	m_Speed(0),
	m_AmmoIDGen(0),
	m_Hit(false),
	m_Dead(false),
	m_CurrentAmmoType(AmmoType::Laser),
	m_GrowState(GrowState::None),
	m_ScaleTimer(0),
	m_Scale(1.f),
	m_UseLaserTimer(0)
{
}

void Ship::Init(const Core::SpriteSheet& spriteSheet, const Core::Vec2D& initialPos)
{
	m_Sprite.Init(Core::Application::Get().GetBasePath() + "AsteroidsAnimations.txt", spriteSheet);
	const auto& sprite = m_Sprite.GetSpriteSheet()->GetSprite("space_ship");
	m_BoundingCircle = Core::Circle(initialPos, std::roundf(sprite.width / 2.f) - 4);
}

void Ship::Update(uint32_t dt)
{
    if (m_Dead)
        return;

    if (m_Hit)
    {
        m_Sprite.Update(dt);
        if (m_Sprite.IsFinishedPlayingAnimation())
        {
            m_Hit = false;
            m_Dead = true;
        }
        return;
    }

    if (m_ShipYawMovement == AsteroidsShipYawMovement::ShipHeadingTurnLeft)
    {
        m_Yaw -= Core::MillisecondsToSeconds(dt) * ANGLE_PER_SECOND;
        if (m_Yaw < 0)
            m_Yaw += Core::TWO_PI;
    }
    else if (m_ShipYawMovement == AsteroidsShipYawMovement::ShipHeadingTurnRight)
    {
        m_Yaw += Core::MillisecondsToSeconds(dt) * ANGLE_PER_SECOND;
        if (m_Yaw > Core::TWO_PI)
            m_Yaw -= Core::TWO_PI;
    }

    if (m_UseLaserTimer > 0)
    {
        m_UseLaserTimer -= dt;
        if (m_UseLaserTimer <= 0)
            m_UseLaserTimer = 0;
    }

    if (ALWAYS_USE_LASER)
        m_CurrentAmmoType = AmmoType::Laser;
    else
        m_CurrentAmmoType = AmmoType::Missile;

    if (m_UseLaserTimer > 0)
        m_CurrentAmmoType = AmmoType::Laser;

    m_Scale = 1.f;

    if (ALWAYS_BIG)
        m_GrowState = GrowState::Stay;

    if (m_GrowState == GrowState::Grow)
    {
        m_ScaleTimer += dt;
        if (m_ScaleTimer >= GROW_TRANSITION_TIME)
        {
            m_ScaleTimer = GROW_TRANSITION_TIME;
            m_GrowState = GrowState::Stay;
        }

        float t = static_cast<float>(m_ScaleTimer) / static_cast<float>(GROW_TRANSITION_TIME);
        t = Ease::EaseInBounce(t);

        m_Scale = Core::Lerpf(1.f, GROW_SCALE, t);
        if (m_GrowState == GrowState::Stay)
            m_ScaleTimer = STAY_BIG_SIZE_TIME;
    }
    else if (m_GrowState == GrowState::Stay)
    {
        m_ScaleTimer -= dt;
        m_Scale = GROW_SCALE;
        if (m_ScaleTimer <= 0)
        {
            m_GrowState = GrowState::Regress;
            m_ScaleTimer = 0;
        }
    }
    else if (m_GrowState == GrowState::Regress)
    {
        m_ScaleTimer += dt;
        if (m_ScaleTimer >= GROW_TRANSITION_TIME)
        {
            m_ScaleTimer = GROW_TRANSITION_TIME;
            m_GrowState = GrowState::None;
        }

        float t = static_cast<float>(m_ScaleTimer) / static_cast<float>(GROW_TRANSITION_TIME);
        t = Ease::EaseOutBounce(t);

        m_Scale = Core::Lerpf(GROW_SCALE, 1.f, t);
        if (m_GrowState == GrowState::None)
            m_ScaleTimer = 0;
    }


    Core::Vec2D heading = Core::Vec2D(0, -1);
    heading = heading.Rotation(m_Yaw);
    heading.Normalize();

    m_BoundingCircle.MoveBy(m_Speed * heading * Core::MillisecondsToSeconds(dt));

    if (m_Speed > 0)
        m_Sprite.Update(dt);

    for (auto& ammo : m_Ammo)
        ammo->Update(dt);
}

void Ship::Draw(Core::Window& window)
{
    if (m_Dead)
        return;

    if (!m_Hit)
    {
        for (auto& ammo : m_Ammo)
            ammo->Draw(window);

        const auto& sprite = m_Sprite.GetSpriteSheet()->GetSprite("space_ship");

        Core::DrawTransform transform;
        transform.Pos = m_BoundingCircle.GetCenterPoint() - Core::Vec2D(std::round(static_cast<float>(sprite.width) / 2.f), std::round(static_cast<float>(sprite.height) / 2.f)) * m_Scale;
        transform.Scale = m_Scale;
        transform.RotationAngle = m_Yaw;

        Core::ColorParams colorParams;
        colorParams.Alpha = 1.0f;
        colorParams.BilinearFiltering = BILINEAR_FILTERING;
        colorParams.Gradient.XParam = Core::GradientXParam::NoXGradient;
        colorParams.Gradient.YParam = Core::GradientYParam::NoYGradient;
        colorParams.Overlay = Core::Color::White();

        Core::UVParams uvParams;
        window.Draw(*m_Sprite.GetSpriteSheet(), "space_ship", transform, colorParams, uvParams);

        if (m_Speed > 0)
        {
            Core::Vec2D heading = Core::Vec2D(0, -1);
            heading = heading.Rotation(m_Yaw);
            heading.Normalize();

            const auto& thrustersSprite = m_Sprite.GetSpriteSheet()->GetSprite("thrusters_1");
            m_Sprite.SetScale(m_Scale);
            m_Sprite.SetAngle(m_Yaw);
            m_Sprite.SetPosition((GetPosition() - (heading * ((static_cast<float>(sprite.height) * m_Scale) / 2.f + (static_cast<float>(thrustersSprite.height) * m_Scale) / 2.f + (BILINEAR_FILTERING ? 0 : -2 * m_Scale))) - Core::Vec2D((static_cast<float>(thrustersSprite.width) * m_Scale) / 2.f, (static_cast<float>(thrustersSprite.height) * m_Scale) / 2.f)));
            m_Sprite.Draw(window, BILINEAR_FILTERING);
        }
    }
    else
    {
        m_Sprite.SetPosition(m_BoundingCircle.GetCenterPoint() - Core::Vec2D(m_Sprite.GetBoundingBox().GetWidth() / 2, m_Sprite.GetBoundingBox().GetHeight() / 2));
        m_Sprite.Draw(window, BILINEAR_FILTERING);
    }
}

void Ship::UpdateYaw(AsteroidsShipYawMovement movement)
{
    m_ShipYawMovement = movement;
}

void Ship::EngageThrusters()
{
    if (!m_Hit && !ThrustersEngaged())
    {
        m_Speed = SHIP_SPEED;
        m_Sprite.SetAnimation("thrusters", true);
    }
}

void Ship::DisengageThrusters()
{
    m_Speed = 0;
}

void Ship::Fire()
{
    if (m_Ammo.size() < AMMO_CAPACITY)
    {
        Core::Vec2D heading = Core::Vec2D(0, -1);
        heading = heading.Rotation(m_Yaw);
        heading.Normalize();

        std::unique_ptr<ShipAmmo> newAmmo;
        if (m_CurrentAmmoType == AmmoType::Missile)
            newAmmo = std::make_unique<Missile>(*m_Sprite.GetSpriteSheet(), m_BoundingCircle.GetCenterPoint(), heading * MISSILE_SPEED, m_Yaw, m_Scale);
        else if (m_CurrentAmmoType == AmmoType::Laser)
            newAmmo = std::make_unique<Laser>(m_BoundingCircle.GetCenterPoint(), heading * LASER_SPEED, Core::Color::Yellow(), Core::Color::Red(), IsBig());

        newAmmo->SetID(m_AmmoIDGen++);
        m_Ammo.push_back(std::move(newAmmo));
    }
}

void Ship::RegainAmmo(size_t ammoInstanceID)
{
    auto it = std::remove_if(m_Ammo.begin(), m_Ammo.end(), [&](const auto& m)
    {
        return ammoInstanceID == m->GetID();
    });
    m_Ammo.erase(it);
}

void Ship::HitByAsteroid()
{
    if (!m_Hit && !m_Dead)
    {
        m_Hit = true;
        m_Sprite.SetAnimation("explosion", false);

        ResetGrowth();
        ResetAmmoType();
        m_Ammo.clear();
    }
}

void Ship::SetPosition(const Core::Vec2D& pos)
{
    m_BoundingCircle.MoveTo(pos);
}

Core::Circle Ship::GetBoundingCircle() const
{
    return Core::Circle(m_BoundingCircle.GetCenterPoint(), m_BoundingCircle.GetRadius() * m_Scale);
}

void Ship::Respawn(const Core::Vec2D& position)
{
    m_Dead = false;
    m_Ammo.clear();
    m_BoundingCircle.MoveTo(position);
    m_ShipYawMovement = AsteroidsShipYawMovement::ShipHeadingNone;
    m_Yaw = 0;
    m_Speed = 0;
    m_GrowState = GrowState::None;
    m_Scale = 1.f;
    m_ScaleTimer = 0;
    m_UseLaserTimer = 0;
}

size_t Ship::AmmoLeft() const
{
    return AMMO_CAPACITY - m_Ammo.size();
}

void Ship::SwitchToLaser()
{
    m_UseLaserTimer = LASER_USAGE_TIME;
}

void Ship::Grow()
{
    if (m_GrowState != GrowState::Stay)
    {
        m_GrowState = GrowState::Grow;
        m_ScaleTimer = 0;
    }
    else
        ExtendGrow();
}

bool Ship::IsBig()
{
    return m_GrowState != GrowState::None;
}

void Ship::ExtendGrow()
{
    if (m_GrowState == GrowState::Stay)
        m_ScaleTimer = STAY_BIG_SIZE_TIME;
}

void Ship::DrawDebug(Core::Window& window)
{
    Core::Vec2D heading = Core::Vec2D(0, -1);
    heading = heading.Rotation(m_Yaw);
    heading.Normalize();
    window.Draw(GetBoundingCircle(), Core::Color::White());
    window.Draw(Core::Line(GetPosition(), 12 * heading + GetPosition()), Core::Color::White());
}

void Ship::ResetGrowth()
{
    m_GrowState = GrowState::None;
    m_ScaleTimer = 0;
    m_Scale = 1.f;
}

void Ship::ResetAmmoType()
{
    m_UseLaserTimer = 0;
}
