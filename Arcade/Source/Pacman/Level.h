#pragma once

#include <random>

#include "Excluder.h"
#include "GameUtils.h"
#include "GhostAI.h"

class Pacman;
class Ghost;

class Level
{
public:
	bool Init(const std::string& filePath, const Core::SpriteSheet& spriteSheet);
	void Update(uint32_t dt, Pacman& pacman, std::vector<Ghost>& ghosts, std::vector<GhostAI>& ghostAIs);
	void Draw(Core::Window& window);

	void ResetLevel();
	bool WillCollide(const Core::Rectangle& boundingBox, PacmanMovement direction) const;
	bool WillCollide(const Ghost& ghost, const GhostAI& ghostAI, PacmanMovement direction) const;
	bool IsLevelOver() const;
	void IncreaseLevel();
	void ResetToFirstLevel();

	inline Core::Vec2D GetLayoutOffset() const { return m_LayoutOffset; }
	inline Core::Vec2D GetPacmanSpawnLocation() const { return m_PacmanSpawnLocation; }
	inline const std::vector<Core::Vec2D>& GetGhostSpwanPoints() const { return m_GhostsSpawnPoints; }
	inline uint32_t GetInGameTextYPos() const { return m_BonusItem.BoundingBox.GetTopLeftPoint().GetY(); }

private:
	struct Tile
	{
		Core::Vec2D Position = Core::Vec2D::Zero;
		Core::Vec2D Offset = Core::Vec2D::Zero;
		int Width = 0;
		int Collidable = 0;
		int IsTeleportTile = 0;
		int ExcludePelletTile = 0;
		int PacmanSpawnPoint = 0;
		int ItemSpawnPoint = 0;
		int BlinkySpawnPoint = 0;
		int InkySpawnPoint = 0;
		int PinkySpawnPoint = 0;
		int ClydeSpawnPoint = 0;
		int IsGate = 0;
		char TeleportToSymbol = 0;
		char Symbol = '-';
	};

	struct Pellet
	{
		uint32_t Score = 0;
		Core::Rectangle BoundingBox;
		int PowerPellet = 0;
		bool Eaten = false;
	};

	struct BonusItem
	{
		uint32_t Score = 0;
		Core::Rectangle BoundingBox;
		int Eaten = 0;
		int Spawned = 0;
		int SpawnTime = -1;
	};

	struct BonusItemLevelProperties
	{
		uint32_t Score = 0;
		std::string SpriteName = "";
		uint32_t Begin = 0;
		uint32_t End = 0;
	};

private:
	bool LoadLevel(const std::string& filePath);
	Tile* GetTileForSymbol(char symbol);
	void ResetPellets();
	bool HasEatenAllPellets() const;
	size_t NumPelletsEaten() const;
	void GetBonusItemSpriteName(std::string& spriteName, uint32_t& score) const;
	void SpawnBonusItem();
	bool ShouldSpawnBonusItem() const;

private:
	Core::BMPImage m_BGImage;

	std::default_random_engine m_Generator;
	BonusItem m_BonusItem;
	std::string m_BonusItemSpriteName;
	Core::SpriteSheet m_SpriteSheet;
	std::vector<BonusItemLevelProperties> m_BonusItemProperties;

	std::vector<Excluder> m_Walls;
	std::vector<Tile> m_Tiles;

	std::vector<Tile> m_ExclusionTiles;
	std::vector<Pellet> m_Pellets;

	std::vector<Excluder> m_Gates;

	Core::Vec2D m_LayoutOffset;
	Core::Vec2D m_PacmanSpawnLocation;
	size_t m_TileHeight;
	int m_CurrentLevel;

	std::vector<Core::Vec2D> m_GhostsSpawnPoints;
};
