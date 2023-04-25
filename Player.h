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

	void SetPosition(XMFLOAT3 position);

	void SetScale(XMFLOAT3 scale);

	void Setrotate(XMFLOAT3 rotate);


	XMFLOAT3 GetPosition() { return position; }

	CubeObject3D *GetCubeObject() {return cubeObject; }

	bool GetDeath() { return isDeath; }

	void Death();

	void SetisDeath(int isDeath) { this->isDeath = isDeath; };

	static void SetInput(Input* input) { Player::input = input; }
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }

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

	//生きてるか
	bool isDeath = false;

	//判定
	CubeObject3D* cubeObject = nullptr;

};

