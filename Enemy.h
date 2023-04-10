#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Vector3.h"
class Enemy
{

public:

	void Initialize(FbxModel* enemyModel, FbxModel* enemyEyeModeld);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetPosition(XMFLOAT3 position);

	void SetScale(XMFLOAT3 scale);

	void Setrotate(XMFLOAT3 rotate);

	void SetModel(FbxModel* enemyModel);

	const Vector3& Getvec() { return vec; }

	const XMFLOAT3& GetPosition() { return position; }

private:

	XMFLOAT3 position = { 0.0f,1.0f,-1.0f };
	XMFLOAT3 scale = { 0.01f,0.01f,0.01f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 target = { 0.0f,0.0f,0.0f };
	Vector3 vec = { 0.0f,0.0f,0.0f };

private:

	//fbx
	FbxObject3D* enemyObject = nullptr;

	//éãê¸
	FbxObject3D* enemyEyeObject = nullptr;

};

