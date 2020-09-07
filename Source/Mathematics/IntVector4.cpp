#include "Injector/Mathematics/IntVector4.hpp"
#include <cmath>

namespace Injector
{
	IntVector4::IntVector4() :
		x(0), y(0), z(0), w(0)
	{}
	IntVector4::IntVector4(int xyzw) :
		x(xyzw), y(xyzw), z(xyzw), w(xyzw)
	{}
	IntVector4::IntVector4(int _x, int _y, int _z, int _w) :
		x(_x), y(_y), z(_z), w(_w)
	{}
	IntVector4::IntVector4(const IntVector2& vector, int _z, int _w) :
		x(vector.x), y(vector.y), z(_z), w(_w)
	{}
	IntVector4::IntVector4(int _x, const IntVector2& vector, int _w) :
		x(_x), y(vector.x), z(vector.y), w(_w)
	{}
	IntVector4::IntVector4(int _x, int _y, const IntVector2& vector) :
		x(_x), y(_y), z(vector.x), w(vector.y)
	{}
	IntVector4::IntVector4(const IntVector3& vector, int _w) :
		x(vector.x), y(vector.y), z(vector.z), w(_w)
	{}
	IntVector4::IntVector4(int _x, const IntVector3& vector) :
		x(_x), y(vector.x), z(vector.y), w(vector.z)
	{}

	int IntVector4::getDotProduct(const IntVector4& vector) const noexcept
	{
		auto result = *this * vector;
		return (result.x + result.y) + (result.z + result.w);
	}
	float IntVector4::getLength() const noexcept
	{
		return sqrtf(static_cast<float>(getDotProduct(*this)));
	}
	float IntVector4::getDistance(const IntVector4& vector) const noexcept
	{
		return  sqrtf(static_cast<float>(getDotProduct(*this - vector)));
	}
	IntVector4 IntVector4::getReflected(const IntVector4& normal) const noexcept
	{
		return *this - normal * getDotProduct(normal) * 2;
	}

	IntVector2 IntVector4::getIntVector2() const noexcept
	{
		return IntVector2(x, y);
	}
	IntVector3 IntVector4::getIntVector3() const noexcept
	{
		return IntVector3(x, y, z);
	}

	bool IntVector4::operator==(const IntVector4& vector) const noexcept
	{
		return x == vector.x && y == vector.y && z == vector.z && w == vector.w;
	}
	bool IntVector4::operator!=(const IntVector4& vector) const noexcept
	{
		return !(*this == vector);
	}

	IntVector4& IntVector4::operator--() noexcept
	{
		--x; --y; --z; --w;
		return *this;
	}
	IntVector4& IntVector4::operator++() noexcept
	{
		++x; ++y; ++z; ++w;
		return *this;
	}
	IntVector4 IntVector4::operator--(int) noexcept
	{
		auto result = IntVector4(*this);
		--x; --y; --z; --w;
		return result;
	}
	IntVector4 IntVector4::operator++(int) noexcept
	{
		auto result = IntVector4(*this);
		++x; ++y; ++z; ++w;
		return result;
	}
	IntVector4 IntVector4::operator-() const noexcept
	{
		return IntVector4(-x, -y, -z, -w);
	}

