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
#include "Sprite.h"
#include "Button.h"
#include"CameraEnemy.h"
#include "Autosave.h"

class GameScene
{
	//メンバ関数
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//更新
	void Update();
	//描画
	void Draw();

	//メンバ変数
private:
	//デバイスとinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	//カメラ
	std::unique_ptr<Camera> camera_;

	//fbx
	FbxModel* model0 = nullptr;
	FbxObject3D* object0 = nullptr;
	FbxObject3D* object2 = nullptr;

	FbxObject3D2* object1 = nullptr;

	FbxModel* model1 = nullptr;
	//FbxObject3D* object1 = nullptr;
	FbxModel* model2 = nullptr;

	//ブロック
	static const int blockSize = 11;
	FbxModel* blockModel = nullptr;
	FbxObject3D2* blockObject[blockSize] = {};

	//プレイヤー
	FbxModel* playerModel = nullptr;
	Player* player = nullptr;
	XMFLOAT3 playerpos;

	//敵
	static const int enemySize = 11;
	FbxModel* enemyModel = nullptr;
	FbxModel* enemyModel2 = nullptr;
	Enemy* enemy[enemySize] = {};
	XMFLOAT3 enemypos[enemySize];
	Vector3 enemyvec[enemySize];
	//判定用
	XMFLOAT3 enemypos2[enemySize];
	XMFLOAT3 enemyposset[enemySize];
	Vector3 enemytargetvec[enemySize];
	float enemydot[enemySize];
	float enemydeg;
	Vector3 enemytarget[enemySize];
	float enemyangle = 0.0f;
	
	int time = 0;
	int maxTime = 170;
	float distance = 9;

	//監視カメラ
	static const int cameraEnemySize = 1;
	FbxModel* cameraEnemyModel = nullptr;
	CameraEnemy* cameraEnemy[cameraEnemySize] = {};
	XMFLOAT3 cameraEnemypos[cameraEnemySize];
	Vector3 cameraEnemyvec[cameraEnemySize];

	//判定用
	XMFLOAT3 cameraEnemyposset[cameraEnemySize];
	Vector3 cameraEnemytargetvec[cameraEnemySize];
	float cameraEnemydot[cameraEnemySize];
	float cameraEnemydeg;
	Vector3 cameraEnemytarget[cameraEnemySize];
	float cameraEnemyangle = 0.0f;
	XMFLOAT3 cameraEnemypos2[cameraEnemySize];


	FbxModel* enemyEyeModel = nullptr;

	//ボタン
	static const int buttonSize = 7;
	FbxModel* buttonModel = nullptr;
	Button* button[buttonSize] = {};

	//オートセーブ
	Autosave *autoSave;

	//ライト
	LightGroup* lightGroup0 = nullptr;
	LightGroup* lightGroup1 = nullptr;

	float ambientColor0[3] = { 1,1,1 };
	//光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 0,0,0 };

	float pointLightPos0[3] = { 0,7,0 };
	float pointLightColor0[3] = { 1,1,1 };
	float pointLightAtten0[3] = { 0.3f,0.05f,0.0f };

	float circleShadowDir[3] = { 1,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f,0.5f };

	float shadowLightPos[3] = { 3,5,3 };

	//変形行列
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };

	//クリア

	bool isClear = false;
	float clearPos = 130;

	bool isHit = false;
	bool isFound = false;
	bool isback = false;


	//テクスチャ
	uint32_t clearTexture = 0;		//クリア画面
	Sprite* clearSprite = nullptr;

	uint32_t gameoverTexture = 0;		//ゲームオーバー画面
	Sprite* gameoverSprite = nullptr;

	uint32_t titleTexture = 0;		//タイトル画面
	Sprite* titleSprite = nullptr;

	uint32_t titleUITexture = 0;
	Sprite* titleUISprite = nullptr;


	//タイトルUI用タイマー
	int titleTimer;
	int titleAssistTime = 300;
};
