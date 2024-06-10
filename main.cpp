#include <Novice.h>
#include "MyLib.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2A_07_オザワ_タイキ_MT3_02_07";

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

	AABB aabb;
	aabb.min = { 0.0f,0.0f ,0.0f };
	aabb.max = { 0.5f,0.5f ,0.5f };

	Segment segment;
	segment.diff = { 2.0f,-0.5f,0.0f };
	segment.origin = { 0.7f,0.3f,0.0f };

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

		ImGui::Begin("AABB");
		ImGui::DragFloat3("Min", &aabb.min.x, 0.01f);
		ImGui::DragFloat3("Max", &aabb.max.x, 0.01f);
		ImGui::End();
		aabb.Update();

		ImGui::Begin("segment");
		ImGui::DragFloat3("origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("diff", &segment.diff.x, 0.01f);
		ImGui::End();

		/*ImGui::Begin("Plane");
		ImGui::DragFloat3("normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("Distance", &plane.distance, 0.01f);
		ImGui::End();

		plane.normal = VectorFunction::Normalize(plane.normal);*/

		bool isCollisin = IsCollision(aabb, segment);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

		DrawAABB(aabb, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), isCollisin ? RED : WHITE);
		Drawline(segment.origin, segment.diff, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);

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
