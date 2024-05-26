#include "CorePCH.h"
#include "Animation.h"
#include "Utility/FileCommandLoader.h"

namespace Core
{
	Animation::Animation()
		: m_Size(Vec2D::Zero), m_SpriteSheetName(""), m_AnimationName(""), m_FPS(0)
	{
	}

	std::vector<Animation> Animation::LoadAnimation(const std::string& filePath)
	{
		std::vector<Animation> animations;
		FileCommandLoader fileLoader;

		Command animationCommand;
		animationCommand.CommandString = "animation";
		animationCommand.ParseFunc = [&](ParseFuncParams params)
		{
			Animation animation;
			animation.SetName(FileCommandLoader::ReadString(params));
			animations.push_back(animation);
		};
		fileLoader.AddCommand(animationCommand);

		Command spriteSheetCommand;
		spriteSheetCommand.CommandString = "sprite_sheet";
		spriteSheetCommand.ParseFunc = [&](ParseFuncParams params)
		{
			animations.back().SetSpriteSheetName(FileCommandLoader::ReadString(params));
		};
		fileLoader.AddCommand(spriteSheetCommand);

		Command sizeCommand;
		sizeCommand.CommandString = "size";
		sizeCommand.ParseFunc = [&](ParseFuncParams params)
		{
			animations.back().SetSize(FileCommandLoader::ReadSize(params));
		};
		fileLoader.AddCommand(sizeCommand);

		Command fpsCommand;
		fpsCommand.CommandString = "fps";
		fpsCommand.ParseFunc = [&](ParseFuncParams params)
		{
			animations.back().SetFPS(FileCommandLoader::ReadInt(params));
		};
		fileLoader.AddCommand(fpsCommand);

		Command framesCommand;
		framesCommand.CommandString = "frame_keys";
		framesCommand.CommandType = CommandType::MultiLine;
		framesCommand.ParseFunc = [&](ParseFuncParams params)
		{
			animations.back().AddFrame(params.Line);
		};
		fileLoader.AddCommand(framesCommand);

		Command overlayCommand;
		overlayCommand.CommandString = "overlay";
		overlayCommand.ParseFunc = [&](ParseFuncParams params)
		{
			animations.back().SetOverlay(FileCommandLoader::ReadString(params));
		};
		fileLoader.AddCommand(overlayCommand);

		Command frameColorsCommand;
		frameColorsCommand.CommandString = "frame_colors";
		frameColorsCommand.CommandType = CommandType::MultiLine;
		frameColorsCommand.ParseFunc = [&](ParseFuncParams params)
		{
			animations.back().AddFrameColor(FileCommandLoader::ReadColor(params));
		};
		fileLoader.AddCommand(frameColorsCommand);

		Command overlayFrameColorCommand;
		overlayFrameColorCommand.CommandString = "overlay_colors";
		overlayFrameColorCommand.CommandType = CommandType::MultiLine;
		overlayFrameColorCommand.ParseFunc = [&](ParseFuncParams params)
		{
			animations.back().AddOverlayColor(FileCommandLoader::ReadColor(params));
		};
		fileLoader.AddCommand(overlayFrameColorCommand);

		Command frameOffsetsCommand;
		frameOffsetsCommand.CommandString = "frame_offsets";
		frameOffsetsCommand.CommandType = CommandType::MultiLine;
		frameOffsetsCommand.ParseFunc = [&](ParseFuncParams params)
		{
			animations.back().AddFrameOffset(FileCommandLoader::ReadSize(params));
		};
		fileLoader.AddCommand(frameOffsetsCommand);

		assert(fileLoader.LoadFile(filePath));

		return animations;
	}

	AnimationFrame Animation::GetAnimationFrame(uint32_t frameNum) const
	{
		AnimationFrame frame;

		if (frameNum > m_Frames.size())
			return frame;

		frame.Frame = m_Frames[frameNum];

		if (frameNum < m_FrameColors.size())
		{
			frame.FrameColor = m_FrameColors[frameNum];
			frame.FrameColorSet = 1;
		}

		if (m_Overlay.size() > 0)
			frame.Overlay = m_Overlay;

		if (frameNum < m_OverlayColors.size())
			frame.OverlayColor = m_OverlayColors[frameNum];

		if (frameNum < m_FrameOffsets.size())
			frame.Offset = m_FrameOffsets[frameNum];

		frame.Size = m_Size;

		return frame;
	}
}