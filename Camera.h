#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "WinApp.h"
//#include "Player.h"
#define PI 3.14159265359

using namespace DirectX;

class Camera
{
public:
	static enum Mode
	{
		forward,
		right,
		backward,
		left,
	};
	static enum ApproachMode
	{
		Max,
		Min,
	};

public:
	//シングルトンインスタンス
	static Camera* GetInstance();
	static void SetInput(Input* input) { Camera::input = input; }

	//インストラクタ デストラクタ
	Camera();
	~Camera();
	//初期化
	void Initialize();
	//更新
	void Update(XMFLOAT3 playerPos);
	//デバッグ Arrowキーで視点座標変更
	void DebugUpdate();

	//ゲッターセッター
	void SetTarget(XMFLOAT3 pos);
	void SetEye(XMFLOAT3 pos);
	XMFLOAT3 GetEye() { return eye_; };
	XMFLOAT3 GetTraget() { return target_; };
	XMFLOAT3 GetUp() { return up_; };
	XMMATRIX GetMatProjection() { return matProjection_; };
	XMMATRIX GetMatView() { return matView_; };
	XMMATRIX GetMatViewProjection() { return matView_ * matProjection_; };

private:
	//入力
	//キーボード
	static Input* input;

	//射影変換
	XMMATRIX matProjection_;
	//ビュー変換行列
	XMMATRIX matView_;
	XMFLOAT3 eye_ = { 0, 20, -100 };
	XMFLOAT3 target_ = { 0, 0, 0 };
	XMFLOAT3 up_ = { 0, 1, 0 };

	//ターゲットまでの距離
	float DebugTargetDistance = 20.0f;
	//加算用
	float DebugChangeRot = PI;
	float DebugChangeRot2 = 0.5f;
	float DebugChangeDistance = 0.0f;
};
