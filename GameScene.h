#pragma once
#include "memory"
#include "list"

#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "FbxObject3d2.h"
#include "FbxModel.h"
#include "LightGroup.h"
#include "Player.h"
#include "Enemy.h"
#include"Vector3.h"

class GameScene
{
	//�����o�֐�
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//�X�V
	void Update();
	//�`��
	void Draw();

	//�����o�ϐ�
private:
	//�f�o�C�X��input
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	//�J����
	std::unique_ptr<Camera> camera_;

	//fbx
	FbxModel* model0 = nullptr;
	FbxObject3D* object0 = nullptr;
	FbxObject3D* object2 = nullptr;

	FbxObject3D2* object1 = nullptr;

	FbxModel* model1 = nullptr;
	//FbxObject3D* object1 = nullptr;
	FbxModel* model2 = nullptr;

	//�u���b�N
	static const int blockSize = 10;
	FbxModel* blockModel = nullptr;
	FbxObject3D2* blockObject[blockSize] = {};

	//�v���C���[
	FbxModel* playerModel = nullptr;
	Player* player = nullptr;
	XMFLOAT3 playerpos;

	//�G
	static const int enemySize = 4;
	FbxModel* enemyModel = nullptr;
	FbxModel* enemyModel2 = nullptr;
	Enemy* enemy[enemySize] = {};
	XMFLOAT3 enemypos[enemySize];
	Vector3 enemyvec[enemySize];

	//���C�g
	LightGroup* lightGroup0 = nullptr;
	LightGroup* lightGroup1 = nullptr;

	float ambientColor0[3] = { 1,1,1 };
	//�������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 0,0,0 };

	float pointLightPos0[3] = { 0,7,0 };
	float pointLightColor0[3] = { 1,1,1 };
	float pointLightAtten0[3] = {0.3f,0.05f,0.0f};

	float circleShadowDir[3] = { 1,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f,0.5f };

	float shadowLightPos[3] = { 3,5,3 };

	//�ό`�s��
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };

	//����p
	Vector3 targetvec[enemySize];
	Vector3 dotvec;
	float dot[enemySize];
	float deg[enemySize];

	int time = 0;
	int maxTime = 150;

	bool isHit = false;
	bool isFound = false;
	bool isback = false;
};
