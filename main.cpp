#include <Novice.h>
#include "VectorFunction.h"
#include "MatrixFunction.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2A_07_オザワ_タイキ_MT3_01_02";

struct Sphere
{
	Vector3 center; //中心点
	float radius;   //半径
};

void DrawGrid(const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix);
void DrawSphere(const Sphere& _sphere, const Matrix4x4& _viewProjectionMatrix, const Matrix4x4& _viewportMatrix, uint32_t _color);

static const int kWindowWidth = 1280;
static const int kWindowHeght = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	Sphere sphere{ {0,0,0},0.5f };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		ImGui::Begin("Window");
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		//ImGui::InputFloat3("cameraTranslate", &cameraTranslate.x, "0.01f", ImGuiInputTextFlags_ReadOnly);
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);
		ImGui::End();

		Matrix4x4 cameraMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = MatrixFunction::Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MatrixFunction::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeght), 0.1f, 100.0f);
		Matrix4x4 viewportMatrix = MatrixFunction::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeght), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(MatrixFunction::Multiply(viewMatrix, projectionMatrix), viewportMatrix);
		DrawSphere(sphere, MatrixFunction::Multiply(viewMatrix, projectionMatrix), viewportMatrix, BLACK);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}


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

