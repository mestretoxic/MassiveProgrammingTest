#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

//Made this a template class for versatility
template<typename T>
class Vector2
{
public:
	T x, y;

	Vector2(T pX, T pY)
	{
		Set(pX, pY);
	}
	
	void Set(T pX, T pY)
	{
		x = pX;
		y = pY;
	}

	bool operator==(const Vector2 &other) const 
	{
		return x == other.x && y == other.y;
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
};

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;

#endif // VECTOR2_H