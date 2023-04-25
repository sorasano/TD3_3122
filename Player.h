#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include"CubeModel.h"
#include"CubeObject3D.h"
#include "Camera.h"

class Player
{

public:

	void Initialize(FbxModel* playerModel, CubeObject3D* cubeObject);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	//セッター
	void SetPosition(XMFLOAT3 position);
	void SetScale(XMFLOAT3 scale);
	void Setrotate(XMFLOAT3 rotate);

	void SetisDeath(int isDeath) { this->isDeath = isDeath; }
	void SetJump(bool isJump) { this->isJump = isJump; }
	void SetSwamp(bool inSwamp) { this->inSwamp = inSwamp; }

	static void SetInput(Input* input) { Player::input = input; }
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }

	//ゲッター
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 GetRotate() { return rotate; }

	CubeObject3D *GetCubeObject() {return cubeObject; }
	bool GetDeath() { return isDeath; }

	//死亡処理
	void Death();
	//ジャンプ処理
	void Jump();

	void Swamp();

private:
	
	XMFLOAT3 position = {0.0f,1.0f,-1.0f};
	XMFLOAT3 scale = { 0.01f,0.01f,0.01f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };

private:

	//入力
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	//fbx
	FbxObject3D* playerObject = nullptr;

	//移動スピード
	float speed = 0.1;

	//生きてるか
	bool isDeath = false;

	//移動制限　沼に入っているか
	bool inSwamp = false;
	float swampSpeed = 0.05;

	//ジャンプ
	bool isJump = false;

	//重力
	float gravity = 0.0f;
	//ジャンプでかかる重力(ジャンプの高さは jumpGravity / gravitySpeed / 10 例: 0.2 / 0.01 / 10 = 2)
	float jumpGravity = 0.2;
	//重力加速度
	float gravitySpeed = 0.01f;

	//判定
	CubeObject3D* cubeObject = nullptr;

};

