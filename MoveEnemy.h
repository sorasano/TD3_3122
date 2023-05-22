#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Player.h"

class MoveEnemy
{

public:

	void Initialize(FbxModel* moveEnemyModel, Player* player);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//当たり判定
	void Colision();
	//見つかる判定
	void DiscoverColision();

	//セッター
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetPositionX(float positionX) { this->position.x = positionX; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	void Setrotate(XMFLOAT3 rotate) { this->rotate = rotate; }
	void SetSRV(ID3D12DescriptorHeap* SRV);

	//ゲッター
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 Getrotate() { return rotate; }

private:

	XMFLOAT3 position = { 3.0f,1.0f,-1.0f };
	XMFLOAT3 scale = { 0.002f,0.001f,0.002f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };

private:

	//fbx
	FbxObject3D* moveEnemyObject = nullptr;

	Player* player = nullptr;

	//見つかる当たり判定大きさ

};

