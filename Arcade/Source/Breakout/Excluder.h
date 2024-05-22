#pragma once

#include <Core.h>

class Excluder
{
public:
	virtual ~Excluder() = default;

	void Init(const Core::Rectangle& rect, bool reverseNormals = false);
	bool HasCollided(const Core::Rectangle& other, Core::BoundaryEdge& edge) const;
	Core::Vec2D GetCollisionOffset(const Core::Rectangle& other) const;
	void MoveBy(const Core::Vec2D& delta);
	void MoveTo(const Core::Vec2D& point);
	const Core::BoundaryEdge& GetEdge(Core::EdgeType edge) const;

	inline const Core::Rectangle& GetRectangle() const { return m_Rect; }

private:
	void SetupEdges();

private:
	Core::Rectangle m_Rect;
	Core::BoundaryEdge m_Edges[Core::EdgeType::Num] ;
	bool m_ReverseNormals;
};