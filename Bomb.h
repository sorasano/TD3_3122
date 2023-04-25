#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Player.h"

class Bomb
{

public:

	void Initialize(FbxModel* bombModel,Player* player);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	//当たり判定
	void Colision();

	//セッター
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetPositionX(float positionX) { this->position.x = positionX; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	void Setrotate(XMFLOAT3 rotate) { this->rotate = rotate; }

	//ゲッター
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 Getrotate() { return rotate; }

private:

	XMFLOAT3 position = { 3.0f,1.0f,-1.0f };
	XMFLOAT3 scale = { 0.01f,0.01f,0.01f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };

private:

	//fbx
	FbxObject3D* bombObject = nullptr;

	Player* player = nullptr;
};

