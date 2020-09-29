#include "Injector/Mathematics/SizeVector4.hpp"
#include <sstream>

namespace Injector
{
	SizeVector4::SizeVector4() :
		x(0),
		y(0),
		z(0),
		w(0)
	{
	}
	SizeVector4::SizeVector4(
		size_t xyzw) :
		x(xyzw),
		y(xyzw),
		z(xyzw),
		w(xyzw)
	{
	}
	SizeVector4::SizeVector4(
		size_t _x,
		size_t _y,
		size_t _z,
		size_t _w) :
		x(_x),
		y(_y),
		z(_z),
		w(_w)
	{
	}
	SizeVector4::SizeVector4(
		const SizeVector2& vector,
		size_t _z,
		size_t _w) :
		x(vector.x),
		y(vector.y),
		z(_z),
		w(_w)
	{
	}
	SizeVector4::SizeVector4(
		size_t _x,
		const SizeVector2& vector,
		size_t _w) :
		x(_x),
		y(vector.x),
		z(vector.y),
		w(_w)
	{
	}
	SizeVector4::SizeVector4(
		size_t _x,
		size_t _y,
		const SizeVector2& vector) :
		x(_x),
		y(_y),
		z(vector.x),
		w(vector.y)
	{
	}
	SizeVector4::SizeVector4(
		const SizeVector3& vector,
		size_t _w) :
		x(vector.x),
		y(vector.y),
		z(vector.z),
		w(_w)
	{
	}
	SizeVector4::SizeVector4(
		size_t _x,
		const SizeVector3& vector) :
		x(_x),
		y(vector.x),
		z(vector.y),
		w(vector.z)
	{
	}

	std::string SizeVector4::getString() const noexcept
	{
		auto ss = std::stringstream();
		ss << x << " " << y << " " << z << " " << w;
		return ss.str();
	}
	SizeVector2 SizeVector4::getSizeVector2() const noexcept
	{
		return SizeVector2(x, y);
	}
	SizeVector3 SizeVector4::getSizeVector3() const noexcept
	{
		return SizeVector3(x, y, z);
	}

	bool SizeVector4::operator==(const SizeVector4& vector) const noexcept
	{
		return
			x == vector.x &&
			y == vector.y &&
			z == vector.z &&
			w == vector.w;
	}
	bool SizeVector4::operator!=(const SizeVector4& vector) const noexcept
	{
		return
			x != vector.x ||
			y != vector.y ||
			z != vector.z ||
			w != vector.w;
	}

	const SizeVector4 SizeVector4::zero = SizeVector4(0);
	const SizeVector4 SizeVector4::one = SizeVector4(1);
	const SizeVector4 SizeVector4::maximum = SizeVector4(SIZE_MAX);
}
