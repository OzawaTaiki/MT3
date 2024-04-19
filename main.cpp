#include <Novice.h>
#include "MyLib.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2A_07_オザワ_タイキ_MT3_02_00";

static const int kWindowWidth = 1280;
static const int kWindowHeght = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Camera* camera = new Camera(kWindowWidth, kWindowHeght);
	camera->initialize();

	Sphere sphere;
	sphere = {
		{0,0,0},
		0.5f
	};

	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 point{ -1.5f,0.6f,0.6f };

	Vector3 project = Project(VectorFunction::Subtract(point, segment.origin), segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);

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

		ImGui::Begin("window");
		ImGui::DragFloat3("point", &point.x, 0.01f);
		ImGui::InputFloat3("segment origin", &segment.origin.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("segment diff", &segment.diff.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("closestPoint", &closestPoint.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();


		project = Project(VectorFunction::Subtract(point, segment.origin), segment.diff);
		closestPoint = ClosestPoint(point, segment);

		Sphere pointSphere{ point,0.01f };
		Sphere closestpointSphere{ closestPoint,0.01f };

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

		Drawline(segment.origin, segment.diff, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);
		DrawSphere(pointSphere, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), RED);
		DrawSphere(closestpointSphere, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), BLACK);

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
