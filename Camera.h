#pragma once

#include "MatrixFunction.h"
#include "VectorFunction.h"

class Camera
{
public:
	Camera(int _windowWidth, int _windowHeight);
	~Camera();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_scale">スケール</param>
	/// <param name="_rotate">回転量</param>
	/// <param name="_translate">座標</param>
	void initialize(const Vector3& _scale = { 1.0f,1.0f,1.0f }, const Vector3& _rotate = { 0.26f, 0.0f, 0.0f }, const Vector3& _translate = { 0.0f, 1.9f, -6.49f });

	void Update(const char* _keys, const char* _preKeys);

	Matrix4x4 GetViewportMatrix() { return viewportMatrix; };
	Matrix4x4 GetviewProjectionMatrix() { return viewProjectionMatrix; };

private:

#ifdef _DEBUG
	void ImGui();
#endif // _DEBUG


	Vector2 windowSize;

	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;

	Matrix4x4 viewMatrix;
	Matrix4x4 cameraMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewportMatrix;
	Matrix4x4 viewProjectionMatrix;

	///カメラ操作関連
	bool isCameraControl = false;
	Vector3 mousePos = { 0 };
	Vector3 preMousePos = { 0 };
	float mouseSensi = 0.001f;
	float cameraMoveSpeed = 0.02f;
};

