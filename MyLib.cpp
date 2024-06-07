#define NOMINMAX
#include "MyLib.h"
#include "Novice.h"
#include <algorithm>

#include <imgui.h>

void DrawGrid(const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix)
{
	const float kGridHalfWidth = 2.0f;                                          // Gridの半分の幅
	const uint32_t kSubdivision = 10;                                           // 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);     // １つ分の長さ

	// 奥から手前への線を引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex)
	{
		// 上の情報を使ってワールド座標系状の始点と終点を求める
		float x = kGridHalfWidth - xIndex * kGridEvery;
		Vector3 startPos = { -kGridHalfWidth,0,x };
		Vector3 endPos = { kGridHalfWidth,0,x };

		// スクリーン座標系まで変換をかける
		Vector3 temp = VectorFunction::Transform(startPos, _viewProjectionMatrix);
		startPos = VectorFunction::Transform(temp, _viewportMatrix);

		temp = VectorFunction::Transform(endPos, _viewProjectionMatrix);
		endPos = VectorFunction::Transform(temp, _viewportMatrix);

		// 変換した座標を使って表示。
		Novice::DrawLine((int)startPos.x, (int)startPos.y, (int)endPos.x, (int)endPos.y, xIndex == (kSubdivision / 2) ? 0xff : 0xaaaaaaff);
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex)
	{
		// 上の情報を使ってワールド座標系状の始点と終点を求める
		float z = kGridHalfWidth - zIndex * kGridEvery;
		Vector3 startPos = { z,0,-kGridHalfWidth };
		Vector3 endPos = { z,0,kGridHalfWidth };

		// スクリーン座標系まで変換をかける
		Vector3 temp = VectorFunction::Transform(startPos, _viewProjectionMatrix);
		startPos = VectorFunction::Transform(temp, _viewportMatrix);


		temp = VectorFunction::Transform(endPos, _viewProjectionMatrix);
		endPos = VectorFunction::Transform(temp, _viewportMatrix);

		// 変換した座標を使って表示。
		Novice::DrawLine((int)startPos.x, (int)startPos.y, (int)endPos.x, (int)endPos.y, zIndex == (kSubdivision / 2) ? 0xff : 0xaaaaaaff);
	}
}


void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
	const uint32_t kSubdivision = 10;                                   // 分割数
	const float kLatEvery = (float)M_PI / (float)kSubdivision;          // 緯度分割１つ分の角度
	const float kLonEvery = (float)M_PI * 2.0 / (float)kSubdivision;    // 経度分割１つ分の角度

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
				Vector3 temp = VectorFunction::Transform(point[i], _viewProjectionMatrix);
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

	Vector3 temp = VectorFunction::Transform(start, _viewProjectionMatrix);
	start = VectorFunction::Transform(temp, _viewportMatrix);


	temp = VectorFunction::Transform(end, _viewProjectionMatrix);
	end = VectorFunction::Transform(temp, _viewportMatrix);

	Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, _color);

}

void DrawPlane(const Plane& _plane, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
	Vector3 center = _plane.normal * _plane.distance; // 1
	Vector3 perpendiculars[4];
	perpendiculars[0] = VectorFunction::Normalize(Perpendicular(_plane.normal)); // 2
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z }; // 3
	perpendiculars[2] = VectorFunction::Cross(_plane.normal, perpendiculars[0]); // 4
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z }; // 5
	// 6
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = VectorFunction::Multiply(2.0f, perpendiculars[index]);
		Vector3 point = center + extend;
		points[index] = VectorFunction::Transform(VectorFunction::Transform(point, _viewProjectionMatrix), _viewportMatrix);
	}
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[2].x, (int)points[2].y, _color);
	Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[3].x, (int)points[3].y, _color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[2].x, (int)points[2].y, _color);
	Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[3].x, (int)points[3].y, _color);

}

void DrawTriangle(const Triangle& _triangle, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
	Vector3 vertices[3];

	for (int i = 0; i < 3; i++)
	{
		vertices[i] = VectorFunction::Transform(VectorFunction::Transform(_triangle.vertices[i], _viewProjectionMatrix), _viewportMatrix);
	}
	Novice::DrawTriangle(int(vertices[0].x), int(vertices[0].y),
		int(vertices[1].x), int(vertices[1].y),
		int(vertices[2].x), int(vertices[2].y),
		_color, kFillModeWireFrame);
}


