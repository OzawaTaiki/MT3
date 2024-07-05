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

	Sphere sphere{};
	sphere.radius = 0.1f;


	bool isStart = false;

	Circle orbit;	//円運動の軌道
	orbit.center = { 0.0f,0.5f,0.0f };
	orbit.radius = 1.0f;

	float anglarVelocity = (float)std::numbers::pi;		//時間当たりの速度
	float angle = 0.0f;									//現在の円上での位置

	Vector3 speed{ 0,0,0 };

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
		if (ImGui::Button("Reset"))
		{
			isStart = false;
		}

		ImGui::Text("speed \n%.3f,%.3f,%.3f", speed.x, speed.y, speed.z);
		ImGui::Text(" %.3f,%.3f,%.3f", sphere.center.x, sphere.center.y, speed.z);

		if (ImGui::TreeNode("Orbit"))
		{
			ImGui::DragFloat3("Center", &orbit.center.x, 0.01f);
			ImGui::DragFloat3("Radius", &orbit.radius, 0.01f);
			ImGui::TreePop();
		}
		ImGui::End();

		if (isStart)
		{
			angle += anglarVelocity * deltaTime;

			speed.x = -orbit.radius * angle * std::sin(angle);
			speed.y = orbit.radius * angle * std::cos(angle);
			speed.z = 0;

			sphere.center.x = orbit.center.x + std::cos(angle) * orbit.radius;
			sphere.center.y = orbit.center.y + std::sin(angle) * orbit.radius;
			sphere.center.z = orbit.center.z;
		}



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

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
