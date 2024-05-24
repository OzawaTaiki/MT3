#include <Novice.h>
#include "MyLib.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2A_07_オザワ_タイキ_MT3_00_00";

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

	AABB aabb1 = {
		{ -0.5f,-0.5f ,-0.5f},
		{0.0f,0.0f,0.0f}
	};

	AABB aabb2 = {
		{  0.2f, 0.2f , 0.2f},
		{1.0f,1.0f,1.0f}
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

		/*ImGui::Begin("Sphere");
		ImGui::DragFloat3("Position", &sphere.center.x, 0.01f);
		ImGui::DragFloat("radius", &sphere.radius, 0.01f);
		ImGui::End();

		ImGui::Begin("Plane");
		ImGui::DragFloat3("normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("Distance", &plane.distance, 0.01f);
		ImGui::End();

		plane.normal = VectorFunction::Normalize(plane.normal);*/

		ImGui::Begin("AABB");
		ImGui::DragFloat3("AABB1_Min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("AABB1_Max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("AABB2_Min", &aabb2.min.x, 0.01f);
		ImGui::DragFloat3("AABB2_Max", &aabb2.max.x, 0.01f);
		ImGui::End();

		aabb1.Update();
		aabb2.Update();

		bool isCollision = IsCollision(aabb1, aabb2);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

		DrawAABB(aabb1, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), isCollision ? RED : WHITE);
		DrawAABB(aabb2, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);

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
