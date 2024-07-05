#include <Novice.h>
#include "MyLib.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <numbers>
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

	float deltaTime = 1.0f / 60.0f;

	ConicalPendulum conicalPendulum = {
		.anchor = {0.0f,1.0f,0.0f},
		.length = 0.8f,
		.halfApexAngle = 0.7f,
		.angle = 0.0f,
		.angularVelocity = 0.0f,
	};

	Sphere bob = {
		.center = {0,0,0},
		.radius = 0.1f
	};


	bool isStart = false;

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

		if (ImGui::Button("start") || keys[DIK_SPACE])
			isStart = true;
		/*if (ImGui::Button("Reset"))
		{
			isStart = false;
		}*/

		//ImGui::Text("speed \n%.3f,%.3f,%.3f", speed.x, speed.y, speed.z);
		//ImGui::Text(" %.3f,%.3f,%.3f", sphere.center.x, sphere.center.y, speed.z);

		//if (ImGui::TreeNode("Orbit"))
		//{
		//	ImGui::DragFloat3("Center", &orbit.center.x, 0.01f);
		//	ImGui::DragFloat3("Radius", &orbit.radius, 0.01f);
		//	ImGui::TreePop();
		//}
		ImGui::End();

		if (isStart)
		{
			conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
			conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;

			float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
			float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
			bob.center.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * radius;
			bob.center.y = conicalPendulum.anchor.y - height;
			bob.center.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;
		}



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

		Drawline_se(conicalPendulum.anchor, bob.center, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), 0xffffffff);
		DrawSphere(bob, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);

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
