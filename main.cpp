#include <Novice.h>
#include "VectorFunction.h"
#include "MatrixFunction.h"
#include <imgui.h>

const char kWindowTitle[] = "LE2A_07_オザワ_タイキ_MT3_00_00";

static const int kWindowWidth = 1280;
static const int kWindowHeght = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


	Vector3 v1 = { 1.2f,-3.9f,2.5f };
	Vector3 v2 = { 2.8f,0.4f,-1.3f };
	Vector3 cross = VectorFunction::Cross(v1, v2);

	Vector3 rotate = { 0,0,0 };
	Vector3 translate = { 0,0,1.0f };
	Vector3 kLocalVertices[3] = {
		{0,0.1f / 1.732f,0},
		{0.05774f,-0.05f / 1.732f,0},
		{-0.05774f,-0.05f / 1.732f,0}
	};

	Vector3 cameraTranslate{ 0.0f,0.0f,0.0f };
	Vector3 cameraRotate{ 0.1f,0.0f,0.0f };

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
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);
		ImGui::End();

		if (keys[DIK_W])
			translate.z += 0.005f;
		if (keys[DIK_S])
			translate.z -= 0.005f;
		if (keys[DIK_A])
			translate.x -= 0.005f;
		if (keys[DIK_D])
			translate.x += 0.005f;

		rotate.y += 0.05f;


		Matrix4x4 worldMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, rotate, translate);
		Matrix4x4 cameraMatrix = MatrixFunction::MakeAffineMatrix({ 1,1,1 }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = MatrixFunction::Inverse(cameraMatrix);
		Matrix4x4 projectioonMatrix = MatrixFunction::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeght), 0.1f, 100.0f);
		Matrix4x4 worldViewprojectioonMatrix = MatrixFunction::Multiply(worldMatrix, MatrixFunction::Multiply(viewMatrix, projectioonMatrix));
		Matrix4x4 viewportMatrix = MatrixFunction::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeght), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		for (int i = 0; i < 3; i++)
		{
			Vector3 ndcVertex = VectorFunction::Transform(kLocalVertices[i], worldViewprojectioonMatrix);
			screenVertices[i] = VectorFunction::Transform(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorFunction::VectorScreenPrintf(0, 0, cross, "cross");

		Novice::DrawTriangle(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid);

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
