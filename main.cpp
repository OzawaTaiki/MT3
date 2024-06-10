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

	Vector3 cpoint[4] = {
		{ -0.8f, 0.58f, 1.0f },
		{ 1.76f,1.0f,-0.3f },
		{0.94f,-0.7f, 2.3f },
		{-0.53f, -0.26f, -0.15f}
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


		ImGui::Begin("CatmullRom");
		ImGui::DragFloat3("point_0", &cpoint[0].x, 0.01f);
		ImGui::DragFloat3("point_1", &cpoint[1].x, 0.01f);
		ImGui::DragFloat3("point_2", &cpoint[2].x, 0.01f);
		ImGui::DragFloat3("point_3", &cpoint[3].x, 0.01f);
		ImGui::End();


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

		DrawCatmullRom(cpoint[0], cpoint[1], cpoint[2], cpoint[3], camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), BLUE, 1);

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