void DrawAABB(const AABB& _aabb, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
	Vector3 vertices[8];

	vertices[0] = { _aabb.min.x,_aabb.min.y ,_aabb.min.z };
	vertices[1] = { _aabb.max.x,_aabb.min.y ,_aabb.min.z };
	vertices[2] = { _aabb.min.x,_aabb.max.y ,_aabb.min.z };
	vertices[3] = { _aabb.max.x,_aabb.max.y ,_aabb.min.z };
	vertices[4] = { _aabb.min.x,_aabb.min.y ,_aabb.max.z };
	vertices[5] = { _aabb.max.x,_aabb.min.y ,_aabb.max.z };
	vertices[6] = { _aabb.min.x,_aabb.max.y ,_aabb.max.z };
	vertices[7] = { _aabb.max.x,_aabb.max.y ,_aabb.max.z };

	for (Vector3& v : vertices)
	{
		v = VectorFunction::Transform(VectorFunction::Transform(v, _viewProjectionMatrix), _viewportMatrix);
	}

	Novice::DrawLine((int)vertices[0].x, (int)vertices[0].y, (int)vertices[1].x, (int)vertices[1].y, _color);
	Novice::DrawLine((int)vertices[0].x, (int)vertices[0].y, (int)vertices[2].x, (int)vertices[2].y, _color);
	Novice::DrawLine((int)vertices[0].x, (int)vertices[0].y, (int)vertices[4].x, (int)vertices[4].y, _color);
	Novice::DrawLine((int)vertices[1].x, (int)vertices[1].y, (int)vertices[3].x, (int)vertices[3].y, _color);
	Novice::DrawLine((int)vertices[1].x, (int)vertices[1].y, (int)vertices[5].x, (int)vertices[5].y, _color);
	Novice::DrawLine((int)vertices[2].x, (int)vertices[2].y, (int)vertices[6].x, (int)vertices[6].y, _color);
	Novice::DrawLine((int)vertices[2].x, (int)vertices[2].y, (int)vertices[3].x, (int)vertices[3].y, _color);
	Novice::DrawLine((int)vertices[3].x, (int)vertices[3].y, (int)vertices[7].x, (int)vertices[7].y, _color);
	Novice::DrawLine((int)vertices[4].x, (int)vertices[4].y, (int)vertices[5].x, (int)vertices[5].y, _color);
	Novice::DrawLine((int)vertices[4].x, (int)vertices[4].y, (int)vertices[6].x, (int)vertices[6].y, _color);
	Novice::DrawLine((int)vertices[5].x, (int)vertices[5].y, (int)vertices[7].x, (int)vertices[7].y, _color);
	Novice::DrawLine((int)vertices[6].x, (int)vertices[6].y, (int)vertices[7].x, (int)vertices[7].y, _color);

}

