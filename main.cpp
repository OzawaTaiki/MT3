#include <Novice.h>
#include "MyLib.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2A_07_オザワ_タイキ_MT3";

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Camera* camera = new Camera(kWindowWidth, kWindowHeight);


	Vector3 translates[3] = {
		{0.2f, 1.0f, 0.0f},
		{0.4f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f}
	};
	Vector3 rotates[3] = {
		{0.0f, 0.0f, -6.8f},
		{0.0f, 0.0f, -1.4f},
		{0.0f, 0.0f, 0.0f }
	};
	Vector3 scales[3] = {
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f}
	};
	Sphere point[3];
	for (int i = 0; i < 3; i++)
	{
		point[i].center = translates[i];
		point[i].radius = 0.1f;
	}

	Matrix4x4 worldMat[3];
	float radius = 0.01f;

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

		ImGui::Begin("point");
		ImGui::DragFloat("radius", &radius, 0.01f);
		ImGui::End();

		ImGui::Begin("window");
		ImGui::DragFloat3("scale_0", &scales[0].x, 0.01f);
		ImGui::DragFloat3("rotate_0", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("translate_0", &translates[0].x, 0.01f);

		ImGui::DragFloat3("scale_1", &scales[1].x, 0.01f);
		ImGui::DragFloat3("rotate_1", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("translate_1", &translates[1].x, 0.01f);

		ImGui::DragFloat3("scale_2", &scales[2].x, 0.01f);
		ImGui::DragFloat3("rotate_2", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("translate_2", &translates[2].x, 0.01f);

		ImGui::End();

		worldMat[0] =  MakeAffineMatrix(scales[0], rotates[0], translates[0]);
		worldMat[1] =  MakeAffineMatrix(scales[1], rotates[1], translates[1]) * worldMat[0];
		worldMat[2] =  MakeAffineMatrix(scales[2], rotates[2], translates[2]) * worldMat[1];

		Vector3 drawPos[3];
		for (int i = 0; i < 3; i++)
		{
			drawPos[i] = { worldMat[i].m[3][0],worldMat[i].m[3][1],worldMat[i].m[3][2] };
			point[i].center = drawPos[i];
			point[i].radius = radius;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

		Drawline_se(drawPos[0], drawPos[1], camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);
		Drawline_se(drawPos[1], drawPos[2], camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);

		DrawSphere(point[0], camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), RED);
		DrawSphere(point[1], camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), GREEN);
		DrawSphere(point[2], camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), BLUE);

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
