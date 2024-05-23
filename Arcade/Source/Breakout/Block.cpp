#include "Block.h"
#include "Ball.h"

Block::Block()
	: m_OutlineColor(Core::Color::White()), m_FillColor(Core::Color::White()), m_HP(1)
{
}

void Block::Init(const Core::Rectangle& rect, int hp, const Core::Color outlineColor, const Core::Color fillColor)
{
	Excluder::Init(rect);
	m_HP = hp;
	m_OutlineColor = outlineColor;
	m_FillColor = fillColor;
}

void Block::Draw(Core::Window& window)
{
	window.Draw(GetRectangle(), m_OutlineColor, true, m_FillColor);
}

void Block::Bounce(Ball& ball, const Core::BoundaryEdge& edge)
{
	ball.Bounce(edge);
}

void Block::ReduceHP()
{
	if (m_HP > 0)
		--m_HP;
}
