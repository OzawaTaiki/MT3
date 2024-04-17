#include <Novice.h>
#include "VectorFunction.h"
#include "MatrixFunction.h"

const char kWindowTitle[] = "LE2A_07_オザワ_タイキ_MT3_00_04";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 rotate = { 0.4f,1.43f,-0.8f };
	Matrix4x4 rotateXMatrix = MatrixFunction::MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MatrixFunction::MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MatrixFunction::MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = MatrixFunction::Multiply(rotateXMatrix, MatrixFunction::Multiply(rotateYMatrix, rotateZMatrix));

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		MatrixFunction::MatrixScreenPrintf(0, 0, rotateXMatrix, "rotateXMatrix");
		MatrixFunction::MatrixScreenPrintf(0, kRowHeight * 5, rotateYMatrix, "rotateYMatrix");
		MatrixFunction::MatrixScreenPrintf(0, kRowHeight * 5 * 2, rotateZMatrix, "rotateZMatrix");
		MatrixFunction::MatrixScreenPrintf(0, kRowHeight * 5 * 3, rotateXYZMatrix, "rotateXYZMatrix");

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
