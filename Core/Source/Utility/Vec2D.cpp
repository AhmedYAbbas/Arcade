#include "Vec2D.h"

namespace Core
{
	const Vec2D Vec2D::Zero;

	bool operator==(const Vec2D& vec1, const Vec2D& vec2)
	{
		return IsEqual(vec1.GetX(), vec2.GetX()) && IsEqual(vec1.GetY(), vec2.GetY());
	}

	bool operator!=(const Vec2D& vec1, const Vec2D& vec2)
	{
		return !(vec1 == vec2);
	}

	Vec2D operator+(const Vec2D& vec1, const Vec2D& vec2)
	{
		return {vec1.GetX() + vec2.GetX(), vec1.GetY() + vec2.GetY()};
	}

	Vec2D operator-(const Vec2D& vec1, const Vec2D& vec2)
	{
		return {vec1.GetX() - vec2.GetX(), vec1.GetY() - vec2.GetY()};
	}

	Vec2D operator*(const Vec2D& vec, float scale)
	{
		return {vec.GetX() * scale, vec.GetY() * scale};
	}

	Vec2D operator*(float scale, const Vec2D& vec)
	{
		return {vec.GetX() * scale, vec.GetY() * scale};
	}

	Vec2D operator/(const Vec2D& vec, float scale)
	{
		assert(std::fabsf(scale) > EPSILON);
		return {vec.GetX() / scale, vec.GetY() / scale};
	}

	Vec2D::Vec2D()
		: Vec2D(0.f, 0.f)
	{
	}

	Vec2D::Vec2D(float x, float y)
		: m_X(x), m_Y(y)
	{
	}

	float Vec2D::Mag2() const
	{
		return m_X * m_X + m_Y * m_Y;
	}

	float Vec2D::Mag() const
	{
		return std::sqrt(Mag2());
	}

	Vec2D Vec2D::GetUnitVec() const
	{
		if (Mag2() <= EPSILON)
			return Zero;
		return *this / Mag();
	}

	Vec2D& Vec2D::Normalize()
	{
		if (Mag2() > EPSILON)
			*this /= Mag();

		return *this;
	}

	float Vec2D::Distance(const Vec2D& other) const
	{
		return (other - *this).Mag();
	}

	float Vec2D::Dot(const Vec2D& other) const
	{
		return m_X * other.m_X + m_Y * other.m_Y;
	}

	Vec2D Vec2D::ProjectOnto(const Vec2D& other) const
	{
		return Dot(other.GetUnitVec()) * other.GetUnitVec();
	}

	Vec2D Vec2D::Reflect(const Vec2D& normal) const
	{
		return *this - 2 * ProjectOnto(normal);
	}

	float Vec2D::AngleBetween(const Vec2D& other) const
	{
		return std::acosf(GetUnitVec().Dot(other.GetUnitVec()));
	}

	void Vec2D::Rotate(float angle, const Vec2D& aroundPoint)
	{
		Vec2D self(m_X - aroundPoint.GetX(), m_Y - aroundPoint.GetY());
		Vec2D rotatedSelf(self.m_X * std::cosf(angle) - self.m_Y * std::sinf(angle), self.m_X * std::sinf(angle) + self.m_Y * std::cosf(angle));
		*this = rotatedSelf + aroundPoint;
	}

	Vec2D Vec2D::Rotation(float angle, const Vec2D& aroundPoint) const
	{
		Vec2D self(m_X - aroundPoint.GetX(), m_Y - aroundPoint.GetY());
		Vec2D rotatedSelf(self.m_X * std::cosf(angle) - self.m_Y * std::sinf(angle), self.m_X * std::sinf(angle) + self.m_Y * std::cosf(angle));
		return rotatedSelf + aroundPoint;
	}

	Vec2D Vec2D::operator-() const
	{
		return Vec2D(-m_X, -m_Y);
	}

	Vec2D& Vec2D::operator+=(const Vec2D& other)
	{
		*this = *this + other;
		return *this;
	}

	Vec2D& Vec2D::operator-=(const Vec2D& other)
	{
		*this = *this - other;
		return *this;
	}

	Vec2D& Vec2D::operator*=(float scale)
	{
		*this = *this * scale;
		return *this;
	}

	Vec2D& Vec2D::operator/=(float scale)
	{
		assert(std::fabsf(scale) > EPSILON);

		*this = *this / scale;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const Vec2D& vec)
	{
		os << "X: " << vec.m_X << ", Y: " << vec.m_Y << '\n';
		return os;
	}
}