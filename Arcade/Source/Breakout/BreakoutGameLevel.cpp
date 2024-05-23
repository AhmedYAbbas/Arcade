#include "BreakoutGameLevel.h"
#include "Ball.h"  

static const int BLOCK_WIDTH = 16;
static const int BLOCK_HEIGHT = 8;


BreakoutGameLevel::BreakoutGameLevel()
{
}

void BreakoutGameLevel::Init(const Core::Rectangle& boundary)
{
	CreateDefaultLevel(boundary);
}

void BreakoutGameLevel::Load(const std::vector<Block>& blocks)
{
	m_Blocks.clear();
	m_Blocks = blocks;
}

void BreakoutGameLevel::Update(uint32_t dt, Ball& ball)
{
	std::vector<Block> collidedBlocks;
	Core::BoundaryEdge edgeToBounceOffOf;
	Block* blockToBounceOffOf = nullptr;
	float largestOffset = -1.f;
	for (auto& block : m_Blocks)
	{
		Core::BoundaryEdge edge;
		if (!block.IsDestroyed() && block.HasCollided(ball.GetBoundingBox(), edge))
		{
			collidedBlocks.push_back(block);
			float mag = block.GetCollisionOffset(ball.GetBoundingBox()).Mag();
			if (mag > largestOffset)
			{
				//largestOffset = mag;
				edgeToBounceOffOf = edge;
				blockToBounceOffOf = &block;
			}
		}
	}

	if (blockToBounceOffOf)
	{
		blockToBounceOffOf->Bounce(ball, edgeToBounceOffOf);
		blockToBounceOffOf->ReduceHP();
	}

	for (const auto& block : collidedBlocks)
	{
		Core::BoundaryEdge edge;
		if (block.HasCollided(ball.GetBoundingBox(), edge))
		{
			Core::Vec2D p;
			ball.MakeFlushWithEdge(edge, p, true);
		}
	}

}

void BreakoutGameLevel::Draw(Core::Window& window)
{
	for (Block& block : m_Blocks)
	{
		if (!block.IsDestroyed())
			block.Draw(window);
	}
}

struct LayoutBlock
{
	char Symbol = '-';
	int HP = 0;
	Core::Color Color = Core::Color::Black();
};

static LayoutBlock FindLayoutBlockForSymbol(const std::vector<LayoutBlock>& blocks, char symbol)
{
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		if (blocks[i].Symbol == symbol)
			return blocks[i];
	}
	return LayoutBlock();
}

std::vector<BreakoutGameLevel> BreakoutGameLevel::LoadLevelsFromFile(const std::string& filePath)
{
	std::vector<BreakoutGameLevel> levels;
	std::vector<LayoutBlock> layoutBlocks;
	std::vector<Block> levelBlocks;
	int width = 0;
	int height = 0;

	Core::FileCommandLoader fileLoader;
	Core::Command levelCommand;
	levelCommand.CommandString = "level";
	levelCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		if (!levels.empty())
			levels.back().Load(levelBlocks);

		layoutBlocks.clear();
		levelBlocks.clear();
		width = 0;
		height = 0;

		BreakoutGameLevel level;
		level.Init(Core::Rectangle(Core::Vec2D::Zero, Core::Application::Get().GetWindow().GetWidth(), Core::Application::Get().GetWindow().GetHeight()));
		levels.push_back(level);
	};
	fileLoader.AddCommand(levelCommand);

	Core::Command blockCommand;
	blockCommand.CommandString = "block";
	blockCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		LayoutBlock lb;
		layoutBlocks.push_back(lb);
	};
	fileLoader.AddCommand(blockCommand);

	Core::Command symbolCommand;
	symbolCommand.CommandString = "symbol";
	symbolCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		layoutBlocks.back().Symbol = Core::FileCommandLoader::ReadChar(params);
	};
	fileLoader.AddCommand(symbolCommand);

	Core::Command fillColorCommand;
	fillColorCommand.CommandString = "fillcolor";
	fillColorCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		layoutBlocks.back().Color = Core::FileCommandLoader::ReadColor(params);
	};
	fileLoader.AddCommand(fillColorCommand);

	Core::Command hpCommand;
	hpCommand.CommandString = "hp";
	hpCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		layoutBlocks.back().HP = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(hpCommand);

	Core::Command widthCommand;
	widthCommand.CommandString = "width";
	widthCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		width = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(widthCommand);

	Core::Command heightCommand;
	heightCommand.CommandString = "height";
	heightCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		height = Core::FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(heightCommand);

	Core::Command layoutCommand;
	layoutCommand.CommandType = Core::CommandType::MultiLine;
	layoutCommand.CommandString = "layout";
	layoutCommand.ParseFunc = [&](Core::ParseFuncParams params)
	{
		int blockWidth = BLOCK_WIDTH;
		int screenWidth = Core::Application::Get().GetWindow().GetWidth();

		float startingX = 0;
		Core::Rectangle blockRect(Core::Vec2D(startingX, (params.LineNum + 1) * BLOCK_HEIGHT), BLOCK_WIDTH, BLOCK_HEIGHT);
		for (int c = 0; c < params.Line.length(); ++c)
		{
			if (params.Line[c] != '-')
			{
				LayoutBlock layoutBlock = FindLayoutBlockForSymbol(layoutBlocks, params.Line[c]);

				Block b;
				b.Init(blockRect, layoutBlock.HP, Core::Color::Black(), layoutBlock.Color);
				levelBlocks.push_back(b);
			}
			blockRect.MoveBy(Core::Vec2D(BLOCK_WIDTH, 0));
		}
	};
	fileLoader.AddCommand(layoutCommand);

	fileLoader.LoadFile(filePath);

	if (!levels.empty())
		levels.back().Load(levelBlocks);

	return levels;
}

void BreakoutGameLevel::CreateDefaultLevel(const Core::Rectangle& boundary)
{
	m_Blocks.clear();

	static const int NUM_BLOCK_ACROSS = ((int)boundary.GetWidth() - (2 * (BLOCK_WIDTH)) / BLOCK_WIDTH);
	static const int NUM_BLOCK_ROWS = 5;

	float startX = ((int)boundary.GetWidth() - (NUM_BLOCK_ACROSS * (BLOCK_WIDTH + 1))) / 2;

	Core::Color colors[NUM_BLOCK_ROWS];
	colors[0] = Core::Color::Red();
	colors[1] = Core::Color::Magenta();
	colors[2] = Core::Color::Yellow();
	colors[3] = Core::Color::Green();
	colors[4] = Core::Color::Cyan();

	for (int r = 0; r < NUM_BLOCK_ROWS; ++r)
	{
		Core::Rectangle blockRect(Core::Vec2D(startX, BLOCK_HEIGHT * (r + 1)), BLOCK_WIDTH, BLOCK_HEIGHT);
		for (int c = 0; c < NUM_BLOCK_ACROSS; ++c)
		{
			Block b;
			b.Init(blockRect, 1, Core::Color::Black(), colors[r]);
			m_Blocks.push_back(b);
			blockRect.MoveBy(Core::Vec2D(BLOCK_WIDTH, 0));
		}
	}
}
