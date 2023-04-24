#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxObject3d2.h"
#include "FbxModel.h"
#include "Player.h"

class Box
{

public:

	void Initialize(FbxModel* buttonModel, Player* player);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	//ボタンとプレイヤーの当たり判定
	void Colision();

	//セッター
	//ボタン
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	void SetPositionX(float positionX) { this->position.x = positionX; }

	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	void Setrotate(XMFLOAT3 rotate) { this->rotate = rotate; }

	static void SetInput(Input* input) { Box::input = input; }
	static void SetDXInput(DXInput* dxInput) { Box::dxInput = dxInput; }

private:

	XMFLOAT3 position = { 0.0f,0.5f,-1.0f };
	XMFLOAT3 scale = { 0.01f,0.01f,0.01f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };

	XMFLOAT3 blockPosition = { 0.0f,1.0f,-1.0f };
	XMFLOAT3 blockScale = { 0.03f,0.03f,0.03f };
	XMFLOAT3 blockRotate = { 0.0f,0.0f,0.0f };

private:

	//入力
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	//fbx
	FbxObject3D* boxObject = nullptr;

	Player* player;

};

