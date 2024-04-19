#pragma once

#include "MatrixFunction.h"
#include "VectorFunction.h"
#define _USE_MATH_DEFINES
#include<cmath>

struct Sphere
{
	Vector3 center; //中心点
	float radius;   //半径
};
//直線
struct Line
{
	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル
};
//半直線
struct Ray
{
	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル
};
//線分
struct Segment
{
	Vector3 origin; // 始点
	Vector3 diff;   // 終点への差分ベクトル
};
//平面
struct Plane
{
	Vector3 normal; // 法線
	float distance; // 距離
};

//グリッドの描画
void DrawGrid(const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix);
//球体の描画
void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);
//直線の描画
void Drawline(const Vector3& _origin, const Vector3& _diff, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);

Vector3 Project(const Vector3& _v1, const Vector3& _v2);

Vector3 ClosestPoint(const Vector3& _point, const Segment& _segment);
//球体同士の衝突判定
bool IsCollision(const Sphere& _s1, const Sphere& _s2);
