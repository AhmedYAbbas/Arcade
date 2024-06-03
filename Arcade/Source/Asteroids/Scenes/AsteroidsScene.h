#pragma once

#include <random>

#include "Asteroids/Ship.h"
#include "Asteroids/Asteroid.h"
#include "Asteroids/PowerUp.h"

enum class AsteroidsGameState
{
	LevelStarting = 0,
	PlayGame,
	GameOver
};

class AsteroidsScene : public Core::Scene
{
public:
	AsteroidsScene();

	virtual bool Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Core::Window& window) override;
	virtual const std::string& GetSceneName() const override;

private:
	void SetupAsteroids();
	void AddRandomAsteroid();
	void RemoveDestroyedAsteroids(uint32_t dt);

	Core::Vec2D GetSpawnLocation();
	Core::Vec2D GetSpawnVelocity(const Core::Vec2D& position);
	bool AmmoOutOfBounds(const std::unique_ptr<ShipAmmo>& ammo);
	std::vector<Asteroid> BreakAsteroid(const Core::Vec2D& position, const Core::Vec2D& velocity, const Asteroid& asteroid, float ammoScale);

	void AddAsteroid(std::vector<Asteroid>& asteroids, const Core::Vec2D& spawnLocation, const Core::Vec2D& velocity, AsteroidSize size);
	Core::Vec2D WrapAround(const Core::Vec2D& objectPosition, const Core::Vec2D& objectSize);
	void ResetLevel();
	void ResetGame();
	int ScoreValue(AsteroidSize asteroidSizeHit);
	void PlayGame(uint32_t dt);

	PowerUp MakePowerUp(PowerUpType type, const Core::Vec2D& spawnLocation, const Core::Vec2D& velocity);
	void SpawnPowerUp(const Core::Vec2D& spawnLocation, const Core::Vec2D& velocity);
	void MaybeSpawnPowerUp(const Core::Vec2D& spawnLocation);

	void DrawScore(Core::Window& window, float windowWidth, float windowHeight, const Core::BitmapFont& font, Core::Vec2D& textDrawPosition, Core::DrawTransform& transform, Core::ColorParams& colorParams, Core::UVParams& uvParams);

private:
	AsteroidsGameState m_GameState;
	Core::BMPImage m_BGImage;
	Ship m_Ship;
	Core::SpriteSheet m_AsteroidsSprites;
	std::vector<Asteroid> m_Asteroids;
	std::vector<PowerUp> m_PowerUps;
	std::default_random_engine m_RandomGenerator;
	Core::Rectangle m_StringRect;
	int m_Lives;
	int m_Score;
	int m_LevelStartTimer;
};