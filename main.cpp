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
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Camera* camera = new Camera(kWindowWidth, kWindowHeight);

	float deltaTime = 1.0f / 60.0f;

	Plane plane = {
		.normal = Normalize({-0.2f,0.9f,-0.3f}),
		.distance = 0.0f
	};

	Ball ball = {
		.position = {0.8f,1.2f,0.3f},
		.acceleration = {0.0f,-9.8f,0.0f},
		.mass = 2.0f,
		.radius = 0.05f,
		.color = WHITE
	};

	float e = 0.8f;

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
		if (ImGui::Button("Reset"))
		{
			isStart = false;
			ball.position = { 0.8f,1.2f,0.3f };
			ball.velocity = { 0.0f,0.0f,0.0f };
		}
		ImGui::DragFloat("e", &e, 0.01f);

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
			ball.velocity += ball.acceleration * deltaTime;
			ball.position += ball.velocity * deltaTime;

			if (IsCollision(Sphere{ ball.position, ball.radius }, plane)) {
				Vector3 reflected = Reflect(ball.velocity, plane.normal);
				Vector3 projectToNormal = Project(reflected, plane.normal);
				Vector3 movingDirection = reflected - projectToNormal;
				ball.velocity = projectToNormal * e + movingDirection;
			}
		}



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

		DrawPlane(plane, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);
		DrawBall(ball, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());

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
