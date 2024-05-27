#include "ArcadePCH.h"
#include "Level.h"

bool Level::Init(const std::string& filePath)
{
	return LoadLevel(filePath);
}

void Level::Update(uint32_t dt)
{
}

void Level::Draw(Core::Window& window)
{
	for (const auto& wall : m_Walls)
		window.Draw(wall.GetRectangle(), Core::Color::Blue());
}

bool Level::LoadLevel(const std::string& filePath)
{
	Core::FileCommandLoader fileLoader;

	Core::Command tileWidthCommand;
	tileWidthCommand.CommandString = "tile_width";
	tileWidthCommand.ParseFunc = [this](Core::ParseFuncParams params)
	{
		m_Tiles.back().Width = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileWidthCommand);

	Core::Command tileHeightCommand;
	tileHeightCommand.CommandString = "tile_height";
	tileHeightCommand.ParseFunc = [this](Core::ParseFuncParams params)
	{
		m_TileHeight = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileHeightCommand);

	Core::Command tileCommand;
	tileCommand.CommandString = "tile";
	tileCommand.ParseFunc = [this](Core::ParseFuncParams params)
	{
		m_Tiles.push_back(Tile());
	};
	fileLoader.AddCommand(tileCommand);

	Core::Command tileSymbolCommand;
	tileSymbolCommand.CommandString = "tile_symbol";
	tileSymbolCommand.ParseFunc = [this](Core::ParseFuncParams params)
	{
		m_Tiles.back().Symbol = Core::FileCommandLoader::ReadChar(params);
	};
	fileLoader.AddCommand(tileSymbolCommand);

	Core::Command tileCollisionCommand;
	tileCollisionCommand.CommandString = "tile_collision";
	tileCollisionCommand.ParseFunc = [this](Core::ParseFuncParams params)
	{
		m_Tiles.back().Collidable = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileCollisionCommand);

	Core::Vec2D layoutOffset;

	Core::Command layoutOffsetCommand;
	layoutOffsetCommand.CommandString = "layout_offset";
	layoutOffsetCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_LayoutOffset = Core::FileCommandLoader::ReadSize(params);
		layoutOffset = m_LayoutOffset;
	};
	fileLoader.AddCommand(layoutOffsetCommand);

	Core::Command layoutCommand;
	layoutCommand.CommandString = "layout";
	layoutCommand.CommandType = Core::CommandType::MultiLine;
	layoutCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		int startingX = layoutOffset.GetX();
		for (int c = 0; c < params.Line.length(); ++c)
		{
			if (Tile* tile = GetTileForSymbol(params.Line[c]))
			{
				tile->Position = Core::Vec2D(startingX, layoutOffset.GetY());
				if (tile->Collidable > 0)
				{
					Excluder wall;
					wall.Init(Core::Rectangle(Core::Vec2D(startingX, layoutOffset.GetY()), tile->Width, static_cast<int>(m_TileHeight)));
					m_Walls.push_back(wall);
				}
				startingX += tile->Width;
			}
		}
		layoutOffset += Core::Vec2D(0, m_TileHeight);
	};
	fileLoader.AddCommand(layoutCommand);

	return fileLoader.LoadFile(filePath);
}

Level::Tile* Level::GetTileForSymbol(char symbol)
{
	for (size_t i = 0; i < m_Tiles.size(); ++i)
	{
		if (m_Tiles[i].Symbol == symbol)
			return &m_Tiles[i];
	}
	return nullptr;
}
