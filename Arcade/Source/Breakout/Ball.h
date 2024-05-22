#pragma once

#include <Core.h>

class Ball
{
public:
	Ball();
	Ball(const Core::Vec2D& pos, float radius);

	void Update(uint32_t dt);
	void Draw(Core::Window& window);
	void MakeFlushWithEdge(const Core::BoundaryEdge& edge, Core::Vec2D& pointOnEdge, bool limitToEdge);
	void MoveTo(const Core::Vec2D& point);

	void Bounce(const Core::BoundaryEdge& edge);

	inline void Stop() { m_Velocity = Core::Vec2D::Zero; }
	inline const Core::Rectangle& GetBoundingBox() const { return m_BoundingBox; }

	inline void SetVelocity(const Core::Vec2D& velocity) { m_Velocity = velocity; }
	inline const Core::Vec2D& GetVelocity() const { return m_Velocity; }
	inline float GetRadius() const { return m_BoundingBox.GetWidth() / 2.f; }
	inline Core::Vec2D GetPosition() const { return m_BoundingBox.GetCenterPoint(); }

private:
	Core::Rectangle m_BoundingBox;
	Core::Vec2D m_Velocity;
};