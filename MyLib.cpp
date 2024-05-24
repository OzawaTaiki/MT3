#include "MyLib.h"
#include "Novice.h"

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
		//Matrix4x4 worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, startPos);
		//Matrix4x4 worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

		Vector3 temp = VectorFunction::Transform(startPos, _viewProjectionMatrix);
		startPos = VectorFunction::Transform(temp, _viewportMatrix);

		//worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, endPos);
		//worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

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
	//	Matrix4x4 worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, startPos);
		//Matrix4x4 worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

		Vector3 temp = VectorFunction::Transform(startPos, _viewProjectionMatrix);
		startPos = VectorFunction::Transform(temp, _viewportMatrix);

		//worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, endPos);
		//worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, _viewProjectionMatrix);

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
				//Matrix4x4 worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, { 0,0,0 }, point[i]);
				//Matrix4x4 worldViewprojectionMatrix = MatrixFunction::Multiply(worldMatrix, );

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

void AABB::Update()
{
	this->min.x = (std::min)(this->min.x, this->max.x);
	this->max.x = (std::max)(this->min.x, this->max.x);
	this->min.y = (std::min)(this->min.y, this->max.y);
	this->max.y = (std::max)(this->min.y, this->max.y);
	this->min.z = (std::min)(this->min.z, this->max.z);
	this->max.z = (std::max)(this->min.z, this->max.z);
}
