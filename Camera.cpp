#include "Camera.h"
#include "Novice.h"
#include <imgui.h>

Camera::Camera(int _windowWidth, int _windowHeight)
{
	windowSize = { (float)_windowWidth,(float)_windowHeight };

	projectionMatrix =  MakePerspectiveFovMatrix(0.45f, windowSize.x / windowSize.y, 0.1f, 100.0f);
	viewportMatrix =  MakeViewportMatrix(0, 0, windowSize.x, windowSize.y, 0.0f, 1.0f);
	initialize();
}

Camera::~Camera()
{
}

void Camera::initialize(const Vector3& _scale, const Vector3& _rotate, const Vector3& _translate)
{
	scale = _scale;
	rotate = _rotate;
	translate = _translate;

	cameraMatrix =  MakeAffineMatrix(_scale, _rotate, _translate);
	viewMatrix =  Inverse(cameraMatrix);
}

void Camera::Update(const char* _keys, const char* _preKeys)
{

#ifdef _DEBUG
	ImGui();
#endif // _DEBUG

	if (_keys[DIK_LCONTROL] &&
		_keys[DIK_RETURN] && !_preKeys[DIK_RETURN])
		isCameraControl = isCameraControl ? false : true;

	preMousePos = mousePos;
	int x, y;
	Novice::GetMousePosition(&x, &y);
	mousePos = { (float)x,(float)y,0 };

	if (_keys[DIK_LCONTROL] || isCameraControl)
	{
		Vector3 mouseMove =  Subtract(mousePos, preMousePos);
		// VectorScreenPrintf(0, 0, mouseMove);
		rotate.x += mouseMove.y * mouseSensi;
		rotate.y += mouseMove.x * mouseSensi;

		Vector3 move{ 0,0,0 };
		if (_keys[DIK_A])
			move.x -= 1;
		if (_keys[DIK_D])
			move.x += 1;
		if (_keys[DIK_SPACE])
			translate.y += cameraMoveSpeed;
		if (_keys[DIK_LSHIFT])
			translate.y -= cameraMoveSpeed;
		if (_keys[DIK_W])
			move.z += 1;
		if (_keys[DIK_S])
			move.z -= 1;

		Matrix4x4 RotateYMatrix =  MakeRotateYMatrix(rotate.y);

		Vector3 nMove =  Normalize(move);
		Vector3 direction =  Transform(nMove, RotateYMatrix);
		translate =  Add( Multiply(cameraMoveSpeed, direction), translate);
	}

	cameraMatrix =  MakeAffineMatrix(scale, rotate, translate);
	viewMatrix =  Inverse(cameraMatrix);
	viewProjectionMatrix =  Multiply(viewMatrix, projectionMatrix);
}

void Camera::ImGui()
{
	ImGui::Begin("camera");
	ImGui::DragFloat3("cameraTranslate", &translate.x, 0.01f);
	ImGui::DragFloat3("cameraRotate", &rotate.x, 0.01f);
	ImGui::DragFloat("mouseSensi", &mouseSensi, 0.00001f);
	ImGui::DragFloat("moveSpeed", &cameraMoveSpeed, 0.001f);
	ImGui::End();
}