void DrawOBB(const OBB& _obb, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color)
{
	Vector3 vertices[8];

	_obb.CaluculateVertices(vertices);

	for (Vector3& v : vertices)
	{
		v = VectorFunction::Transform(VectorFunction::Transform(v, _viewProjectionMatrix), _viewportMatrix);
	}

	Novice::DrawLine((int)vertices[0].x, (int)vertices[0].y, (int)vertices[1].x, (int)vertices[1].y, _color);
	Novice::DrawLine((int)vertices[0].x, (int)vertices[0].y, (int)vertices[2].x, (int)vertices[2].y, _color);
	Novice::DrawLine((int)vertices[0].x, (int)vertices[0].y, (int)vertices[4].x, (int)vertices[4].y, _color);
	Novice::DrawLine((int)vertices[1].x, (int)vertices[1].y, (int)vertices[3].x, (int)vertices[3].y, _color);
	Novice::DrawLine((int)vertices[1].x, (int)vertices[1].y, (int)vertices[5].x, (int)vertices[5].y, _color);
	Novice::DrawLine((int)vertices[2].x, (int)vertices[2].y, (int)vertices[6].x, (int)vertices[6].y, _color);
	Novice::DrawLine((int)vertices[2].x, (int)vertices[2].y, (int)vertices[3].x, (int)vertices[3].y, _color);
	Novice::DrawLine((int)vertices[3].x, (int)vertices[3].y, (int)vertices[7].x, (int)vertices[7].y, _color);
	Novice::DrawLine((int)vertices[4].x, (int)vertices[4].y, (int)vertices[5].x, (int)vertices[5].y, _color);
	Novice::DrawLine((int)vertices[4].x, (int)vertices[4].y, (int)vertices[6].x, (int)vertices[6].y, _color);
	Novice::DrawLine((int)vertices[5].x, (int)vertices[5].y, (int)vertices[7].x, (int)vertices[7].y, _color);
	Novice::DrawLine((int)vertices[6].x, (int)vertices[6].y, (int)vertices[7].x, (int)vertices[7].y, _color);
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

Vector3 Perpendicular(const Vector3& _v)
{
	if (_v.x != 0.0f || _v.y != 0.0f)
	{
		return { -_v.y,_v.x,0.0f };
	}
	return { 0.0f, -_v.z,_v.y };
}

void GetPlaneVertex(const Plane& _plane, Vector3* _vertex)
{
	Vector3 center = _plane.normal * _plane.distance;

	_vertex[0] = VectorFunction::Normalize(Perpendicular(_plane.normal));
	_vertex[1] = { -_vertex[0].x, -_vertex[0].y, -_vertex[0].z };
	_vertex[2] = VectorFunction::Cross(_plane.normal, _vertex[0]);
	_vertex[3] = { -_vertex[2].x, -_vertex[2].y, -_vertex[2].z };

	for (int i = 0; i < 4; i++)
		_vertex[i] = _vertex[i] * _plane.scalar + center;
}

bool IsCollision(const Sphere& _s1, const Sphere& _s2)
{
	float distance = VectorFunction::length(VectorFunction::Subtract(_s1.center, _s2.center));
	return distance <= _s1.radius + _s2.radius ? true : false;
}

bool IsCollision(const Sphere& _s, const Plane& _p)
{
	float distance = VectorFunction::Dot(_p.normal, _s.center) - _p.distance;
	distance = distance < 0 ? -distance : distance;

	if (distance <= _s.radius)
		return true;
	return false;
}

bool IsCollision(const Plane& _plane, const Segment& _segment)
{
	float dot = VectorFunction::Dot(_plane.normal, _segment.diff);

	if (dot == 0.0f)
	{
		return false;
	}

	float t = (_plane.distance - VectorFunction::Dot(_segment.origin, _plane.normal)) / dot;

	if (t < 0.0f || t>1.0f)
		return false;

	return true;
}

bool IsCollision(const Triangle& _triangle, const Segment& _segment)
{
	Plane lPlane = CalculatePlane(_triangle);

	float dot = VectorFunction::Dot(lPlane.normal, _segment.diff);

	if (dot == 0.0f)
	{
		return false;
	}

	float t = (lPlane.distance - VectorFunction::Dot(_segment.origin, lPlane.normal)) / dot;

	if (t < 0.0f || t>1.0f)
		return false;

	Vector3 point = _segment.origin + _segment.diff * t;

	Vector3 v01 = _triangle.vertices[1] - _triangle.vertices[0];
	Vector3 v1p = point - _triangle.vertices[1];

	Vector3 v12 = _triangle.vertices[2] - _triangle.vertices[1];
	Vector3 v2p = point - _triangle.vertices[2];

	Vector3 v20 = _triangle.vertices[0] - _triangle.vertices[2];
	Vector3 v0p = point - _triangle.vertices[0];

	Vector3 cross01 = VectorFunction::Cross(v01, v1p);
	Vector3 cross12 = VectorFunction::Cross(v12, v2p);
	Vector3 cross20 = VectorFunction::Cross(v20, v0p);

	if (VectorFunction::Dot(cross01, lPlane.normal) >= 0.0f &&
		VectorFunction::Dot(cross12, lPlane.normal) >= 0.0f &&
		VectorFunction::Dot(cross20, lPlane.normal) >= 0.0f)
	{
		return true;
	}
	return false;

}

bool IsCollision(const AABB& _a, const AABB& _b)
{
	if ((_a.min.x <= _b.max.x && _a.max.x >= _b.min.x) && // x
		(_a.min.y <= _b.max.y && _a.max.y >= _b.min.y) && // y
		(_a.min.z <= _b.max.z && _a.max.z >= _b.min.z)) { // z
		//衝突
		return true;
	}

	return false;
}

bool IsCollision(const AABB& _a, const Sphere& _s)
{

	Vector3 closestPoint;
	closestPoint.x = std::clamp(_s.center.x, _a.min.x, _a.max.x);
	closestPoint.y = std::clamp(_s.center.y, _a.min.y, _a.max.y);
	closestPoint.z = std::clamp(_s.center.z, _a.min.z, _a.max.z);

	float distance = VectorFunction::length(closestPoint - _s.center);

	if (distance <= _s.radius)
	{
		return true;
	}

	return false;
}

bool IsCollision(const AABB& _aabb, const Segment& _segment)
{
	Vector3 tminVec;
	Vector3 tmaxVec;

	tminVec.y = (_aabb.min.y - _segment.origin.y) / _segment.diff.y;
	tminVec.x = (_aabb.min.x - _segment.origin.x) / _segment.diff.x;
	tminVec.z = (_aabb.min.z - _segment.origin.z) / _segment.diff.z;

	tmaxVec.x = (_aabb.max.x - _segment.origin.x) / _segment.diff.x;
	tmaxVec.y = (_aabb.max.y - _segment.origin.y) / _segment.diff.y;
	tmaxVec.z = (_aabb.max.z - _segment.origin.z) / _segment.diff.z;

	Vector3 tNear;
	tNear.x = std::min(tminVec.x, tmaxVec.x);
	tNear.y = std::min(tminVec.y, tmaxVec.y);
	tNear.z = std::min(tminVec.z, tmaxVec.z);

	Vector3 tFar;
	tFar.x = std::max(tminVec.x, tmaxVec.x);
	tFar.y = std::max(tminVec.y, tmaxVec.y);
	tFar.z = std::max(tminVec.z, tmaxVec.z);

	float tmin = std::max(std::max(tNear.x, tNear.y), tNear.z);
	float tmax = std::min(std::min(tFar.x, tFar.y), tFar.z);

#ifdef _DEBUG

	ImGui::Begin("aaa");
	ImGui::Text("tmin : %.3f", tmin);
	ImGui::Text("tmax ; %.3f", tmax);
	ImGui::End();

#endif // _DEBUG

	if (tmin <= tmax &&
		tmax >= 0.0f &&
		tmin <= 1.0f)
	{
		//衝突
		return true;
	}

	return false;
}

bool IsCollision(const OBB& _obb, const Sphere& _sphere)
{
	Matrix4x4 obbWolrdMat = MatrixFunction::MakeAffineMatrix({ 1.0f,1.0f ,1.0f }, _obb.rotate, _obb.center);
	Matrix4x4 obbWorldMatInv = MatrixFunction::Inverse(obbWolrdMat);

	Vector3  centerInOBBLocalSphere = VectorFunction::Transform(_sphere.center, obbWorldMatInv);
	AABB aabbOBBLocal{ .min = -_obb.size,.max = _obb.size };
	Sphere sphereOBBLocal{ centerInOBBLocalSphere,_sphere.radius };

	return IsCollision(aabbOBBLocal, sphereOBBLocal);

}

bool IsCollision(const OBB& _obb, const Segment& _segment)
{
	Matrix4x4 obbWolrdMat = MatrixFunction::MakeAffineMatrix({ 1.0f,1.0f ,1.0f }, _obb.rotate, _obb.center);
	Matrix4x4 obbWorldMatInv = MatrixFunction::Inverse(obbWolrdMat);
	Vector3 localOrigin = VectorFunction::Transform(_segment.origin, obbWorldMatInv);
	Vector3 localEnd = VectorFunction::Transform(_segment.origin + _segment.diff, obbWorldMatInv);

	AABB localAABB{
		{-_obb.size.x, -_obb.size.y, -_obb.size.z},
		{+_obb.size.x, +_obb.size.y, +_obb.size.z},
	};
	Segment localSegment;
	localSegment.origin = localOrigin;
	localSegment.diff = localEnd - localOrigin;

	return IsCollision(localAABB, localSegment);
}

bool IsCollision(const OBB& _obb1, const OBB& _obb2)
{
	/// 分離軸候補の計算
	Vector3 axis[15];
	axis[0] = _obb1.orientations[0];
	axis[1] = _obb1.orientations[1];
	axis[2] = _obb1.orientations[2];

	axis[3] = _obb2.orientations[0];
	axis[4] = _obb2.orientations[1];
	axis[5] = _obb2.orientations[2];

	int index = 6;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			axis[index++] = VectorFunction::Cross(_obb1.orientations[i], _obb2.orientations[j]);
		}
	}




	return false;
}

