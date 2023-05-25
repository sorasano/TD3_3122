#pragma once
#include "memory"
#include "list"

#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "LightGroup.h"
#include "Player.h"
#include "Enemy.h"
#include"Vector3.h"
#include "Sprite.h"
#include "Button.h"
#include"CameraEnemy.h"
#include "Autosave.h"
#include "Bomb.h"
#include"CubeModel.h"
#include"CubeObject3D.h"
#include "SpriteManager.h"
#include"Goal.h"
#include "Swamp.h"
#include "Light.h"
#include "PushBlock.h"
#include"Block.h"
#include"PushButton.h"
#include "AudioManager.h"
#include "Ladder.h"
#include "MoveEnemy.h"
#include "Menu.h"

enum Scene {
	PLAY,
	MENU,
	CLEAR
};

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
	void DrawFBXLightView();
	void DrawFBX();
	void DrawSprite();

	//リセット //isFirst = 最初からか
	void Reset(bool isFirst);

	//セッター
	void SetSRV(ID3D12DescriptorHeap* SRV);
	//ゲッター
	DirectX::XMMATRIX GetLightViewProjection();

	//メンバ変数
private:
	//デバイスとinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	//カメラ
	std::unique_ptr<Camera> camera_;

	//ライト 影用
	Light* light = nullptr;
	float lightDir[3] = { 0.0f,-1.0f , 1.0f };
	float lightPos[3] = { 0.0f,10.0f,25.0f };
	float lightTarget[3] = { 0.0f,0.0f,0.0f };

	//シーン
	int scene = PLAY;

	//床
	FbxObject3D* groundObject = nullptr;
	FbxModel* groundModel = nullptr;

	////ブロック
	//static const int blockSize = 16;
	//FbxModel* blockModel = nullptr;
	//FbxObject3D* blockObject[blockSize] = {};

	//プレイヤー
	Player* player = nullptr;
	XMFLOAT3 playerpos;

	//スプライトマネージャー
	SpriteManager* spriteManager = nullptr;

	//敵
	static const int enemySize = 43;

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
	static const int cameraEnemySize = 8;
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
	static const int buttonSize = 22;
	FbxModel* buttonModel = nullptr;
	Button* button[buttonSize] = {};

	//押している間のボタン
	static const int pushButtonSize = 2;
	FbxModel* pushButtonModel = nullptr;
	PushButton* pushButton[pushButtonSize] = {};

	//爆弾
	static const int bombSize = 17;
	FbxModel* bombModel = nullptr;
	Bomb* bomb[buttonSize] = {};

	//沼
	static const int swampSize = 10;
	FbxModel* swampModel = nullptr;
	Swamp* swamp[swampSize] = {};

	//押せるブロック
	static const int pushBlockSize = 2;
	FbxModel* pushBlockModel = nullptr;
	PushBlock* pushBlock[pushBlockSize] = { };

	//動かせないブロック
	//21
	static const int blockSize = 16;
	FbxModel* blockModel = nullptr;
	Block* block[blockSize] = { };
	//梯子
	static const int ladderSize = 6;
	FbxModel* ladderModel = nullptr;
	Ladder* ladder[ladderSize] = {};

	//動く敵
	static const int moveEnemySize = 2;
	MoveEnemy* moveEnemy[moveEnemySize] = {};

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
	float clearPos = 10;

	bool isHit = false;
	bool isFound = false;


	//テクスチャ
	Sprite* clearSprite = nullptr;
	Sprite* whiteSprite = nullptr;

	Sprite* gameoverSprite = nullptr;
	Sprite* blackSprite = nullptr;

	Sprite* titleSprite = nullptr;
	Sprite* titleUISprite = nullptr;

	Sprite* playUISprite = nullptr;

	//タイトルUI用タイマー
	int titleTimer;
	int titleAssistTime = 300;

	//当たり判定キューブモデル
	CubeModel* cubeModel = nullptr;
	//当たり判定キューブオブジェクト
	CubeObject3D* playerColBox = nullptr;
	CubeObject3D* buttonColBox[buttonSize] = {};
	CubeObject3D* pushBlockColBox[pushBlockSize] = {};
	CubeObject3D* blockColBox[blockSize] = {};
	CubeObject3D* pushButtonColBox[pushButtonSize] = {};
	CubeObject3D* pushButtonBlockColBox[pushButtonSize] = {};
	//お試し用
	CubeObject3D* cube = nullptr;

	//ゴール判定
	Goal* goal = nullptr;

	//演出
	float alpha = 0.0f;
	bool isback = false;
	bool noPush = false;

	//音
	AudioManager* titleBGM;
	float titleBGMVolume = 0.1f;

	//メニュー
	Menu* menu;
};
