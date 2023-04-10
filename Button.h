#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxObject3d2.h"
#include "FbxModel.h"
#include "Player.h"

class Button
{

public:

	void Initialize(FbxModel* buttonModel,Player *player);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	//ボタンとプレイヤーの当たり判定
	void ButtonCol();

	//ボタン連動ブロックとプレイヤーの当たり判定
	void BlockCol();

	//ボタンが押されたとき
	void PushButton();

	void MoveBlock();

	//セッター
	//ボタン
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	void SetPositionX(float positionX) { this->position.x = positionX; }

	void SetScale(XMFLOAT3 scale) { this->scale = scale; } 

	void Setrotate(XMFLOAT3 rotate) { this->rotate = rotate; }

	//ブロック
	void SetBlockPosition(XMFLOAT3 blockPosition) { this->blockPosition = blockPosition ; }

	void SetBlockPositionX(float blockPositionX) { this->blockPosition.x = blockPositionX; }

	void SetBlockScale(XMFLOAT3 blockScale) { this->blockScale = blockScale; }
	void SetBlockScaleY(float blockScaleY) { this->blockScale.y = blockScaleY; }

	void SetBlockrotate(XMFLOAT3 blockRotate) { this->blockRotate = blockRotate; }

	void SetUpHeight(float upHight) { this->upHight = upHight; }


	static void SetInput(Input* input) { Button::input = input; }
	static void SetDXInput(DXInput* dxInput) { Button::dxInput = dxInput; }

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
	FbxObject3D* buttonObject = nullptr;

	//ブロック
	FbxObject3D2* blockObject = nullptr;
	FbxModel* blockModel = nullptr;

	Player* player;

	//ボタンが押されたとき
	bool push = false;
	int pushTimer = 0;
	//クールタイムで開閉時間
	float pushCollTime = 300;

	//どのくらいあげるか
	float upHight = 5;
};

