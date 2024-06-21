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

	float deltaTime = 1.0f / 60.0f;

	Spring spring{};
	spring.anchor = { 0.0f, 1.0f, 0.0f };
	spring.naturalLength = 0.7f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball{};
	ball.position = { 0.8f, 0.2f, 0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;

	const Vector3 kGravity{ 0.0f,-9.8f,0.0f };

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
			ball.position = { 0.8f, 0.2f, 0.0f };
			ball.velocity = { 0.0f,0.0f,0.0f };
			isStart = false;
		}

		ImGui::End();


		Vector3 diff = ball.position - spring.anchor;

		if (isStart)
		{
			float length = Length(diff);
			if (length != 0.0f) {
				Vector3 direction = Normalize(diff);
				Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
				Vector3 displacement = length * (ball.position - restPosition);
				Vector3 restoringForce = -spring.stiffness * displacement;
				Vector3 dapingForce = -spring.dampingCoefficient * ball.velocity;
				Vector3 gravityForce = kGravity * ball.mass;
				Vector3 force = restoringForce + dapingForce + gravityForce;
				ball.acceleration = force / ball.mass;

				ball.velocity = ball.velocity + ball.acceleration * deltaTime;
				ball.position = ball.position + ball.velocity * deltaTime;
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());


		Drawline(spring.anchor, diff, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);
		DrawBall(ball, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix());
		//Drawline_se(spring.anchor, ball.position, camera->GetviewProjectionMatrix(), camera->GetViewportMatrix(), WHITE);

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
