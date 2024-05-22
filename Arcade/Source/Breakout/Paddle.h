#pragma once

#include "Excluder.h"

enum PaddleDirection
{
	Left = 1 << 0,
	Right = 1 << 1
};

class Paddle : public Excluder
{
public:
	void Init(const Core::Rectangle& rect, const Core::Rectangle& boundary);
	void Update(uint32_t dt);
	void Draw(Core::Window& window);

	inline bool IsMovingLeft() const { return m_Direction == PaddleDirection::Left; }
	inline bool IsMovingRight() const { return m_Direction == PaddleDirection::Right; }

	inline void SetMovementDirection(PaddleDirection direction) { m_Direction |= direction; }
	inline void UnsetMovementDirection(PaddleDirection direction) { m_Direction &= ~direction; }
	inline void StopMovement() { m_Direction = 0; }

public:
	inline static const uint32_t s_PaddleWidth = 50;
	inline static const uint32_t s_PaddleHeight = 10;

private:
	uint32_t m_Direction;
	Core::Rectangle m_Boundary;
	const float m_Velocity = 100.f;
};