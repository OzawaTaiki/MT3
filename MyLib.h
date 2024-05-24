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
	float scalar = 2;  // 
};

//三角形
struct Triangle
{
	Vector3 vertices[3];//頂点
};
//矩形？
struct AABB
{
	Vector3 min;
	Vector3 max;
	void Update();
};
//グリッドの描画
void DrawGrid(const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix);
//球体の描画
void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);
//直線の描画
void Drawline(const Vector3& _origin, const Vector3& _diff, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);

void DrawPlane(const Plane& _plane, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);

void DrawTriangle(const Triangle& _triangle, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);


void DrawAABB(const AABB& _aabb, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);

Vector3 Project(const Vector3& _v1, const Vector3& _v2);

Vector3 ClosestPoint(const Vector3& _point, const Segment& _segment);

//法線に垂直なVectorを求める
Vector3 Perpendicular(const Vector3& _v);

/// <summary>
/// 平面の四点を求める
/// </summary>
/// <param name="_plane">４点を求める平面</param>
/// <param name="_vertex">計算結果を格納するvevtor3[4] 0-2-1-3</param>
void GetPlaneVertex(const Plane& _plane, Vector3* _vertex);

//球体同士の衝突判定
bool IsCollision(const Sphere& _s1, const Sphere& _s2);


bool IsCollision(const Sphere& _s, const Plane& _p);

//平面と線分の衝突判定
bool IsCollision(const Plane& _plane, const Segment& _segment);

//三角形と線分の衝突判定
bool IsCollision(const Triangle& _triangle, const Segment& _segment);

//AABB同士の衝突判定
bool IsCollision(const AABB& _a, const AABB& _b);

//三角形のある平面を計算
Plane CalculatePlane(const Triangle& _triangle);
