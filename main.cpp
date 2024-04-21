#include <Novice.h>
#include "MyLib.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2A_07_オザワ_タイキ_MT3_01_01_advance";

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

//法線を求める
Vector3 calculateNormal(const Vector3 _vertex[]);

Vector3 calculateCameraToTriangleCenter(const Vector3& _cameraPos, const Vector3 _triangleVertex[]);

bool performBackfaceCulling(const Vector3& _cameraPos, const Vector3 _triangleVertex[]);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Camera* camera = new Camera(kWindowWidth, kWindowHeight);


	Vector3 rotate = { 0,0,0 };
	Vector3 translate = { 0,0,1.0f };
	Vector3 kLocalVertices[3] = {
		{0,1.0f / 1.732f,0},
		{0.5774f,-0.5f / 1.732f,0},
		{-0.5774f,-0.5f / 1.732f,0}
	};


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		camera->Update(keys, preKeys);

		///
		/// ↓更新処理ここから
		///

		rotate.y += 0.05f;


		Matrix4x4 worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, rotate, translate);
		Matrix4x4 worldViewprojectioonMatrix = MatrixFunction::Multiply(worldMatrix, camera->GetviewProjectionMatrix());
		Vector3 screenVertices[3];
		for (int i = 0; i < 3; i++)
		{
			Vector3 ndcVertex = VectorFunction::Transform(kLocalVertices[i], worldViewprojectioonMatrix);
			screenVertices[i] = VectorFunction::Transform(ndcVertex, camera->GetViewportMatrix());
		}

		bool isCull = performBackfaceCulling(camera->GetTranslate(), screenVertices);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

		Novice::DrawTriangle(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), int(screenVertices[2].x), int(screenVertices[2].y), RED, isCull ? kFillModeWireFrame : kFillModeSolid);

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

	delete camera;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

Vector3 calculateNormal(const Vector3 _vertex[])
{
	Vector3 edge1 = _vertex[1] - _vertex[0];
	Vector3 edge2 = _vertex[2] - _vertex[0];

	Vector3 cross = VectorFunction::Cross(edge1, edge2);


	return VectorFunction::Normalize(cross);
}

Vector3 calculateCameraToTriangleCenter(const Vector3& _cameraPos, const Vector3 _triangleVertex[])
{
	Vector3 center = _triangleVertex[0] + _triangleVertex[1] + _triangleVertex[2];

	return center - _cameraPos;
}

bool performBackfaceCulling(const Vector3& _cameraPos, const Vector3 _triangleVertex[])
{
	Vector3 triangleNormal = calculateNormal(_triangleVertex);
	Vector3 cameraToCenter = calculateCameraToTriangleCenter(_cameraPos, _triangleVertex);
	float dotProduct = VectorFunction::Dot(triangleNormal, cameraToCenter);

	// 内積が負の場合は背面を向いている
	return dotProduct >= 0.0f;

}
