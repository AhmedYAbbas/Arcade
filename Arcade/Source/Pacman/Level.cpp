#include "ArcadePCH.h"
#include "Level.h"
#include "Pacman.h"
#include "Ghost.h"

static const uint32_t NUM_LEVELS = 256;
static const uint32_t SPRITE_HEIGHT = 16;
static const uint32_t SPRITE_WIDTH = 16;

bool Level::Init(const std::string& filePath, const Core::SpriteSheet& spriteSheet)
{
	m_CurrentLevel = 0;
	m_SpriteSheet = spriteSheet;
	m_BonusItemSpriteName = "";
	std::random_device r;
	m_Generator.seed(r());
	m_GhostsSpawnPoints.resize(static_cast<int>(GhostName::Num));

	bool levelLoaded = LoadLevel(filePath);
	if (levelLoaded)
		ResetLevel();

	return levelLoaded;
}

void Level::Update(uint32_t dt, Pacman& pacman, std::vector<Ghost>& ghosts, std::vector<GhostAI>& ghostAIs)
{
	for (const auto& wall : m_Walls)
	{
		Core::BoundaryEdge edge;
		if (wall.HasCollided(pacman.GetBoundingBox(), edge))
		{
			Core::Vec2D offset = wall.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			pacman.Stop();
		}

		for (auto& ghost : ghosts)
		{
			if (wall.HasCollided(ghost.GetBoundingBox(), edge))
			{
				Core::Vec2D offset = wall.GetCollisionOffset(ghost.GetBoundingBox());
				ghost.MoveBy(offset);
				ghost.Stop();
			}
		}
	}

	for (const auto& gate : m_Gates)
	{
		Core::BoundaryEdge edge;
		if (gate.HasCollided(pacman.GetBoundingBox(), edge))
		{
			Core::Vec2D offset = gate.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			pacman.Stop();
		}

		for (size_t i = 0; i < ghosts.size(); ++i)
		{
			GhostAI& ghostAI = ghostAIs[i];
			Ghost& ghost = ghosts[i];

			if (!(ghostAI.WantsToLeavePen() || ghostAI.IsEnteringPen()) && gate.HasCollided(ghost.GetBoundingBox(), edge))
			{
				Core::Vec2D offset = gate.GetCollisionOffset(ghost.GetBoundingBox());
				ghost.MoveBy(offset);
				ghost.Stop();
			}
		}
	}

	for (const Tile& t : m_Tiles)
	{
		if (t.IsTeleportTile)
		{
			Core::Rectangle teleportTileAABB(t.Position, t.Width, static_cast<float>(m_TileHeight));
			Tile* teleportToTile = GetTileForSymbol(t.TeleportToSymbol);
			assert(teleportToTile);
			if (teleportToTile->IsTeleportTile)
			{
				if (teleportTileAABB.Intersects(pacman.GetBoundingBox()))
					pacman.MoveTo(teleportToTile->Position + teleportToTile->Offset);

				for (auto& ghost : ghosts)
				{
					if (teleportTileAABB.Intersects(ghost.GetBoundingBox()))
						ghost.MoveTo(teleportToTile->Position + teleportToTile->Offset);
				}
			}
		}
	}

	for (auto& pellet : m_Pellets)
	{
		if (!pellet.Eaten)
		{
			if (pacman.GetEatingBoundingBox().Intersects(pellet.BoundingBox))
			{
				pellet.Eaten = true;
				pacman.AteItem(pellet.Score);
				if (pellet.PowerPellet)
				{
					pacman.ResetGhostEatenMultiplier();
					for (auto& ghost : ghosts)
						ghost.SetStateToVulnerable();
				}
			}
		}
	}

	if (ShouldSpawnBonusItem())
		SpawnBonusItem();

	if (m_BonusItem.Spawned && !m_BonusItem.Eaten)
	{
		if (pacman.GetEatingBoundingBox().Intersects(m_BonusItem.BoundingBox))
		{
			m_BonusItem.Eaten = true;
			pacman.AteItem(m_BonusItem.Score);
		}
	}
}

