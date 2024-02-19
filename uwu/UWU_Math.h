#ifndef UWU_Math
#define UWU_Math
#include <SDL.h>
#include <cmath>
struct int2;
struct float2 {
	float x;
	float y;
	float2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	float2() {
		this->x = 0;
		this->y = 0;
	}
	float2& operator+=(const float2& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	float2& operator-=(const float2& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}
	float2& operator-=(const int2& rhs);
	float2& operator*=(const int& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}
	float2& operator*=(const float& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}
	float2 operator+(const float2& rhs)
	{
		return float2(this->x + rhs.x, this->y + rhs.y);
	}
	float2 operator*(const int& rhs)
	{
		return float2(this->x * rhs, this->y * rhs);
	}
	float2 operator*(const float& rhs)
	{
		return float2(this->x * rhs, this->y * rhs);
	}
	float2 operator*(const float2& rhs)
	{
		return float2(this->x * rhs.x, this->y * rhs.y);
	}
	float2 operator/(const float& rhs)
	{
		return float2(this->x / rhs, this->y / rhs);
	}
	float2 operator-() const
	{
		return float2(-this->x, -this->y);
	}
	float2 operator-(const float2& rhs)
	{
		return float2(this->x - rhs.x, this->y - rhs.y);
	}
	bool operator==(const float2& rhs)
	{
		return this->x == rhs.x && this->y == rhs.y;
	}
	bool operator!=(const float2& rhs)
	{
		return this->x != rhs.x || this->y != rhs.y;
	}
	operator float2() const {

		return float2(static_cast<float>(x), static_cast<float>(y));
	}
	explicit operator int2() const;
	float Distance(const float2& other) const {
		float dx = other.x - x;
		float dy = other.y - y;
		return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
	}
	void Normalise() {
		float length = std::sqrt(std::pow(x, 2) + std::pow(y, 2));
		x /= length;
		y /= length;
	}
	float Dot(const float2& other) const {
		return (x * other.x) + (y * other.y);
	}
	float Dot(const int2& other) const;
	static float2 FromSize(SDL_Rect rect) {
		return float2(rect.w, rect.h);
	}
	static float2 FromPosition(SDL_Rect rect) {
		return float2(rect.x, rect.y);
	}
};
struct int2 {
	int x;
	int y;
	int2(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int2() {
		this->x = 0;
		this->y = 0;
	}
	int2& operator+=(const int2& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	int2& operator-=(const float2& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}
	int2 operator-(const int2& rhs)
	{
		return int2(this->x - rhs.x, this->y - rhs.y);
	}
	int2 operator-(const float2& rhs)
	{
		return int2(this->x - rhs.x, this->y - rhs.y);
	}
	int2 operator+(const int2& rhs)
	{
		return int2(this->x + rhs.x, this->y + rhs.y);
	}
	int2 operator+(const float2& rhs)
	{
		return int2(this->x + rhs.x, this->y + rhs.y);
	}
	int2 operator*(const int& rhs)
	{
		return int2(this->x * rhs, this->y * rhs);
	}
	int2 operator*(const float& rhs)
	{
		return int2(this->x * rhs, this->y * rhs);
	}
	operator int2() const {

		return int2(static_cast<int>(x), static_cast<int>(y));
	}
	explicit operator float2() const {
		return float2(static_cast<float>(x), static_cast<float>(y));
	}
	float Distance(const int2& other) const {
		float dx = other.x - x;
		float dy = other.y - y;
		return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
	}
	static int2 FromSize(SDL_Rect rect) {
		return int2(rect.w, rect.h);
	}
	static int2 FromPosition(SDL_Rect rect) {
		return int2(rect.x, rect.y);
	}
};


#endif // !UWU_Math
