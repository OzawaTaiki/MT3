#include <Novice.h>
#include "MyLib.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2A_07_オザワ_タイキ_MT3_02_08";

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

	OBB obb1{
		.center{-1.0f, 0.0f, 0.0f},
		.orientations = {{1.0f, 0.0f, 0.0f},
						{0.0f, 1.0f, 0.0f},
						{0.0f, 0.0f, 1.0f}},
		.size{0.5f, 0.5f, 0.5f},
		.rotate{0.0f, 0.0f, 0.0f}

	};

	OBB obb2{
		.center{1.0f, 0.0f, 0.0f},
		.orientations = {{1.0f, 0.0f, 0.0f},
						{0.0f, 1.0f, 0.0f},
						{0.0f, 0.0f, 1.0f}},
		.size{0.5f, 0.5f, 0.5f},
		.rotate{0.0f, 0.0f, 0.0f}

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

		ImGui::Begin("OBB1");
		ImGui::DragFloat3("center", &obb1.center.x, 0.01f);
		ImGui::DragFloat3("orientations[0]", &obb1.orientations[0].x, 0.01f);
		ImGui::DragFloat3("orientations[1]", &obb1.orientations[1].x, 0.01f);
		ImGui::DragFloat3("orientations[2]", &obb1.orientations[2].x, 0.01f);
		ImGui::DragFloat3("size", &obb1.size.x, 0.01f);
		ImGui::DragFloat3("rotate", &obb1.rotate.x, 0.01f);
		ImGui::End();
		obb1.CalculateOrientations();

		ImGui::Begin("OBB2");
		ImGui::DragFloat3("center", &obb2.center.x, 0.01f);
		ImGui::DragFloat3("orientations[0]", &obb2.orientations[0].x, 0.01f);
		ImGui::DragFloat3("orientations[1]", &obb2.orientations[1].x, 0.01f);
		ImGui::DragFloat3("orientations[2]", &obb2.orientations[2].x, 0.01f);
		ImGui::DragFloat3("size", &obb2.size.x, 0.01f);
		ImGui::DragFloat3("rotate", &obb2.rotate.x, 0.01f);
		ImGui::End();
		obb2.CalculateOrientations();

		/*ImGui::Begin("sphere");
		ImGui::DragFloat3("center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("radius", &sphere.radius, 0.01f);
		ImGui::End();*/

		/*ImGui::Begin("AABB");
		ImGui::DragFloat3("Min", &aabb.min.x, 0.01f);
		ImGui::DragFloat3("Max", &aabb.max.x, 0.01f);
		ImGui::End();
		aabb.Update();*/

		/*ImGui::Begin("segment");
		ImGui::DragFloat3("origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("diff", &segment.diff.x, 0.01f);
		ImGui::End();*/

		/*ImGui::Begin("Plane");
		ImGui::DragFloat3("normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("Distance", &plane.distance, 0.01f);
		ImGui::End();

		plane.normal = VectorFunction::Normalize(plane.normal);*/

		bool isCollisin = IsCollision(obb1, obb2);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

		DrawOBB(obb1, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), isCollisin ? RED : WHITE);
		DrawOBB(obb2, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), BLUE);
		//Drawline(segment.origin, segment.diff, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);
		//DrawSphere(sphere, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);
		//DrawAABB(aabb, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), isCollisin ? RED : WHITE);

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
