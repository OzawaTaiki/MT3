﻿#include "MyLib.h"
#include "Novice.h"

void DrawGrid(const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix)
{
	const float kGridHalfWidth = 1.0f;                                          // Gridの半分の幅
	const uint32_t kSubdivision = 10;                                           // 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);     // １つ分の長さ

	// 置くから手前への線を引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; xIndex++)
	{
		// 上の情報を使ってワールド座標系状の始点と終点を求める
		float x = -kGridHalfWidth + xIndex * kGridEvery;
		Vector3 startPos = { x,0,-kGridHalfWidth };
		Vector3 endPos = { x,0,kGridHalfWidth };
		// スクリーン座標系まで変換をかける
		Matrix4x4 worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, startPos);
		Matrix4x4 worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

		Vector3 temp = VectorFunction::Transform(startPos, worldViewprojectionMatrix);
		startPos = VectorFunction::Transform(temp, _viewportMatrix);

		worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, endPos);
		worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

		temp = VectorFunction::Transform(endPos, worldViewprojectionMatrix);
		endPos = VectorFunction::Transform(temp, _viewportMatrix);

		// 変換した座標を使って表示。
		Novice::DrawLine((int)startPos.x, (int)startPos.y, (int)endPos.x, (int)endPos.y, xIndex == (kSubdivision / 2) ? 0xff : 0xaaaaaaff);
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; zIndex++)
	{
		// 上の情報を使ってワールド座標系状の始点と終点を求める
		float z = -kGridHalfWidth + zIndex * kGridEvery;
		Vector3 startPos = { -kGridHalfWidth,0,z };
		Vector3 endPos = { kGridHalfWidth,0,z };
		// スクリーン座標系まで変換をかける
		Matrix4x4 worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, startPos);
		Matrix4x4 worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

		Vector3 temp = VectorFunction::Transform(startPos, worldViewprojectionMatrix);
		startPos = VectorFunction::Transform(temp, _viewportMatrix);

		worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, endPos);
		worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

		temp = VectorFunction::Transform(endPos, worldViewprojectionMatrix);
		endPos = VectorFunction::Transform(temp, _viewportMatrix);

		// 変換した座標を使って表示。
		Novice::DrawLine((int)startPos.x, (int)startPos.y, (int)endPos.x, (int)endPos.y, zIndex == (kSubdivision / 2) ? 0xff : 0xaaaaaaff);
	}
}


void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
	const uint32_t kSubdivision = 10;                                   // 分割数
	const float kLatEvery = (float)M_PI / (float)kSubdivision;          // 緯度分割１つ分の角度
	const float kLonEvery = (float)M_PI * 2.0f / (float)kSubdivision;   // 経度分割１つ分の角度

	//緯度の方向に分割   -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++)
	{
		float lat = -(float)M_PI / 2.0f + kLatEvery * latIndex;         // 現在の緯度

		// 経度の方向に分割   0 ~ π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++)
		{
			float lon = lonIndex * kLonEvery;                           // 現在の経度
			// world座標系でのa,b,cを求める
			Vector3 point[3] = {
				{
					std::cosf(lat) * std::cosf(lon),
					std::sinf(lat),
					std::cosf(lat) * std::sinf(lon)
				},
				{
					std::cosf(lat + kLatEvery) * std::cosf(lon),
					std::sinf(lat + kLatEvery),
					std::cosf(lat + kLatEvery) * std::sinf(lon)
				},
				{
					std::cosf(lat) * std::cosf(lon + kLonEvery),
					std::sinf(lat),
					std::cosf(lat) * std::sinf(lon + kLonEvery)
				}
			};

			for (int i = 0; i < 3; i++)
				point[i] = VectorFunction::Add(_sphere.center, VectorFunction::Multiply(_sphere.radius, point[i]));

			// a,b,cをScreen座標系まで変換
			Vector3 drawPoint[3];
			for (int i = 0; i < 3; i++)
			{
				Matrix4x4 worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, point[i]);
				Matrix4x4 worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

				Vector3 temp = VectorFunction::Transform(point[i], worldViewprojectionMatrix);
				drawPoint[i] = VectorFunction::Transform(temp, _viewportMatrix);
			}

			// ab,acで線を引く
			Novice::DrawLine((int)drawPoint[0].x, (int)drawPoint[0].y, (int)drawPoint[1].x, (int)drawPoint[1].y, _color);
			Novice::DrawLine((int)drawPoint[0].x, (int)drawPoint[0].y, (int)drawPoint[2].x, (int)drawPoint[2].y, _color);

		}
	}
}

void Drawline(const Vector3& _origin, const Vector3& _diff, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
	Vector3 start = _origin;
	Vector3 end = VectorFunction::Add(_origin, _diff);

	Matrix4x4 worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, start);
	Matrix4x4 worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

	Vector3 temp = VectorFunction::Transform(start, worldViewprojectionMatrix);
	start = VectorFunction::Transform(temp, _viewportMatrix);

	worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, end);
	worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

	temp = VectorFunction::Transform(end, worldViewprojectionMatrix);
	end = VectorFunction::Transform(temp, _viewportMatrix);

	Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, _color);

}

Vector3 Project(const Vector3& _v1, const Vector3& _v2)
{
	Vector3 normalize = VectorFunction::Normalize(_v2);

	Vector3 result = VectorFunction::Multiply(VectorFunction::Dot(_v1, normalize), normalize);

	return result;
}

Vector3 ClosestPoint(const Vector3& _point, const Segment& _segment)
{
	Vector3 cp = VectorFunction::Add(_segment.origin, Project(VectorFunction::Subtract(_point, _segment.origin), _segment.diff));

	return cp;
}

bool IsCollision(const Sphere& _s1, const Sphere& _s2)
{
	float distance = VectorFunction::length(VectorFunction::Subtract(_s1.center, _s2.center));
	return distance <= _s1.radius + _s2.radius ? true : false;
}
