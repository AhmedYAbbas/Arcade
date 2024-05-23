#pragma once

#include <Core.h>

#include "Excluder.h"

class Ball;
class Window;

class Block : public Excluder
{
public:
	Block();

	void Init(const Core::Rectangle& rect, int hp, const Core::Color outlineColor, const Core::Color fillColor);
	void Draw(Core::Window& window);
	void Bounce(Ball& ball, const Core::BoundaryEdge& edge);

	void ReduceHP();

	inline int GetHP() const { return m_HP; }
	inline bool IsDestroyed() const { return m_HP == 0; }
	inline const Core::Color& GetOutlineColor() const { return m_OutlineColor; }
	inline const Core::Color& GetFillColor() const { return m_FillColor; }

public:
	static const int s_Unbreakable = -1;

private:
	Core::Color m_OutlineColor;
	Core::Color m_FillColor;

	int m_HP = -1;
};