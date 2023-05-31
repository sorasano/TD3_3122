#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Player.h"
#include"CubeModel.h"
#include"CubeObject3D.h"

class PushButton
{

public:

	void Initialize(FbxModel* buttonUpModel, FbxModel* buttonDwonModel,Player* player, CubeObject3D* buttonObject, CubeObject3D* blockObject);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//ボタンの当たり判定
	void ButtonCol(CubeObject3D* cubeobject);

	//ボタン連動ブロックとプレイヤーの当たり判定
	void BlockCol();

	//ボタンが押されたとき
	void Push();

	void MoveBlock();

	void Reset();
	//セッター
	//ボタン
	void SetPosition(XMFLOAT3 position) { this->position = position; savepos = position; }

	void SetPositionX(float positionX) { this->position.x = positionX; savepos.x = positionX; }

	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	void Setrotate(XMFLOAT3 rotate) { this->rotate = rotate; }
	void SetSRV(ID3D12DescriptorHeap* SRV);
	void SetPush(bool push) { this->push = push; }

	//ブロック
	void SetBlockPosition(XMFLOAT3 blockPosition) {	this->blockPosition = blockPosition; saveblockpos= blockPosition;}

	void SetBlockPositionX(float blockPositionX) { this->blockPosition.x = blockPositionX; saveblockpos.x = blockPositionX;}

	void SetBlockScale(XMFLOAT3 blockScale) { this->blockScale = blockScale; }
	void SetBlockScaleY(float blockScaleY) { this->blockScale.y = blockScaleY; }

	void SetBlockrotate(XMFLOAT3 blockRotate) { this->blockRotate = blockRotate; }

	void SetUpHeight(float upHight) { this->upHight = upHight; }

	XMFLOAT3 GetPosition() { return position; }
	CubeObject3D* GetCubeObject() { return blockColBox; }

	static void SetInput(Input* input) { PushButton::input = input; }
	static void SetDXInput(DXInput* dxInput) { PushButton::dxInput = dxInput; }

private:

	XMFLOAT3 position = { 0.0f,1.0f,-1.0f };
	XMFLOAT3 scale = { 0.01f,0.01f,0.01f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };

	XMFLOAT3 blockPosition = { 0.0f,1.0f,-1.0f };
	XMFLOAT3 blockScale = { 0.03f,0.03f,0.03f };
	XMFLOAT3 blockRotate = { 0.0f,0.0f,0.0f };

	XMFLOAT3 buttonColPosition = { 0.0f,1.0f,-1.0f };
	XMFLOAT3 blockColPosition = { 0.0f,1.0f,-1.0f };

	XMFLOAT3 saveblockpos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 savepos = { 0.0f,0.0f,0.0f };

private:

	//入力
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	//fbx
	FbxObject3D* buttonObject = nullptr;
	FbxModel* buttonUpModel;
	FbxModel* buttonDownModel;
	//ブロック
	FbxObject3D* blockObject = nullptr;
	FbxModel* blockModel = nullptr;

	Player* player;

	//判定
	CubeObject3D* buttonColBox = nullptr;
	CubeObject3D* blockColBox = nullptr;


	//ボタンが押されたとき
	bool push = false;
	int pushTimer = 0;
	//クールタイムで開閉時間
	float pushCollTime = 300;

	//どのくらいあげるか
	float upHight = 5;

	bool isMove = false;
	bool oldIsMove = false;

	//音
	AudioManager* blockUpSE;
	float blockUpSEVolume = 0.1f;
};

