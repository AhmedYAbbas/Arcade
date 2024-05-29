#include "CorePCH.h"
#include "FileCommandLoader.h"
#include "Graphics/Color.h"
#include "Vec2D.h"

namespace Core
{
	void FileCommandLoader::AddCommand(const Command& command)
	{
		m_Commands.push_back(command);
	}

	bool FileCommandLoader::LoadFile(const std::string& filePath)
	{
		std::ifstream inFile;
		inFile.open(filePath);
		std::string line = "";
		if (!inFile.is_open())
		{
			std::cout << "Could not open the file: " << filePath << std::endl;
			return false;
		}

		while (!inFile.eof())
		{
			std::getline(inFile, line);
			size_t commandPos = std::string::npos;
			if ((commandPos = line.find(":")) != std::string::npos)
			{
				size_t dilimitPos = line.find_first_of(" ", commandPos);
				if (dilimitPos == std::string::npos)
					dilimitPos = line.length();
				else
					dilimitPos -= 1;

				std::string commandStr = line.substr(commandPos + 1, dilimitPos);
				dilimitPos += 1;
				for (size_t commandIndex = 0; commandIndex < m_Commands.size(); ++commandIndex)
				{
					if (commandStr == m_Commands[commandIndex].CommandString)
					{
						if (m_Commands[commandIndex].CommandType == CommandType::OneLine)
						{
							ParseFuncParams params;
							params.DilimitPos = dilimitPos;
							params.LineNum = 0;
							params.Line = line;
							m_Commands[commandIndex].ParseFunc(params);
						}
						else
						{
							std::string numLines = line.substr(dilimitPos + 1);
							int totalLines = std::stoi(numLines);
							int lineNum = 0;
							while (lineNum < totalLines)
							{
								std::getline(inFile, line);
								if (line.empty())
									continue;
								else
								{
									ParseFuncParams params;
									params.DilimitPos = 0;
									params.LineNum = lineNum;
									params.Line = line;
									m_Commands[commandIndex].ParseFunc(params);
									++lineNum;
								}
							}
						}
					}
				}

			}
		}
		return true;
	}

	Color FileCommandLoader::ReadColor(const ParseFuncParams& params)
	{
		size_t nextSpacePos = params.Line.find_first_of(" ", params.DilimitPos + 1);
		int r = std::stoi(params.Line.substr(params.DilimitPos, nextSpacePos - params.DilimitPos));

		size_t lastSpacePos = nextSpacePos;
		nextSpacePos = params.Line.find_first_of(" ", lastSpacePos + 1);
		int g = std::stoi(params.Line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));

		lastSpacePos = nextSpacePos;
		nextSpacePos = params.Line.find_first_of(" ", lastSpacePos + 1);
		int b = std::stoi(params.Line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));
		int a = std::stoi(params.Line.substr(nextSpacePos + 1));

		return Color(r, g, b, a);
	}

	Vec2D FileCommandLoader::ReadSize(const ParseFuncParams& params)
	{
		size_t nextSpacePos = params.Line.find_first_of(" ", params.DilimitPos + 1);
		int width = std::stoi(params.Line.substr(params.DilimitPos, nextSpacePos - params.DilimitPos));
		size_t lastSpacePos = nextSpacePos;
		nextSpacePos = params.Line.find_first_of(" ", lastSpacePos + 1);
		int height = std::stoi(params.Line.substr(lastSpacePos + 1, nextSpacePos - lastSpacePos));
		return Vec2D(width, height);
	}

	int FileCommandLoader::ReadInt(const ParseFuncParams& params)
	{
		std::string intStr = params.Line.substr(params.DilimitPos + 1);
		return std::stoi(intStr);
	}

	std::string FileCommandLoader::ReadString(const ParseFuncParams& params)
	{
		return params.Line.substr(params.DilimitPos + 1);
	}

	char FileCommandLoader::ReadChar(const ParseFuncParams& params)
	{
		return params.Line.substr(params.DilimitPos + 1)[0];
	}
}