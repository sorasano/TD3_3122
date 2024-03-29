#include "Camera.h"
#include "Math.h"
#define PI 3.14159265359

Input* Camera::input = nullptr;

Camera* Camera::GetInstance()
{
	static Camera instance;
	return &instance;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	//射影変換
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//上下画角45度
		(float)window_width / window_height,//アスペクト比(画面横幅/画面立幅)
		0.1f, 1000.0f						//前端、奥端
	);

	//行列計算
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}


void Camera::Update(XMFLOAT3 playerPos)
{
	//デバッグ用
	/*if (input->IsKeyPress(DIK_D))
	{
		position.x += 0.55;
	}
	if (input->IsKeyPress(DIK_A))
	{
		position.x -= 0.55;
	}
	if (input->IsKeyPress(DIK_W))
	{
		eye_.y += 0.55;
	}
	if (input->IsKeyPress(DIK_S))
	{
		eye_.y -= 0.55;
	}
	if (input->IsKeyPress(DIK_W))
	{
		position.z += 0.55;
	}
	if (input->IsKeyPress(DIK_S))
	{
		position.z -= 0.55;
	}*/

	//追従カメラ
	//if (input->IsKeyPress(DIK_A))
	//{
	//	target_.x -= 0.1;
	//}
	//if (input->IsKeyPress(DIK_D))
	//{
	//	target_.x += 0.1;
	//}

	target_.x = playerPos.x;

	//視点座標に代入
	eye_.x = sin(DebugChangeRot) * DebugTargetDistance + target_.x;
	eye_.y = sin(DebugChangeRot2) * DebugTargetDistance + target_.y;
	eye_.z = cos(DebugChangeRot) * DebugTargetDistance + target_.z;

	//target_.x = playerPos.x;

	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}

void Camera::DebugUpdate()
{
	//1フレームあたりの移動量
	float rot = (float)PI / 120.0f;

	//視点座標を変更
	if (input->IsKeyPress(DIK_LEFT))
	{
		DebugChangeRot -= rot;
	}
	if (input->IsKeyPress(DIK_RIGHT))
	{
		DebugChangeRot += rot;
	}
	if (input->IsKeyPress(DIK_UP))
	{
		DebugChangeRot2 -= rot;
	}
	if (input->IsKeyPress(DIK_DOWN))
	{
		DebugChangeRot2 += rot;
	}

	//ターゲットまでの距離を変更
	if (input->IsKeyPress(DIK_O))
	{
		DebugTargetDistance -= 0.2;
	}
	if (input->IsKeyPress(DIK_P))
	{
		DebugTargetDistance += 0.2;
	}

	//視点座標に代入
	eye_.x = sin(DebugChangeRot) * DebugTargetDistance + target_.x;
	eye_.y = sin(DebugChangeRot2) * DebugTargetDistance + target_.y;
	eye_.z = cos(DebugChangeRot) * DebugTargetDistance + target_.z;
}

void Camera::SetTarget(XMFLOAT3 pos)
{
	target_ = pos;
}

void Camera::SetEye(XMFLOAT3 pos)
{
	eye_ = pos;
}