#pragma once
#include <Vector2.h>
#include <Vector3.h>
#include <Matrix4x4.h>


class VectorFunction
{
public:
	static Vector3 Add(const Vector3& _v1, const Vector3& _v2);
	static Vector3 Subtract(const Vector3& _v1, const Vector3& _v2);
	static Vector3 Multiply(float _scalar, const Vector3& _v);
	static Vector3 Multiply(const Vector3& _v1, const Vector3& _v2);
	static float Dot(const Vector3& _v1, const Vector3& _v2);
	static Vector3 Cross(const Vector3& _v1, const Vector3& _v2);
	static float length(const Vector3& _v);
	static Vector3 Normalize(const Vector3& _v);
	static void VectorScreenPrintf(int _x, int _y, const Vector3& _v);
	static void VectorScreenPrintf(int _x, int _y, const Vector3& _v, const char* _label);
	static Vector3 Transform(const Vector3& _vector, const Matrix4x4& _matrix);
};

Vector3 operator+(const Vector3& _v1, const Vector3& _v2);
Vector3 operator-(const Vector3& _v1, const Vector3& _v2);
Vector3 operator*(const Vector3& _v1, const Vector3& _v2);
Vector3 operator/(const Vector3& _v1, const Vector3& _v2);
Vector3 operator*(const Vector3& _v, float _s);
