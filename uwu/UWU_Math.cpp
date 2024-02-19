#include "UWU_Math.h"

float2& float2::operator-=(const int2& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	return *this;
}

float2::operator int2() const
{
	return int2(std::llroundf(x), std::llroundf(y));
}

float float2::Dot(const int2& rhs) const
{
	return this->x * rhs.x + this->y * rhs.y;
}