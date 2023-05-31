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
#include "CSVLoader.h"

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

	//スプライトマネージャー
	SpriteManager* spriteManager = nullptr;

	//床
	FbxObject3D* groundObject = nullptr;
	FbxModel* groundModel = nullptr;

	//背景
	FbxObject3D* backGroundObject = nullptr;
	FbxModel* backGroundModel = nullptr;

	//csvの変数
	CSVLoader* treeCsv = nullptr;
	CSVLoader* enemyCsv = nullptr;
	CSVLoader* cameraEnemyCsv = nullptr;
	CSVLoader* buttonCsv = nullptr;
	CSVLoader* buttonBlockCsv = nullptr;
	CSVLoader* pushButtonCsv = nullptr;
	CSVLoader* pushButtonBlockCsv = nullptr;
	CSVLoader* bombCsv = nullptr;
	CSVLoader* swampCsv = nullptr;
	CSVLoader* pushBlockCsv = nullptr;
	CSVLoader* blockCsv = nullptr;
	CSVLoader* ladderCsv = nullptr;
	CSVLoader* moveEnemyCsv = nullptr;

	//木の数
	size_t treeNum = 63;
	//木のモデル
	FbxModel* modelTree = nullptr;
	//木のオブジェクト
	std::list<std::unique_ptr<FbxObject3D>> objectTree;

	//プレイヤー
	Player* player = nullptr;
	XMFLOAT3 playerpos;

	//敵
	size_t enemySize = 24;
	FbxModel* enemyModel = nullptr;
	FbxModel* enemyModel2 = nullptr;
	FbxModel* enemyEyeModel = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys;

	//監視カメラ
	size_t cameraEnemySize = 5;
	FbxModel* cameraEnemyModel = nullptr;
	std::list<std::unique_ptr<CameraEnemy>> cameraEnemys;

	//ボタン
	size_t buttonSize = 23;
	FbxModel* buttonUpModel = nullptr;
	FbxModel* buttonDownModel = nullptr;
	std::list<std::unique_ptr<Button>> buttons;

	//押している間のボタン
	size_t pushButtonSize = 12;
	FbxModel* pushButtonUpModel = nullptr;
	FbxModel* pushButtonDownModel = nullptr;
	std::list<std::unique_ptr<PushButton>> pushButtons;

	//爆弾
	size_t bombSize = 19;
	FbxModel* bombModel = nullptr;
	std::list<std::unique_ptr<Bomb>> bombs;

	//沼
	size_t swampSize = 13;
	FbxModel* swampModel = nullptr;
	std::list<std::unique_ptr<Swamp>> swamps;

	//押せるブロック
	size_t pushBlockSize = 10;
	FbxModel* pushBlockModel = nullptr;
	std::list<std::unique_ptr<PushBlock>> pushBlocks;

	//動かせないブロック
	size_t blockSize = 17;
	FbxModel* blockModel = nullptr;
	std::list<std::unique_ptr<Block>> blocks;

	//梯子
	size_t ladderSize = 1;
	FbxModel* ladderModel = nullptr;
	std::list<std::unique_ptr<Ladder>> ladders;

	//動く敵
	size_t moveEnemySize = 10;
	std::list<std::unique_ptr<MoveEnemy>> moveEnemys;

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

	//操作説明
	Sprite* playOpSprite = nullptr;
	Sprite* clearOpSprite = nullptr;
	Sprite* menuOpSprite = nullptr;

	//タイトルUI用タイマー
	int titleTimer;
	int titleAssistTime = 300;

	//当たり判定キューブモデル
	CubeModel* cubeModel = nullptr;
	//当たり判定キューブオブジェクト
	CubeObject3D* playerColBox = nullptr;

	/*std::list<std::unique_ptr<CubeObject3D>> playerColBox;*/
	std::list<std::unique_ptr<CubeObject3D>> buttonColBoxs;
	std::list<std::unique_ptr<CubeObject3D>> pushBlockColBoxs;
	std::list<std::unique_ptr<CubeObject3D>> blockColBoxs;
	std::list<std::unique_ptr<CubeObject3D>> pushButtonColBoxs;
	std::list<std::unique_ptr<CubeObject3D>> pushButtonBlockColBoxs;

	//お試し用
	CubeObject3D* cube = nullptr;

	//ゴール判定
	Goal* goal = nullptr;

	//演出
	float alpha = 0.0f;
	bool isback = false;
	bool noPush = false;

	//音
	//BGM
	AudioManager* playBGM;
	float playBGMVolume = 0.4f;

	//決定音
	AudioManager* pickSE;
	float pickSEVolume = 0.05f;

	//メニュー
	Menu* menu;
	
	//判定用 
	int i = 0;
	int j = 0;
};
