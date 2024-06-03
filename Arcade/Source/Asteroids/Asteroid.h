#pragma once

enum class AsteroidSize
{
	None = 0,
	Large,
	Medium,
	Small,
	Num
};

class Asteroid
{
public:
	Asteroid();

	void Init(const Core::SpriteSheet& spriteSheet, const std::string& spriteName, AsteroidSize size, const Core::Vec2D& initialPos, const Core::Vec2D& velocity, float rotationRate);
	void Update(uint32_t dt);
	void Draw(Core::Window& window);

	inline void SetPosition(const Core::Vec2D& position) { m_BoundingCircle.MoveTo(position); }
	inline void SetVelocity(const Core::Vec2D& vel) { m_Velocity = vel; }

	inline const Core::Circle& GetBoundingCircle() const { return m_BoundingCircle; }
	inline Core::Vec2D GetPosition() const { return m_BoundingCircle.GetCenterPoint(); }
	inline const Core::Vec2D& GetVelocity() const { return m_Velocity; }
	inline AsteroidSize GetSize() const { return m_Size; }
	inline bool IsExploding() const { return m_Hit; }
	inline bool IsDestroyed() const { return m_Destroyed; }

	bool IsOutOfBounds(const Core::Vec2D& largestSprite) const;
	void HitByAmmo(const std::vector<Core::Vec2D>& intersections);

private:
	void DrawDebug(Core::Window& window);
	void DrawAsteroidSplit(Core::Window& window, float alpha);

private:
	Core::AnimatedSprite m_Sprite;
	std::string m_SpriteName;
	Core::Circle m_BoundingCircle;
	Core::Vec2D m_Velocity;
	Core::Vec2D m_SpawnPosition;
	AsteroidSize m_Size;
	float m_RotationRate;
	float m_Rotation;
	bool m_Hit;
	bool m_Destroyed;

	bool m_Split;
	Core::Vec2D m_SplitPoint1;
	Core::Vec2D m_SplitPoint2;
	uint32_t m_SplitTime;
	Core::Vec2D m_PosAtSplitTime;
	float m_RotationAtSplit;
};