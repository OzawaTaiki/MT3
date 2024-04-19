#pragma once
#include <Vector2.h>
#include <Vector3.h>
#include <Matrix4x4.h>


class VectorFunction
{
public:
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	static Vector3 Multiply(float scalar, const Vector3& v);
	static Vector3 Multiply(const Vector3& v1, const Vector3& v2);
	static float Dot(const Vector3& v1, const Vector3& v2);
	static Vector3 Cross(const Vector3& _v1, const Vector3& _v2);
	static float length(const Vector3& v);
	static Vector3 Normalize(const Vector3& v);
	static void VectorScreenPrintf(int x, int y, const Vector3& v);
	static void VectorScreenPrintf(int x, int y, const Vector3& v, const char* label);
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
};
