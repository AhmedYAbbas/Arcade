#pragma once

#include "ShipAmmo.h"

enum class AsteroidsShipYawMovement
{
	ShipHeadingNone = 0,
	ShipHeadingTurnLeft,
	ShipHeadingTurnRight
};

class Ship
{
public:
	Ship();

	void Init(const Core::SpriteSheet& spriteSheet, const Core::Vec2D& initialPos);
	void Update(uint32_t dt);
	void Draw(Core::Window& window);

	void UpdateYaw(AsteroidsShipYawMovement movement);
	void EngageThrusters();
	void DisengageThrusters();

	void Fire();
	void RegainAmmo(size_t ammoInstanceID);
	void HitByAsteroid();


	void SetPosition(const Core::Vec2D& pos);
	Core::Circle GetBoundingCircle()const;

	void Respawn(const Core::Vec2D& position);
	size_t AmmoLeft() const;

	void SwitchToLaser();
	void Grow();
	bool IsBig();
	void ExtendGrow();

	inline float GetWidth() const { return m_BoundingCircle.GetRadius() * 2; }
	inline float GetHeight() const { return GetWidth(); }
	inline bool ThrustersEngaged() const { return m_Speed > 0; }
	inline Core::Vec2D GetPosition() const { return m_BoundingCircle.GetCenterPoint(); }
	inline const std::vector<std::unique_ptr<ShipAmmo>>& GetAmmoInFlight() const { return m_Ammo; }
	inline bool IsDestroyed() const { return m_Dead; }

private:
	void DrawDebug(Core::Window& window);

	void ResetGrowth();
	void ResetAmmoType();

private:
	enum class GrowState
	{
		None = 0,
		Grow,
		Stay,
		Regress
	};

	AsteroidsShipYawMovement m_ShipYawMovement;
	float m_Yaw;
	Core::AnimatedSprite m_Sprite;
	std::vector<std::unique_ptr<ShipAmmo>> m_Ammo;
	float m_Speed;
	Core::Circle m_BoundingCircle;

	static const size_t AMMO_CAPACITY = 10;
	size_t m_AmmoIDGen;
	bool m_Hit;
	bool m_Dead;
	AmmoType m_CurrentAmmoType;

	float m_Scale;
	GrowState m_GrowState;
	int m_ScaleTimer;
	int m_UseLaserTimer;
};