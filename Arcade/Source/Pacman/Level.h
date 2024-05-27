#pragma once

#include "Excluder.h"

class Level
{
public:
	bool Init(const std::string& filePath);
	void Update(uint32_t dt);
	void Draw(Core::Window& window);

private:
	struct Tile
	{
		Core::Vec2D Position = Core::Vec2D::Zero;
		int Width = 0;
		int Collidable = 0;
		char Symbol = '-';
	};

	std::vector<Excluder> m_Walls;
	std::vector<Tile> m_Tiles;

	Core::Vec2D m_LayoutOffset;
	size_t m_TileHeight;

private:
	bool LoadLevel(const std::string& filePath);
	Tile* GetTileForSymbol(char symbol);

};
