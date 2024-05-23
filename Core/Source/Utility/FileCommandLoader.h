#pragma once

#include <string>
#include <functional>
#include <vector>

namespace Core
{
	enum class CommandType
	{
		OneLine = 0,
		MultiLine
	};

	struct ParseFuncParams
	{
		std::string Line;
		size_t DilimitPos;
		uint32_t LineNum;
	};

	using ParseFunc = std::function<void(const ParseFuncParams& params)>;

	struct Command
	{
		CommandType CommandType = CommandType::OneLine;
		std::string CommandString = "";
		ParseFunc ParseFunc = nullptr;
	};

	class Color;
	class Vec2D;

	class FileCommandLoader
	{
	public:

		void AddCommand(const Command& command);
		bool LoadFile(const std::string& filePath);

		static Color ReadColor(const ParseFuncParams& params);
		static Vec2D ReadSize(const ParseFuncParams& params);
		static int ReadInt(const ParseFuncParams& params);
		static std::string ReadString(const ParseFuncParams& params);
		static char ReadChar(const ParseFuncParams& params);

	private:
		std::vector<Command> m_Commands;
	};
}
