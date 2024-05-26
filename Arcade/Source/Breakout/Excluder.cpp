#include "ArcadePCH.h"
#include "Excluder.h"

void Excluder::Init(const Core::Rectangle& rect, bool reverseNormals)
{
	m_Rect = rect;
	m_ReverseNormals = reverseNormals;
	SetupEdges();
}

bool Excluder::HasCollided(const Core::Rectangle& other, Core::BoundaryEdge& edge) const
{
	if (m_Rect.Intersects(other))
	{
		float yMin = m_Rect.GetTopLeftPoint().GetY() >= other.GetTopLeftPoint().GetY() ? m_Rect.GetTopLeftPoint().GetY() : other.GetTopLeftPoint().GetY();
		float yMax = m_Rect.GetBottomRightPoint().GetY() <= other.GetBottomRightPoint().GetY() ? m_Rect.GetBottomRightPoint().GetY() : other.GetBottomRightPoint().GetY();
		float ySize = yMax - yMin;

		float xMin = m_Rect.GetTopLeftPoint().GetX() >= other.GetTopLeftPoint().GetX() ? m_Rect.GetTopLeftPoint().GetX() : other.GetTopLeftPoint().GetX();
		float xMax = m_Rect.GetBottomRightPoint().GetX() <= other.GetBottomRightPoint().GetX() ? m_Rect.GetBottomRightPoint().GetX() : other.GetBottomRightPoint().GetX();
		float xSize = xMax - xMin;

		if (xSize > ySize)
		{
			if (other.GetCenterPoint().GetY() > m_Rect.GetCenterPoint().GetY())
				edge = m_Edges[Core::EdgeType::Bottom];
			else
				edge = m_Edges[Core::EdgeType::Top];
		}
		else
		{
			if (other.GetCenterPoint().GetX() < m_Rect.GetCenterPoint().GetX())
				edge = m_Edges[Core::EdgeType::Left];
			else
				edge = m_Edges[Core::EdgeType::Right];
		}
		return true;
	}
	return false;
}

Core::Vec2D Excluder::GetCollisionOffset(const Core::Rectangle& other) const
{
	Core::BoundaryEdge edge;
	Core::Vec2D offset = Core::Vec2D::Zero;

	if (HasCollided(other, edge))
	{
		float yMin = m_Rect.GetTopLeftPoint().GetY() >= other.GetTopLeftPoint().GetY() ? m_Rect.GetTopLeftPoint().GetY() : other.GetTopLeftPoint().GetY();
		float yMax = m_Rect.GetBottomRightPoint().GetY() <= other.GetBottomRightPoint().GetY() ? m_Rect.GetBottomRightPoint().GetY() : other.GetBottomRightPoint().GetY();
		float ySize = yMax - yMin;

		float xMin = m_Rect.GetTopLeftPoint().GetX() >= other.GetTopLeftPoint().GetX() ? m_Rect.GetTopLeftPoint().GetX() : other.GetTopLeftPoint().GetX();
		float xMax = m_Rect.GetBottomRightPoint().GetX() <= other.GetBottomRightPoint().GetX() ? m_Rect.GetBottomRightPoint().GetX() : other.GetBottomRightPoint().GetX();
		float xSize = xMax - xMin;

		if (!Core::IsEqual(edge.Normal.GetY(), 0))
			offset = (ySize + 1) * edge.Normal;
		else
			offset = (xSize + 1) * edge.Normal;
	}

	return offset;
}

void Excluder::MoveBy(const Core::Vec2D& delta)
{
	m_Rect.MoveBy(delta);
	SetupEdges();
}

void Excluder::MoveTo(const Core::Vec2D& point)
{
	m_Rect.MoveTo(point);
	SetupEdges();
}

const Core::BoundaryEdge& Excluder::GetEdge(Core::EdgeType edge) const
{
	assert(edge != Core::EdgeType::Num);
	return m_Edges[edge];
}

void Excluder::SetupEdges()
{
	m_Edges[Core::EdgeType::Top].Edge = {m_Rect.GetTopLeftPoint().GetX(), m_Rect.GetTopLeftPoint().GetY(), m_Rect.GetBottomRightPoint().GetX(), m_Rect.GetTopLeftPoint().GetY()};
	m_Edges[Core::EdgeType::Top].Normal = Core::Vec2D::Up;

	m_Edges[Core::EdgeType::Left].Edge = {m_Rect.GetTopLeftPoint().GetX(), m_Rect.GetTopLeftPoint().GetY(), m_Rect.GetTopLeftPoint().GetX(), m_Rect.GetBottomRightPoint().GetY()};
	m_Edges[Core::EdgeType::Left].Normal = Core::Vec2D::Left;

	m_Edges[Core::EdgeType::Bottom].Edge = {m_Rect.GetTopLeftPoint().GetX(), m_Rect.GetBottomRightPoint().GetY(), m_Rect.GetBottomRightPoint().GetX(), m_Rect.GetBottomRightPoint().GetY()};
	m_Edges[Core::EdgeType::Bottom].Normal = Core::Vec2D::Down;

	m_Edges[Core::EdgeType::Right].Edge = {m_Rect.GetBottomRightPoint().GetX(), m_Rect.GetTopLeftPoint().GetY(), m_Rect.GetBottomRightPoint().GetX(), m_Rect.GetBottomRightPoint().GetY()};
	m_Edges[Core::EdgeType::Right].Normal = Core::Vec2D::Right;

	if (m_ReverseNormals)
	{
		for (auto& edge : m_Edges)
			edge.Normal = -edge.Normal;
	}
}
