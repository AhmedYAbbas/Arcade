#include "CorePCH.h"
#include "SpriteSheet.h"
#include "Utility/FileCommandLoader.h"
#include "Utility/Utils.h"
#include "Core/Application.h"

namespace Core
{
	SpriteSheet::SpriteSheet()
	{
	}

	bool SpriteSheet::Load(const std::string& name)
	{
		bool loadedImage = m_BMPImage.Load(Application::Get().GetBasePath() + name + ".bmp");
		bool loadedSpriteSections = LoadSpriteSections(Application::Get().GetBasePath() + name + ".txt");
		return loadedImage && loadedSpriteSections;
	}

	Sprite SpriteSheet::GetSprite(const std::string& spriteName) const
	{
		size_t length = m_Sections.size();
		for (size_t i = 0; i < length; ++i)
		{
			if (StringCompare(m_Sections[i].key, spriteName))
				return m_Sections[i].sprite;
		}
		return Sprite();
	}

	std::vector<std::string> SpriteSheet::SpriteNames() const
	{
		std::vector<std::string> spriteNames;
		for (const auto& section : m_Sections)
			spriteNames.push_back(section.key);
		return spriteNames;
	}

	bool SpriteSheet::LoadSpriteSections(const std::string& filePath)
	{
		FileCommandLoader fileLoader;

		Command spriteCommand;
		spriteCommand.CommandString = "sprite";
		spriteCommand.ParseFunc = [&](ParseFuncParams params)
		{
			BMPImageSection section;
			m_Sections.push_back(section);
		};
		fileLoader.AddCommand(spriteCommand);

		Command keyCommand;
		keyCommand.CommandString = "key";
		keyCommand.ParseFunc = [&](ParseFuncParams params)
		{
			m_Sections.back().key = FileCommandLoader::ReadString(params);
		};
		fileLoader.AddCommand(keyCommand);
		
		Command xPosCommand;
		xPosCommand.CommandString = "xPos";
		xPosCommand.ParseFunc = [&](ParseFuncParams params)
		{
			m_Sections.back().sprite.xPos = FileCommandLoader::ReadInt(params);
		};
		fileLoader.AddCommand(xPosCommand);
		
		Command yPosCommand;
		yPosCommand.CommandString = "yPos";
		yPosCommand.ParseFunc = [&](ParseFuncParams params)
		{
			m_Sections.back().sprite.yPos = FileCommandLoader::ReadInt(params);
		};
		fileLoader.AddCommand(yPosCommand);
		
		Command widthCommand;
		widthCommand.CommandString = "width";
		widthCommand.ParseFunc = [&](ParseFuncParams params)
		{
			m_Sections.back().sprite.width = FileCommandLoader::ReadInt(params);
		};
		fileLoader.AddCommand(widthCommand);
		
		Command heightCommand;
		heightCommand.CommandString = "height";
		heightCommand.ParseFunc = [&](ParseFuncParams params)
		{
			m_Sections.back().sprite.height = FileCommandLoader::ReadInt(params);
		};
		fileLoader.AddCommand(heightCommand);

		return fileLoader.LoadFile(filePath);
	}
}