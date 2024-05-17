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

	/// <summary>
	/// ２つのベクトルの内積を求める
	/// </summary>
	/// <param name="v1">ベクトル１</param>
	/// <param name="v2">ベクトル２</param>
	/// <returns>２つのベクトルの内積計算結果</returns>
	static float Dot(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// ２つのベクトルの外積を求める
	/// </summary>
	/// <param name="_v1">ベクトル１</param>
	/// <param name="_v2">ベクトル２</param>
	/// <returns>２つのベクトルの外積計算結果</returns>
	static Vector3 Cross(const Vector3& _v1, const Vector3& _v2);

	/// <summary>
	/// ベクトルの長さを求める
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>ベクトルの長さ</returns>
	static float length(const Vector3& v);
	static Vector3 Normalize(const Vector3& v);
	static void VectorScreenPrintf(int x, int y, const Vector3& v);
	static void VectorScreenPrintf(int x, int y, const Vector3& v, const char* label);
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
};

Vector3 operator+(const Vector3& _v1, const Vector3& _v2);
Vector3 operator-(const Vector3& _v1, const Vector3& _v2);
Vector3 operator*(const Vector3& _v1, const Vector3& _v2);
Vector3 operator/(const Vector3& _v1, const Vector3& _v2);
Vector3 operator*(const Vector3& _v, const float _s);
