#include "VectorFunction.h"
#include <cmath>
#include <Novice.h>
#include <assert.h>

static const int kColumnWidth = 60;

Vector3 VectorFunction::Add(const Vector3& _v1, const Vector3& _v2)
{
	Vector3 result;
	result.x = _v1.x + _v2.x;
	result.y = _v1.y + _v2.y;
	result.z = _v1.z + _v2.z;

	return result;
}

Vector3 VectorFunction::Subtract(const Vector3& _v1, const Vector3& _v2)
{
	Vector3 result;
	result.x = _v1.x - _v2.x;
	result.y = _v1.y - _v2.y;
	result.z = _v1.z - _v2.z;

	return result;
}

Vector3 VectorFunction::Multiply(float _scalar, const Vector3& _v)
{
	Vector3 result;
	result.x = _v.x * _scalar;
	result.y = _v.y * _scalar;
	result.z = _v.z * _scalar;

	return result;
}

Vector3 VectorFunction::Multiply(const Vector3& _v1, const Vector3& _v2)
{
	Vector3 result;
	result.x = _v1.x * _v2.x;
	result.y = _v1.y * _v2.y;
	result.z = _v1.z * _v2.z;

	return result;
}

float VectorFunction::Dot(const Vector3& _v1, const Vector3& _v2)
{
	float result = _v1.x * _v2.x + _v1.y * _v2.y + _v1.z * _v2.z;

	return result;
}

Vector3 VectorFunction::Cross(const Vector3& _v1, const Vector3& _v2)
{
	Vector3 result;

	result.x = _v1.y * _v2.z - _v1.z * _v2.y;
	result.y = _v1.z * _v2.x - _v1.x * _v2.z;
	result.z = _v1.x * _v2.y - _v1.y * _v2.x;

	return result;
}

float VectorFunction::length(const Vector3& _v)
{
	float result;
	result = std::sqrtf(_v.x * _v.x + _v.y * _v.y + _v.z * _v.z);
	return result;
}

Vector3 VectorFunction::Normalize(const Vector3& _v)
{
	Vector3 result;
	float _length = length(_v);
	if (_length == 0)
		return { 0 };
	result.x = _v.x / _length;
	result.y = _v.y / _length;
	result.z = _v.z / _length;

	return result;
}

void VectorFunction::VectorScreenPrintf(int _x, int _y, const Vector3& _v)
{
	Novice::ScreenPrintf(_x, _y, "%.02f", _v.x);
	Novice::ScreenPrintf(_x + kColumnWidth, _y, "%.02f", _v.y);
	Novice::ScreenPrintf(_x + kColumnWidth * 2, _y, "%.02f", _v.z);
}

void VectorFunction::VectorScreenPrintf(int _x, int _y, const Vector3& _v, const char* _label)
{
	Novice::ScreenPrintf(_x, _y, "%.02f", _v.x);
	Novice::ScreenPrintf(_x + kColumnWidth, _y, "%.02f", _v.y);
	Novice::ScreenPrintf(_x + kColumnWidth * 2, _y, "%.02f", _v.z);
	Novice::ScreenPrintf(_x + kColumnWidth * 3, _y, "%s", _label);
}

Vector3 VectorFunction::Transform(const Vector3& _vector, const Matrix4x4& _matrix)
{
	Vector3 result;

	result.x = _vector.x * _matrix.m[0][0] + _vector.y * _matrix.m[1][0] + _vector.z * _matrix.m[2][0] + 1.0f * _matrix.m[3][0];
	result.y = _vector.x * _matrix.m[0][1] + _vector.y * _matrix.m[1][1] + _vector.z * _matrix.m[2][1] + 1.0f * _matrix.m[3][1];
	result.z = _vector.x * _matrix.m[0][2] + _vector.y * _matrix.m[1][2] + _vector.z * _matrix.m[2][2] + 1.0f * _matrix.m[3][2];
	float _w = _vector.x * _matrix.m[0][3] + _vector.y * _matrix.m[1][3] + _vector.z * _matrix.m[2][3] + 1.0f * _matrix.m[3][3];
	assert(_w != 0.0f);
	result.x /= _w;
	result.y /= _w;
	result.z /= _w;
	return result;
}

Vector3 VectorFunction::Lerp(const Vector3& _vector1, const Vector3& _vector2, float _t)
{
	Vector3 result;
	result = _vector1 * (1.0f - _t) + _vector2 * _t;

	return result;
}

Vector3 operator+(const Vector3& _v1, const Vector3& _v2)
{
	return Vector3(_v1.x + _v2.x, _v1.y + _v2.y, _v1.z + _v2.z);
}

Vector3 operator-(const Vector3& _v1, const Vector3& _v2)
{
	return Vector3(_v1.x - _v2.x, _v1.y - _v2.y, _v1.z - _v2.z);
}

Vector3 operator*(const Vector3& _v1, const Vector3& _v2)
{
	return Vector3(_v1.x * _v2.x, _v1.y * _v2.y, _v1.z * _v2.z);
}

Vector3 operator/(const Vector3& _v1, const Vector3& _v2)
{
	return Vector3(_v1.x / _v2.x, _v1.y / _v2.y, _v1.z / _v2.z);
}

Vector3 operator*(const Vector3& _v, float _s)
{
	return Vector3(_v.x * _s, _v.y * _s, _v.z * _s);
}

Vector3 operator-(const Vector3& _v)
{
	return Vector3(-_v.x, -_v.y, -_v.z);
}
