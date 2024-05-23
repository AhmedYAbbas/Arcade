#pragma once

#include <Core.h>
#include <vector>

#include "Block.h"

class BreakoutGameLevel
{
public:
	BreakoutGameLevel();

	void Init(const Core::Rectangle& boundary);
	void Load(const std::vector<Block>& blocks);
	void Update(uint32_t dt, Ball& ball);
	void Draw(Core::Window& window);

	bool IsLevelComplete() const;
	static std::vector<BreakoutGameLevel> LoadLevelsFromFile(const std::string& filePath);

private:
	void CreateDefaultLevel(const Core::Rectangle& boundary);

private:
	std::vector<Block> m_Blocks;
};