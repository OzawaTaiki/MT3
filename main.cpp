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

	Pendulum pendulum = {
		.anchor = {0.0f,1.0f,0.0f},
		.length = 0.8f,
		.angle = 0.7f,
		.angularVelocity = 0.7f,
		.angularAcceleration = 0.0f
	};

	Sphere sphere = {
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
			pendulum.angularAcceleration =
				-(9.8f / pendulum.length) * std::sin(pendulum.angle);
			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
			pendulum.angle += pendulum.angularVelocity * deltaTime;

			//pは振り子の先端の位置。取り付けたいものを取り付ければ良い
			sphere.center.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
			sphere.center.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
			sphere.center.z = pendulum.anchor.z;
		}



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

		Drawline_se(pendulum.anchor, sphere.center, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), 0xffffffff);
		DrawSphere(sphere, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);

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