Plane CalculatePlane(const Triangle& _triangle)
{
	Plane result{};

	//ab,bcを求める
	Vector3 edge1 = _triangle.vertices[1] - _triangle.vertices[0];
	Vector3 edge2 = _triangle.vertices[2] - _triangle.vertices[1];

	//法線計算
	result.normal = VectorFunction::Cross(edge1, edge2);
	result.normal = VectorFunction::Normalize(result.normal);

	//平面方程式を用いて距離を求める
	result.distance = VectorFunction::Dot(_triangle.vertices[0], result.normal);

	return result;
}

void AABB::Update()
{
	this->min.x = (std::min)(this->min.x, this->max.x);
	this->max.x = (std::max)(this->min.x, this->max.x);
	this->min.y = (std::min)(this->min.y, this->max.y);
	this->max.y = (std::max)(this->min.y, this->max.y);
	this->min.z = (std::min)(this->min.z, this->max.z);
	this->max.z = (std::max)(this->min.z, this->max.z);
}

void OBB::CalculateOrientations()
{
	Matrix4x4 rotateMatrix = MatrixFunction::MakeRotateMatrix(this->rotate);


	this->orientations[0].x = rotateMatrix.m[0][0];
	this->orientations[0].y = rotateMatrix.m[0][1];
	this->orientations[0].z = rotateMatrix.m[0][2];

	this->orientations[1].x = rotateMatrix.m[1][0];
	this->orientations[1].y = rotateMatrix.m[1][1];
	this->orientations[1].z = rotateMatrix.m[1][2];

	this->orientations[2].x = rotateMatrix.m[2][0];
	this->orientations[2].y = rotateMatrix.m[2][1];
	this->orientations[2].z = rotateMatrix.m[2][2];

	this->orientations[0] = VectorFunction::Normalize(this->orientations[0]);
	this->orientations[1] = VectorFunction::Normalize(this->orientations[1]);
	this->orientations[2] = VectorFunction::Normalize(this->orientations[2]);
}

void OBB::CaluculateVertices(Vector3* vertices) const
{
	Vector3 rotateAxis[3];
	rotateAxis[0] = this->orientations[0] * this->size.x;
	rotateAxis[1] = this->orientations[1] * this->size.y;
	rotateAxis[2] = this->orientations[2] * this->size.z;

	vertices[0] = this->center + rotateAxis[0] + rotateAxis[1] + rotateAxis[2];
	vertices[1] = this->center + rotateAxis[0] + rotateAxis[1] - rotateAxis[2];
	vertices[2] = this->center + rotateAxis[0] - rotateAxis[1] + rotateAxis[2];
	vertices[3] = this->center + rotateAxis[0] - rotateAxis[1] - rotateAxis[2];
	vertices[4] = this->center - rotateAxis[0] + rotateAxis[1] + rotateAxis[2];
	vertices[5] = this->center - rotateAxis[0] + rotateAxis[1] - rotateAxis[2];
	vertices[6] = this->center - rotateAxis[0] - rotateAxis[1] + rotateAxis[2];
	vertices[7] = this->center - rotateAxis[0] - rotateAxis[1] - rotateAxis[2];
}