void Level::Draw(Core::Window& window)
{
	Core::Sprite bgSprite;
	bgSprite.width = m_BGImage.GetWidth();
	bgSprite.height = m_BGImage.GetHeight();
	window.Draw(m_BGImage, bgSprite, Core::Vec2D::Zero);

	for (const auto& pellet : m_Pellets)
	{
		if (!pellet.Eaten)
		{
			if (!pellet.PowerPellet)
				window.Draw(pellet.BoundingBox, Core::Color::White());
			else
			{
				Core::Circle c(pellet.BoundingBox.GetCenterPoint(), pellet.BoundingBox.GetWidth() / 2.f);
				window.Draw(c, Core::Color::White(), true);
			}
		}
	}

	if (m_BonusItem.Spawned && !m_BonusItem.Eaten)
		window.Draw(m_SpriteSheet, m_BonusItemSpriteName, m_BonusItem.BoundingBox.GetTopLeftPoint());
}

bool Level::WillCollide(const Core::Rectangle& boundingBox, PacmanMovement direction) const
{
	Core::Rectangle box = boundingBox;
	box.MoveBy(GetMovementVector(direction));
	Core::BoundaryEdge edge;
	for (const auto& wall : m_Walls)
	{
		if (wall.HasCollided(box, edge))
			return true;
	}

	for (const auto& gate : m_Gates)
	{
		if (gate.HasCollided(box, edge))
			return true;
	}

	return false;
}

bool Level::WillCollide(const Ghost& ghost, const GhostAI& ghostAI, PacmanMovement direction) const
{
	Core::Rectangle bbox(ghost.GetBoundingBox());
	bbox.MoveBy(GetMovementVector(direction));

	Core::BoundaryEdge edge;
	for (const auto& wall : m_Walls)
	{
		if (wall.HasCollided(bbox, edge))
			return true;
	}

	for (const auto& gate : m_Gates)
	{
		if (!(ghostAI.IsEnteringPen() || ghostAI.WantsToLeavePen()) && gate.HasCollided(bbox, edge))
			return true;
	}

	return false;
}

bool Level::IsLevelOver() const
{
	return HasEatenAllPellets();
}

void Level::IncreaseLevel()
{
	++m_CurrentLevel;
	if (m_CurrentLevel > NUM_LEVELS)
		m_CurrentLevel = 1;

	ResetLevel();
}

void Level::ResetToFirstLevel()
{
	m_CurrentLevel = 1;
	ResetLevel();
}

