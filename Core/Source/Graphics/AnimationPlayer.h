#pragma once

#include "Animation.h"

namespace Core
{
	class AnimationPlayer
	{
	public:
		AnimationPlayer();

		bool Init(const std::string& animationsFilePath);
		bool Play(const std::string& animationName, bool looped);
		void Pause();
		void Stop();
		void Update(uint32_t dt);
		AnimationFrame GetCurrentAnimationFrame() const;

		inline uint32_t GetCurrentFrameNumber() const { return m_Frame; }
		inline bool IsFinishedPlaying() const { return m_IsFinishedPlaying; }

		const Animation& GetCurrentAnimation() const;
		inline bool IsPlaying() const { return m_IsPlaying; }
		void PrintFrame();

		bool HasAnimation() const { return m_HasAnimation; }

	private:
		uint32_t m_Time;
		std::vector<Animation> m_Animations;
		size_t m_CurrentAnimation;
		uint32_t m_Frame;

		bool m_Looped;
		bool m_IsPlaying;
		bool m_IsFinishedPlaying;
		bool m_HasAnimation;
	};
}