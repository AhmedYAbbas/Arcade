#include "CorePCH.h"
#include "AnimationPlayer.h"

namespace Core
{
	AnimationPlayer::AnimationPlayer()
		: m_Time(0), m_CurrentAnimation(0), m_Frame(0), m_Looped(false), m_IsPlaying(false), m_IsFinishedPlaying(false), m_HasAnimation(false)
	{
	}

	bool AnimationPlayer::Init(const std::string& animationsFilePath)
	{
		m_Animations = Animation::LoadAnimation(animationsFilePath);
		return m_Animations.size() > 0;
	}

	bool AnimationPlayer::Play(const std::string& animationName, bool looped)
	{
		bool found = false;
		for (size_t i = 0; i < m_Animations.size(); ++i)
		{
			if (m_Animations[i].GetName() == animationName)
			{
				m_CurrentAnimation = i;
				found = true;
				break;
			}
		}

		if (found)
		{
			m_IsPlaying = true;
			m_Frame = 0;
			m_Time = 0;
			m_Looped = looped;
			m_IsFinishedPlaying = false;

			m_HasAnimation = true;
		}

		return found;
	}

	void AnimationPlayer::Pause()
	{
		if (m_IsFinishedPlaying)
			m_IsPlaying = !m_IsPlaying;
	}

	void AnimationPlayer::Stop()
	{
		m_IsPlaying = false;
		m_Time = 0;
		m_Frame = 0;
		m_IsFinishedPlaying = true;
	}

	void AnimationPlayer::Update(uint32_t dt)
	{
		if (m_IsPlaying)
		{
			m_Time += dt;

			int fps = m_Animations[m_CurrentAnimation].FPS();
			float millisecondsPerFrame = 1000.f / static_cast<float>(fps);

			auto numFrames = m_Animations[m_CurrentAnimation].NumFrames();
			uint32_t totalAnimationTime = static_cast<uint32_t>(millisecondsPerFrame * numFrames);
			auto result = m_Time % totalAnimationTime;
			m_Frame = static_cast<uint32_t>(static_cast<float>(result) / millisecondsPerFrame);

			if (m_Time >= totalAnimationTime && !m_Looped)
					m_IsFinishedPlaying = true;
		}
	}

	const Animation& AnimationPlayer::GetCurrentAnimation() const
	{
		assert(m_HasAnimation && "We need an animation for this to work!");
		return m_Animations[m_CurrentAnimation];
	}

	AnimationFrame AnimationPlayer::GetCurrentAnimationFrame() const
	{
		assert(m_HasAnimation && "We need an animation for this to work!");

		if (m_Animations.empty())
			return AnimationFrame();

		return m_Animations[m_CurrentAnimation].GetAnimationFrame(GetCurrentFrameNumber());
	}

	void AnimationPlayer::PrintFrame()
	{
		printf("Frame: %u\n", m_Frame);
	}
}