bool Level::LoadLevel(const std::string& filePath)
{
	Core::FileCommandLoader fileLoader;

	std::string bgImageName;

	Core::Command bgImageCommand;
	bgImageCommand.CommandString = "bg_image";
	bgImageCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		bgImageName = Core::FileCommandLoader::ReadString(params);
		bool loaded = m_BGImage.Load(Core::Application::Get().GetBasePath() + bgImageName);

		assert(loaded && "Didn't  load the BG Image");
	};
	fileLoader.AddCommand(bgImageCommand);

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

	Core::Command tileIsTeleportTileCommand;
	tileIsTeleportTileCommand.CommandString = "tile_is_teleport_tile";
	tileIsTeleportTileCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().IsTeleportTile = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileIsTeleportTileCommand);

	Core::Command tileToTeleportToCommand;
	tileToTeleportToCommand.CommandString = "tile_teleport_to_symbol";
	tileToTeleportToCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().TeleportToSymbol = Core::FileCommandLoader::ReadChar(params);
	};
	fileLoader.AddCommand(tileToTeleportToCommand);

	Core::Command tileOffsetCommand;
	tileOffsetCommand.CommandString = "tile_offset";
	tileOffsetCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().Offset = Core::FileCommandLoader::ReadSize(params);
	};
	fileLoader.AddCommand(tileOffsetCommand);

	Core::Command tileExcludePelletCommand;
	tileExcludePelletCommand.CommandString = "tile_exclude_pellet";
	tileExcludePelletCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().ExcludePelletTile = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileExcludePelletCommand);

	Core::Command tilePacmanSpawnPointCommand;
	tilePacmanSpawnPointCommand.CommandString = "tile_pacman_spawn_point";
	tilePacmanSpawnPointCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().PacmanSpawnPoint = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tilePacmanSpawnPointCommand);

	Core::Command tileItemSpawnPointCommand;
	tileItemSpawnPointCommand.CommandString = "tile_item_spawn_point";
	tileItemSpawnPointCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().ItemSpawnPoint = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileItemSpawnPointCommand);

	Core::Command tileBlinkySpawnPointCommand;
	tileBlinkySpawnPointCommand.CommandString = "tile_blinky_spawn_point";
	tileBlinkySpawnPointCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().BlinkySpawnPoint = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileBlinkySpawnPointCommand);

	Core::Command tileInkySpawnPointCommand;
	tileInkySpawnPointCommand.CommandString = "tile_inky_spawn_point";
	tileInkySpawnPointCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().InkySpawnPoint = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileInkySpawnPointCommand);

	Core::Command tilePinkySpawnPointCommand;
	tilePinkySpawnPointCommand.CommandString = "tile_pinky_spawn_point";
	tilePinkySpawnPointCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().PinkySpawnPoint = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tilePinkySpawnPointCommand);

	Core::Command tileClydeSpawnPointCommand;
	tileClydeSpawnPointCommand.CommandString = "tile_clyde_spawn_point";
	tileClydeSpawnPointCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().ClydeSpawnPoint = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileClydeSpawnPointCommand);

	Core::Command tileGateCommand;
	tileGateCommand.CommandString = "tile_is_gate";
	tileGateCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_Tiles.back().IsGate = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileGateCommand);

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

				if (tile->IsGate > 0)
				{
					Excluder gate;
					gate.Init(Core::Rectangle(Core::Vec2D(startingX, layoutOffset.GetY()), tile->Width, static_cast<int>(m_TileHeight)));
					m_Gates.push_back(gate);
				}
				else if (tile->Collidable > 0)
				{
					Excluder wall;
					wall.Init(Core::Rectangle(Core::Vec2D(startingX, layoutOffset.GetY()), tile->Width, static_cast<int>(m_TileHeight)));
					m_Walls.push_back(wall);
				}

				if (tile->PacmanSpawnPoint > 0)
					m_PacmanSpawnLocation = Core::Vec2D(startingX + tile->Offset.GetX(), layoutOffset.GetY() + tile->Offset.GetY());
				else if (tile->ItemSpawnPoint)
					m_BonusItem.BoundingBox = Core::Rectangle(Core::Vec2D(startingX + tile->Offset.GetX(), layoutOffset.GetY() + tile->Offset.GetY()), SPRITE_WIDTH, SPRITE_HEIGHT);
				else if (tile->BlinkySpawnPoint > 0)
					m_GhostsSpawnPoints[static_cast<int>(GhostName::Blinky)] = Core::Vec2D(startingX + tile->Offset.GetX() + 1, layoutOffset.GetY() + tile->Offset.GetY());
				else if (tile->InkySpawnPoint > 0)
					m_GhostsSpawnPoints[static_cast<int>(GhostName::Inky)] = Core::Vec2D(startingX + tile->Offset.GetX() + 1, layoutOffset.GetY() + tile->Offset.GetY());
				else if (tile->PinkySpawnPoint > 0)
					m_GhostsSpawnPoints[static_cast<int>(GhostName::Pinky)] = Core::Vec2D(startingX + tile->Offset.GetX(), layoutOffset.GetY() + tile->Offset.GetY());
				else if (tile->ClydeSpawnPoint > 0)
					m_GhostsSpawnPoints[static_cast<int>(GhostName::Clyde)] = Core::Vec2D(startingX + tile->Offset.GetX(), layoutOffset.GetY() + tile->Offset.GetY());

				if (tile->ExcludePelletTile > 0)
					m_ExclusionTiles.push_back(*tile);

				startingX += tile->Width;
			}
		}
		layoutOffset += Core::Vec2D(0, m_TileHeight);
	};
	fileLoader.AddCommand(layoutCommand);

	Core::Command bounsItemCommand;
	bounsItemCommand.CommandString = "bonus_item";
	bounsItemCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		BonusItemLevelProperties newProperty;
		m_BonusItemProperties.push_back(newProperty);
	};
	fileLoader.AddCommand(bounsItemCommand);

	Core::Command bounsItemSpriteNameCommand;
	bounsItemSpriteNameCommand.CommandString = "bonus_item_sprite_name";
	bounsItemSpriteNameCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_BonusItemProperties.back().SpriteName = Core::FileCommandLoader::ReadString(params);
	};
	fileLoader.AddCommand(bounsItemSpriteNameCommand);

	Core::Command bounsItemScoreCommand;
	bounsItemScoreCommand.CommandString = "bonus_item_score";
	bounsItemScoreCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_BonusItemProperties.back().Score = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(bounsItemScoreCommand);

	Core::Command bounsItemBeginLevelCommand;
	bounsItemBeginLevelCommand.CommandString = "bonus_item_begin_level";
	bounsItemBeginLevelCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_BonusItemProperties.back().Begin = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(bounsItemBeginLevelCommand);

	Core::Command bounsItemEndLevelCommand;
	bounsItemEndLevelCommand.CommandString = "bonus_item_end_level";
	bounsItemEndLevelCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		m_BonusItemProperties.back().End = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(bounsItemEndLevelCommand);

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

