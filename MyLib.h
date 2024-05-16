#pragma once

#include "MatrixFunction.h"
#include "VectorFunction.h"
#define _USE_MATH_DEFINES
#include<cmath>
#include <algorithm>

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

//法線に垂直なVectorを求める
Vector3 Perpendicular(const Vector3& _v);

//球体同士の衝突判定
bool IsCollision(const Sphere& _s1, const Sphere& _s2);

//球と平面の衝突判定
bool IsCollision(const Sphere& _s, const Plane& _p);

//平面と直線の衝突判定
bool IsCollision(const Plane& _plane, const Line& _line);

//平面と半直線の衝突判定
bool IsCollision(const Plane& _plane, const Ray& _ray);

//平面と線分の衝突判定
bool IsCollision(const Plane& _plane, const Segment& _segment);
