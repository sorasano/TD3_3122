#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Player.h"

class MoveEnemy
{

public:

	void Initialize(Player* player);

	void Update();
	//動き
	void Move();

	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//当たり判定
	void Colision();
	//見つかる判定
	void DiscoverColision();

	//リセット
	void Reset();

	//セッター
	void SetPosition(XMFLOAT3 position) { this->position = position, this->initPosition = position; }
	void SetPositionX(float positionX) { this->position.x = positionX,this->initPosition.x = positionX; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	void Setrotate(XMFLOAT3 rotate) { this->rotate = rotate,this->initRotate = rotate; }
	void SetSRV(ID3D12DescriptorHeap* SRV);

	//ゲッター
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 Getrotate() { return rotate; }

private:

	XMFLOAT3 position = { 3.0f,1.0f,-1.0f };
	XMFLOAT3 scale = { 0.002f,0.001f,0.002f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };

	XMFLOAT3 initPosition = {};
	XMFLOAT3 initRotate = {};

private:

	//fbx
	FbxObject3D* moveEnemyObject = nullptr;
	FbxModel* enemyWaitModel = nullptr;
	FbxModel* enemyWalkModel = nullptr;

	Player* player = nullptr;

	//見つかる当たり判定大きさ
	XMFLOAT3  discoverSize = { 5,5,1 };

	//見つかったか
	bool isDiscover = false;

	//移動速度
	float speed = 0.05;

	//行動中の動き
	int action;
	//前フレームの動き
	int oldAction;
};