void Level::ResetPellets()
{
	m_Pellets.clear();

	const uint32_t PELLET_SIZE = 2;
	const uint32_t PADDING = static_cast<uint32_t>(m_TileHeight);

	uint32_t startingY = m_LayoutOffset.GetY() + PADDING + m_TileHeight - 1;
	uint32_t startingX = PADDING + 3;

	const uint32_t LEVEL_HEIGHT = m_LayoutOffset.GetY() + 32 * m_TileHeight;

	Pellet p;
	p.Score = 10;

	uint32_t row = 0;
	for (uint32_t y = startingY; y < LEVEL_HEIGHT; y += PADDING, ++row)
	{
		for (uint32_t x = startingX, col = 0; x < Core::Application::Get().GetWindow().GetWidth(); x += PADDING, ++col)
		{
			if (row == 0 || row == 22)
			{
				if (col == 0 || col == 25)
				{
					p.PowerPellet = 1;
					p.Score = 50;
					p.BoundingBox = Core::Rectangle(Core::Vec2D(x - 3, y - 3), m_TileHeight, m_TileHeight);
					m_Pellets.push_back(p);

					p.PowerPellet = 0;
					p.Score = 10;

					continue;
				}
			}

			Core::Rectangle rect(Core::Vec2D(x, y), PELLET_SIZE, PELLET_SIZE);
			bool found = false;
			for (const Excluder& wall : m_Walls)
			{
				if (wall.GetRectangle().Intersects(rect))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				for (const Tile& excludedPelletTile : m_ExclusionTiles)
				{
					if (excludedPelletTile.ExcludePelletTile)
					{
						Core::Rectangle tileAABB(excludedPelletTile.Position, excludedPelletTile.Width, m_TileHeight);
						if (tileAABB.Intersects(rect))
						{
							found = true;
							break;
						}
					}
				}
			}

			if (!found)
			{
				p.BoundingBox = rect;
				m_Pellets.push_back(p);
			}
		}
	}
}

bool Level::HasEatenAllPellets() const
{
	return NumPelletsEaten() >= m_Pellets.size() - 4;
}

size_t Level::NumPelletsEaten() const
{
	size_t numEaten = 0;
	for (const auto& pellet : m_Pellets)
	{
		if (!pellet.PowerPellet && pellet.Eaten)
			++numEaten;
	}
	return numEaten;
}

void Level::GetBonusItemSpriteName(std::string& spriteName, uint32_t& score) const
{
	for (const auto& properties : m_BonusItemProperties)
	{
		if (m_CurrentLevel >= properties.Begin && m_CurrentLevel <= properties.End)
		{
			spriteName = properties.SpriteName;
			score = properties.Score;
			return;
		}
	}
}

void Level::SpawnBonusItem()
{
	m_BonusItem.Spawned = 1;
	m_BonusItem.Eaten = 0;
}

bool Level::ShouldSpawnBonusItem() const
{
	auto numEaten = NumPelletsEaten();
	return !m_BonusItem.Spawned && numEaten >= m_BonusItem.SpawnTime;
}

void Level::ResetLevel()
{
	ResetPellets();

	std::uniform_int_distribution<size_t> distribution(20, m_Pellets.size() - 50);
	m_BonusItem.SpawnTime = static_cast<int>(distribution(m_Generator));

	GetBonusItemSpriteName(m_BonusItemSpriteName, m_BonusItem.Score);
}
