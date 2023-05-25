#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Player.h"
#include"CubeModel.h"
#include"CubeObject3D.h"

class PushBlock
{
public://メンバ関数
	//初期化
	void Initialize(FbxModel* blockModel, Player* player, CubeObject3D* cubeObject);
	//更新処理
	void Update();
	//描画処理
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//当たり判定
	void Collision();
	//ブロックを押す
	void Push();
	//押さない
	void NoPush();
	//押し戻し処理
	void pushback(CubeObject3D* cubeObject);

	//セッター
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	void SetPositionX(float positionX) { this->position.x = positionX; }

	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	void Setrotate(XMFLOAT3 rotate) { this->rotate = rotate; }
	void SetSRV(ID3D12DescriptorHeap* SRV);

	//ゲッター
	bool GetIsPush() { return isPush; }
	bool GetNoPush() { return noPush; }
	XMFLOAT3 GetPosition() { return position; }
public://静的メンバ関数
	static void SetInput(Input* input) { PushBlock::input = input; }
	static void SetDXInput(DXInput* dxInput) { PushBlock::dxInput = dxInput; }


private://メンバ変数
	XMFLOAT3 position = { 0.0f,1.0f,-1.0f };
	XMFLOAT3 scale = { 0.01f,0.01f,0.01f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };
	//仮に動かしたら
	XMFLOAT3 newposition = { 0.0f,1.0f,-1.0f };

	//入力
	////キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	//ブロック
	FbxObject3D* blockObject = nullptr;
	//プレイヤー
	Player* player;
	//判定
	CubeObject3D* cubeObject = nullptr;
	bool isPush = false;
	bool noPush = false;
};