	IntVector4 IntVector4::operator-(const IntVector4& vector) const noexcept
	{
		return IntVector4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
	}
	IntVector4 IntVector4::operator+(const IntVector4& vector) const noexcept
	{
		return IntVector4(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
	}
	IntVector4 IntVector4::operator/(const IntVector4& vector) const noexcept
	{
		return IntVector4(x / vector.x, y / vector.y, z / vector.z, w / vector.w);
	}
	IntVector4 IntVector4::operator*(const IntVector4& vector) const noexcept
	{
		return IntVector4(x * vector.x, y * vector.y, z * vector.z, w * vector.w);
	}
	IntVector4 IntVector4::operator%(const IntVector4& vector) const noexcept
	{
		return IntVector4(x % vector.x, y % vector.y, z % vector.z, w % vector.w);
	}
	IntVector4 IntVector4::operator|(const IntVector4& vector) const noexcept
	{
		return IntVector4(x | vector.x, y | vector.y, z | vector.z, w | vector.w);
	}
	IntVector4 IntVector4::operator&(const IntVector4& vector) const noexcept
	{
		return IntVector4(x & vector.x, y & vector.y, z & vector.z, w & vector.w);
	}
	IntVector4 IntVector4::operator^(const IntVector4& vector) const noexcept
	{
		return IntVector4(x ^ vector.x, y ^ vector.y, z ^ vector.z, w ^ vector.w);
	}
	IntVector4 IntVector4::operator<<(const IntVector4& vector) const noexcept
	{
		return IntVector4(x << vector.x, y << vector.y, z << vector.z, w << vector.w);
	}
	IntVector4 IntVector4::operator>>(const IntVector4& vector) const noexcept
	{
		return IntVector4(x >> vector.x, y >> vector.y, z >> vector.z, w >> vector.w);
	}
	IntVector4& IntVector4::operator-=(const IntVector4& vector) noexcept
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		w -= vector.w;
		return *this;
	}
	IntVector4& IntVector4::operator+=(const IntVector4& vector) noexcept
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		w += vector.w;
		return *this;
	}
	IntVector4& IntVector4::operator/=(const IntVector4& vector) noexcept
	{
		x /= vector.x;
		y /= vector.y;
		z /= vector.z;
		w /= vector.w;
		return *this;
	}
	IntVector4& IntVector4::operator*=(const IntVector4& vector) noexcept
	{
		x *= vector.x;
		y *= vector.y;
		z *= vector.z;
		w *= vector.w;
		return *this;
	}
	IntVector4& IntVector4::operator%=(const IntVector4& vector) noexcept
	{
		x %= vector.x;
		y %= vector.y;
		z %= vector.z;
		w %= vector.w;
		return *this;
	}
	IntVector4& IntVector4::operator|=(const IntVector4& vector) noexcept
	{
		x |= vector.x;
		y |= vector.y;
		z |= vector.z;
		w |= vector.w;
		return *this;
	}
	IntVector4& IntVector4::operator&=(const IntVector4& vector) noexcept
	{
		x &= vector.x;
		y &= vector.y;
		z &= vector.z;
		w &= vector.w;
		return *this;
	}
	IntVector4& IntVector4::operator^=(const IntVector4& vector) noexcept
	{
		x ^= vector.x;
		y ^= vector.y;
		z ^= vector.z;
		w ^= vector.w;
		return *this;
	}
	IntVector4& IntVector4::operator<<=(const IntVector4& vector) noexcept
	{
		x <<= vector.x;
		y <<= vector.y;
		z <<= vector.z;
		w <<= vector.w;
		return *this;
	}
	IntVector4& IntVector4::operator>>=(const IntVector4& vector) noexcept
	{
		x >>= vector.x;
		y >>= vector.y;
		z >>= vector.z;
		w >>= vector.w;
		return *this;
	}

	IntVector4 IntVector4::operator-(int value) const noexcept
	{
		return IntVector4(x - value, y - value, z - value, w - value);
	}
	IntVector4 IntVector4::operator+(int value) const noexcept
	{
		return IntVector4(x + value, y + value, z + value, w + value);
	}
	IntVector4 IntVector4::operator/(int value) const noexcept
	{
		return IntVector4(x / value, y / value, z / value, w / value);
	}
	IntVector4 IntVector4::operator*(int value) const noexcept
	{
		return IntVector4(x * value, y * value, z * value, w * value);
	}
	IntVector4 IntVector4::operator%(int value) const noexcept
	{
		return IntVector4(x % value, y % value, z % value, w % value);
	}
	IntVector4 IntVector4::operator|(int value) const noexcept
	{
		return IntVector4(x | value, y | value, z | value, w | value);
	}
	IntVector4 IntVector4::operator&(int value) const noexcept
	{
		return IntVector4(x & value, y & value, z & value, w & value);
	}
	IntVector4 IntVector4::operator^(int value) const noexcept
	{
		return IntVector4(x ^ value, y ^ value, z ^ value, w ^ value);
	}
	IntVector4 IntVector4::operator<<(int value) const noexcept
	{
		return IntVector4(x << value, y << value, z << value, w << value);
	}
	IntVector4 IntVector4::operator>>(int value) const noexcept
	{
		return IntVector4(x >> value, y >> value, z >> value, w >> value);
	}
	IntVector4& IntVector4::operator-=(int value) noexcept
	{
		x -= value;
		y -= value;
		z -= value;
		w -= value;
		return *this;
	}
	IntVector4& IntVector4::operator+=(int value) noexcept
	{
		x += value;
		y += value;
		z += value;
		w += value;
		return *this;
	}
	IntVector4& IntVector4::operator/=(int value) noexcept
	{
		x /= value;
		y /= value;
		z /= value;
		w /= value;
		return *this;
	}
	IntVector4& IntVector4::operator*=(int value) noexcept
	{
		x *= value;
		y *= value;
		z *= value;
		w *= value;
		return *this;
	}
	IntVector4& IntVector4::operator%=(int value) noexcept
	{
		x %= value;
		y %= value;
		z %= value;
		w %= value;
		return *this;
	}
	IntVector4& IntVector4::operator|=(int value) noexcept
	{
		x |= value;
		y |= value;
		z |= value;
		w |= value;
		return *this;
	}
	IntVector4& IntVector4::operator&=(int value) noexcept
	{
		x &= value;
		y &= value;
		z &= value;
		w &= value;
		return *this;
	}
	IntVector4& IntVector4::operator^=(int value) noexcept
	{
		x ^= value;
		y ^= value;
		z ^= value;
		w ^= value;
		return *this;
	}
	IntVector4& IntVector4::operator<<=(int value) noexcept
	{
		x <<= value;
		y <<= value;
		z <<= value;
		w <<= value;
		return *this;
	}
	IntVector4& IntVector4::operator>>=(int value) noexcept
	{
		x >>= value;
		y >>= value;
		z >>= value;
		w >>= value;
		return *this;
	}

	const IntVector4 IntVector4::zero = IntVector4(0);
	const IntVector4 IntVector4::minusOne = IntVector4(-1);
	const IntVector4 IntVector4::one = IntVector4(1);
}
