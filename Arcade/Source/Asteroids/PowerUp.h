#pragma once

enum class PowerUpType
{
	Grow = 0,
	Laser,
	Num
};

class PowerUp
{
public:
	using PowerUpEffect = std::function<void(void)>;

	PowerUp();

	void Init(const Core::SpriteSheet& spriteSheet, const std::string& spriteName, const Core::Vec2D& initialPos, const Core::Vec2D& velocity, float lifeTime, PowerUpEffect effect);

	bool Update(uint32_t dt);
	void Draw(Core::Window& window);
	void ExecuteEffect();
	void SetPosition(const Core::Vec2D& newPosition);

	inline bool IsActive() const { return m_LifeTime > 0; }
	inline Core::Vec2D GetPosition() const { return m_BoundingCircle.GetCenterPoint(); }
	inline const float GetWidth() const { return m_BBox.GetWidth(); }
	inline const float GetHeight() const { return m_BBox.GetHeight(); }
	inline const Core::Circle& GetBoundingCircle() const { return m_BoundingCircle; }
	inline const Core::Rectangle& GetBoundingBox() const { return m_BBox; }

private:
	const Core::SpriteSheet* m_SpriteSheet;
	std::string m_SpriteName;
	Core::Rectangle m_BBox;
	Core::Circle m_BoundingCircle;
	Core::Vec2D m_Velocity;
	int m_LifeTime;

	PowerUpEffect m_Effect;
};