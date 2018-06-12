#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
//Made this a template class for versatility
template<typename T>
class Vector2<T>
{
public:
	Vector2()
	{
		x = 0.f;
		y = 0.f;
	}

	Vector2(T anX, T anY)
	{
		x = anX;
		y = anY;
	}

	const Vector2 operator-(const Vector2 &other) const 
	{
		Vector2 v(x - other.x, y - other.y);
		return v;
	}

	const Vector2 operator+(const Vector2 &other) const 
	{
		Vector2 v(x + other.x, y + other.y);
		return v;
	}

	
	const Vector2 operator*(const Vector2& other) const 
	{
		Vector2 v(x*other.x, y*other.y);
		return v;
	}

	Vector2& operator+=(const Vector2 &other) 
	{
		x = x + other.x;
		y = y + other.y;

		return *this;
	}

	Vector2& operator*=(const T rValue) 
	{
		x *= rValue;
		y *= rValue;

		return *this;
	}

	Vector2& operator/=(const T rValue) 
	{
		x /= rValue;
		y /= rValue;

		return *this;
	}

	const Vector2 operator*(const T rValue) const 
	{
		Vector2 v(x * rValue, y * rValue);
		return v;
	}

	T Length() const
	{
		return std::sqrt(x*x + y*y);
	}

	void Normalize()
	{
		T length = Length();

		if (length > 0.f)
			*this /= length;
	}

	float x;
	float y;
};

#endif // VECTOR2_H