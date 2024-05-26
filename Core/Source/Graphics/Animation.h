#pragma once

#include "Utility/Vec2D.h"
#include "Color.h"

namespace Core
{
	struct AnimationFrame
	{
		std::string Frame = "";
		std::string Overlay = "";
		Color FrameColor = Color::White();
		Color OverlayColor = Color::White();
		Vec2D Size;
		Vec2D Offset;
		int FrameColorSet = 0;
	};

	class Animation
	{
	public:
		Animation();

		static std::vector<Animation> LoadAnimation(const std::string& filePath);

		AnimationFrame GetAnimationFrame(uint32_t frameNum) const;

		inline void AddFrame(const std::string& frame) { m_Frames.push_back(frame); }
		inline void AddFrameColor(const Color& color) { m_FrameColors.push_back(color); }
		inline void AddOverlayColor(const Color& color) { m_OverlayColors.push_back(color); }
		inline void AddFrameOffset(const Vec2D& offset) { m_FrameOffsets.push_back(offset); }

		inline void SetSpriteSheetName(const std::string& spriteSheetName) { m_SpriteSheetName = spriteSheetName; }
		inline void SetName(const std::string& animationName) { m_AnimationName = animationName; }
		inline void SetSize(const Vec2D& size) { m_Size = size; }
		inline void SetFPS(int fps) { m_FPS = fps; }
		inline void SetOverlay(const std::string& overlayName) { m_Overlay = overlayName; }

		inline const std::string& GetSpriteName() const { return m_SpriteSheetName; }
		inline const std::string& GetName() const { return m_AnimationName; }
		inline const Vec2D& GetSize() const { return m_Size; }
		inline int FPS() const { return m_FPS; }
		inline const std::string& OverlayName() const { return m_Overlay; }

		inline size_t NumFrames() const { return m_Frames.size(); }
		inline size_t NumFramesColors() const { return m_FrameColors.size(); }
		inline size_t NumOverlayColors() const { return m_OverlayColors.size(); }
		inline size_t NumFrameOffsets() const { return m_FrameOffsets.size(); }

	private:
		std::string m_AnimationName;
		std::string m_SpriteSheetName;
		std::string m_Overlay;
		std::vector<std::string> m_Frames;
		std::vector<Color> m_FrameColors;
		std::vector<Color> m_OverlayColors;
		std::vector<Vec2D> m_FrameOffsets;
		Vec2D m_Size;
		int m_FPS;
	};
}