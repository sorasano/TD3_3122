#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Vector3.h"
#include "Player.h"

class CameraEnemy
{

public:

	void Initialize(FbxModel* enemyModel, FbxModel* enemyEyeModel, FbxModel* enemyRotModel, Player* player);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	void SetPosition(XMFLOAT3 position);

	void SetScale(XMFLOAT3 scale);

	void Setrotate(XMFLOAT3 rotate);

	void SetModel(FbxModel* enemyModel);

	void SetTarget(Vector3 target) { this->target = target; }
	void SetSRV(ID3D12DescriptorHeap* SRV);

	const Vector3& Getvec() { return vec; }

	const XMFLOAT3& GetPosition() { return position; }

	const Vector3& GetTarget() { return target; }

	const float GetDeg() { return enemydeg; }

private:

	XMFLOAT3 position = { 0.0f,1.0f,-1.0f };
	XMFLOAT3 scale = { 0.01f,0.01f,0.01f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };
	
	XMFLOAT3 objrotate = { 0.0f,0.0f,0.0f };
	
	XMFLOAT3 rotPosition = { 0.0f,0.0f,0.0f };
	XMFLOAT3 rotScale = { 0.001f,0.10f,0.001f };
	XMFLOAT3 rotRotate = { 0.0f,0.0f,0.0f };
	
	Vector3 target = { 0.0f,0.0f,0.0f };
	Vector3 vec = { 0.0f,0.0f,0.0f };
	
	//îªíËóp
	Vector3 enemytargetvec = { 0.0f,0.0f,0.0f };
	float enemydot = 0.0f;
	float enemydeg = 0.0f;
	//äpìx
	float enemyangle = 0.0f;

	//ãììÆéûä‘
	int time = 0;
	int maxTime = 170;
	float distance = 9;
	bool isback = false;
private:
	//ìGÉÇÉfÉã
	FbxModel* enemyModel = nullptr;
	//fbx
	FbxObject3D* enemyObject = nullptr;
	//éãê¸
	FbxObject3D* enemyEyeObject = nullptr;
	//ñ_
	FbxObject3D* enemyRotObject = nullptr;
	Player* player;


};

