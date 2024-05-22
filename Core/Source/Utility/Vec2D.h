#pragma once

#include <iostream>
#include <assert.h>

#include "Utils.h"

namespace Core
{
	class Vec2D
	{
	public:
		Vec2D();
		Vec2D(float x, float y);

		inline void SetX(float x) { m_X = x; }
		inline void SetY(float y) { m_Y = y; }
		inline const float GetX() const { return m_X; }
		inline const float GetY() const { return m_Y; }

		float Mag2() const;
		float Mag() const;

		Vec2D GetUnitVec() const;
		Vec2D& Normalize();

		float Distance(const Vec2D& other) const;
		float Dot(const Vec2D& other) const;

		Vec2D ProjectOnto(const Vec2D& other) const;
		Vec2D Reflect(const Vec2D& normal) const;
		float AngleBetween(const Vec2D& other) const;

		void Rotate(float angle, const Vec2D& aroundPoint);
		Vec2D Rotation(float angle, const Vec2D& aroundPoint) const;

		Vec2D operator-() const;
		Vec2D& operator+=(const Vec2D& other);
		Vec2D& operator-=(const Vec2D& other);
		Vec2D& operator*=(float scale);
		Vec2D& operator/=(float scale);

		friend std::ostream& operator<<(std::ostream& os, const Vec2D& vec);

	public:
		static const Vec2D Zero;
		static const Vec2D Up;
		static const Vec2D Down;
		static const Vec2D Right;
		static const Vec2D Left;

	private:
		float m_X;
		float m_Y;
	};

	bool operator==(const Vec2D& vec1, const Vec2D& vec2);
	bool operator!=(const Vec2D& vec1, const Vec2D& vec2);
	Vec2D operator+(const Vec2D& vec1, const Vec2D& vec2);
	Vec2D operator-(const Vec2D& vec1, const Vec2D& vec2);
	Vec2D operator*(const Vec2D& vec, float scale);
	Vec2D operator*(float scale, const Vec2D& vec);
	Vec2D operator/(const Vec2D& vec, float scale);
}

