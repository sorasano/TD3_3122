#include "GameScene.h"
#include "FbxLoader.h"
#include<fstream>
#include "sstream"
#include "stdio.h"
#include "string.h"
#include "vector"
#include "imgui.h"
#include"Vector3.h"
#include<math.h>

#define PI 3.1415

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete clearSprite;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	this->dxCommon_ = dxCommon;
	this->input_ = input;

	//当たり判定キューブモデル
	cubeModel = new CubeModel();
	cubeModel->CreateBuffers(dxCommon_->GetDevice());
	cubeModel->SetImageData(XMFLOAT4(255, 0, 0, 1));

	//カメラ初期化
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 10,-10 });

	//当たり判定キューブオブジェクト
	CubeObject3D::SetCamera(camera_.get());
	CubeObject3D::SetDevice(dxCommon_->GetDevice());
	CubeObject3D::SetInput(input_);
	CubeObject3D::CreateGraphicsPipeline();
	//スプライトマネージャー
	SpriteManager::SetDevice(dxCommon->GetDevice());
	spriteManager = new SpriteManager;
	spriteManager->Initialize();

	//ライト(影)
	light = new Light;
	light->Initialize();

	//ライト生成
	lightGroup0 = LightGroup::Create();

	lightGroup0->SetDirLightActive(0, true);
	lightGroup0->SetDirLightActive(1, false);
	lightGroup0->SetDirLightActive(2, false);
	lightGroup0->SetPointLightActive(0, false);
	lightGroup0->SetPointLightActive(1, false);
	lightGroup0->SetPointLightActive(2, false);
	lightGroup0->SetSpotLightActive(0, false);
	lightGroup0->SetSpotLightActive(1, false);
	lightGroup0->SetSpotLightActive(2, false);
	/*lightGroup->SetCircleShadowActive(0, true);*/

	//ライト生成
	lightGroup1 = LightGroup::Create();

	lightGroup1->SetDirLightActive(0, true);
	lightGroup1->SetDirLightActive(1, false);
	lightGroup1->SetDirLightActive(2, false);
	lightGroup1->SetPointLightActive(0, false);
	lightGroup1->SetPointLightActive(1, false);
	lightGroup1->SetPointLightActive(2, false);
	lightGroup1->SetSpotLightActive(0, false);
	lightGroup1->SetSpotLightActive(1, false);
	lightGroup1->SetSpotLightActive(2, false);
	///*lightGroup1->SetCircleShadowActive(0, true);*/

	//FBX読み込み
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	groundModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/grassFiled.png");
	blockModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/gray1x1.png");

	enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/red1x1.png");
	enemyModel2 = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/green1x1.png");
	enemyEyeModel = FbxLoader::GetInstance()->LoadModelFromFile("enemyEye", "Resources/color/yellow1x1.png");
	cameraEnemyModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/yellow1x1.png");
	buttonModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/yellow1x1.png");
	bombModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/red1x1.png");
	swampModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/brown1x1.png");
	pushBlockModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/blue1x1.png");
	ladderModel = FbxLoader::GetInstance()->LoadModelFromFile("ladder", "Resources/color/brown1x1.png");
	modelTree = FbxLoader::GetInstance()->LoadModelFromFile("Tree3", "Resources/black.png");

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLight(light);
	FbxObject3D::SetLightGroup(lightGroup0);
	FbxObject3D::CreateGraphicsPipelineLightView();
	FbxObject3D::CreateGraphicsPipeline();

	//地面
	groundObject = new FbxObject3D;
	groundObject->Initialize();
	groundObject->SetModel(groundModel);

	//----------プレイヤー--------
	playerColBox = new CubeObject3D();
	playerColBox->Initialize();
	playerColBox->SetModel(cubeModel);
	Player::SetInput(input_);
	Player::SetDXInput(dxInput);
	player = new Player;
	player->Initialize(playerColBox);

	////=============ギミック===============

	//各ステージの開始位置
	float stage1 = 0.0f;
	float stage2 = 150.0f;
	float stage3 = 290.0f;
	float stage4 = 400.0f;
	float stage5 = 600.0f;

	//---------敵---------

	enemyCsv = new CSVLoader;
	enemyCsv->LoadCSV("Resources/csv/enemy.csv");

	for (int i = 0; i < enemySize; i++)
	{
		std::unique_ptr<Enemy>newObject = std::make_unique<Enemy>();
		newObject->Initialize(enemyModel, enemyModel2, enemyEyeModel, player);
		newObject->SetModel(enemyModel);

		newObject->SetPosition(enemyCsv->GetPosition(i));
		newObject->SetScale(enemyCsv->GetScale(i));
		newObject->Setrotate(enemyCsv->GetRotation(i));

		enemys.push_back(std::move(newObject));
	}

	//---------監視カメラ---------
	for (int i = 0; i < cameraEnemySize; i++) {
		cameraEnemy[i] = new CameraEnemy;
		cameraEnemy[i]->Initialize(cameraEnemyModel, enemyEyeModel, player);
	}

	//stage1
	cameraEnemy[0]->SetPosition({ stage1 + 50,5,0 });
	cameraEnemy[1]->SetPosition({ stage1 + 60,5,0 });

	//stage2
	cameraEnemy[2]->SetPosition({ stage2 + 27,5,0 });
	cameraEnemy[3]->SetPosition({ stage2 + 40,5,0 });

	//stage4
	cameraEnemy[4]->SetPosition({ stage4 + 50.0f,5,0 });
	cameraEnemy[5]->SetPosition({ stage4 + 55.5f,5,0 });
	cameraEnemy[6]->SetPosition({ stage4 + 61.0f,5,0 });
	cameraEnemy[7]->SetPosition({ stage4 + 66.5f,5,0 });

	//--------ボタン----------

	for (int i = 0; i < buttonSize; i++) {
		buttonColBox[i] = new CubeObject3D();
		buttonColBox[i]->Initialize();
		buttonColBox[i]->SetModel(cubeModel);

		button[i] = new Button;
		button[i]->Initialize(buttonModel, player, buttonColBox[i]);

	}

	//stage1
	button[0]->SetPositionX(stage1 + 70);
	button[0]->SetBlockPositionX(stage1 + 73);


	button[1]->SetPositionX(stage1 + 79);
	button[1]->SetBlockPositionX(stage1 + 91);

	button[2]->SetPositionX(stage1 + 100);
	button[2]->SetBlockPositionX(stage1 + 109);

	button[3]->SetPositionX(stage1 + 100);
	button[3]->SetBlockPositionX(stage1 + 112);


	button[4]->SetPositionX(stage1 + 118);
	button[4]->SetBlockPositionX(stage1 + 135);

	button[5]->SetPositionX(stage1 + 122);
	button[5]->SetBlockPositionX(stage1 + 138);

	button[6]->SetPositionX(stage1 + 126);
	button[6]->SetBlockPositionX(stage1 + 141);

	//stage2
	button[7]->SetPositionX(stage2 + 50);
	button[7]->SetBlockPositionX(stage2 + 53);


	button[8]->SetPositionX(stage2 + 59);
	button[8]->SetBlockPositionX(stage2 + 71);

	button[9]->SetPositionX(stage2 + 80);
	button[9]->SetBlockPositionX(stage2 + 89);

	button[10]->SetPositionX(stage2 + 80);
	button[10]->SetBlockPositionX(stage2 + 92);


	button[11]->SetPositionX(stage2 + 98);
	button[11]->SetBlockPositionX(stage2 + 115);

	button[12]->SetPositionX(stage2 + 102);
	button[12]->SetBlockPositionX(stage2 + 118);

	button[13]->SetPositionX(stage2 + 106);
	button[13]->SetBlockPositionX(stage2 + 121);

	//stage4
	button[14]->SetPositionX(stage4 + 74);
	button[14]->SetBlockPositionX(stage4 + 86);

	button[15]->SetPositionX(stage4 + 92);
	button[15]->SetBlockPositionX(stage4 + 108);

	button[16]->SetPositionX(stage4 + 114);
	button[16]->SetBlockPositionX(stage4 + 130);

	button[17]->SetPositionX(stage4 + 135);
	button[17]->SetBlockPositionX(stage4 + 144);
	button[18]->SetPositionX(stage4 + 135);
	button[18]->SetBlockPositionX(stage4 + 147);

	button[19]->SetPositionX(stage4 + 153);
	button[19]->SetBlockPositionX(stage4 + 170);
	button[20]->SetPositionX(stage4 + 157);
	button[20]->SetBlockPositionX(stage4 + 173);
	button[21]->SetPositionX(stage4 + 161);
	button[21]->SetBlockPositionX(stage4 + 176);

	//--------爆弾--------	
	for (int i = 0; i < buttonSize; i++) {

		bomb[i] = new Bomb();
		bomb[i]->Initialize(bombModel, player);
	}

	//stage3
	bomb[0]->SetPositionX(stage3 + 15);
	bomb[1]->SetPositionX(stage3 + 20);
	bomb[2]->SetPositionX(stage3 + 25);

	bomb[3]->SetPosition(XMFLOAT3(stage3 + 35, 2, -1));
	bomb[4]->SetPosition(XMFLOAT3(stage3 + 40, 1, -1));
	bomb[5]->SetPosition(XMFLOAT3(stage3 + 45, 2, -1));

	bomb[6]->SetPosition(XMFLOAT3(stage3 + 90, 2, -1));
	bomb[7]->SetPosition(XMFLOAT3(stage3 + 96, 1, -1));
	bomb[8]->SetPosition(XMFLOAT3(stage3 + 102, 2, -1));

	//stage4
	bomb[9]->SetPosition(XMFLOAT3(stage4 + 74, 2, -1));
	bomb[10]->SetPosition(XMFLOAT3(stage4 + 86, 1, -1));

	bomb[11]->SetPosition(XMFLOAT3(stage4 + 118, 1, -1));
	bomb[12]->SetPosition(XMFLOAT3(stage4 + 122, 1, -1));
	bomb[13]->SetPosition(XMFLOAT3(stage4 + 126, 1, -1));
	bomb[14]->SetPosition(XMFLOAT3(stage4 + 130, 1, -1));

	bomb[15]->SetPosition(XMFLOAT3(stage4 + 155, 1, -1));
	bomb[16]->SetPosition(XMFLOAT3(stage4 + 159, 1, -1));

	//----------沼----------
	for (int i = 0; i < swampSize; i++) {
		swamp[i] = new Swamp();
		swamp[i]->Initialize(swampModel, player);
	}

	//stage3
	swamp[0]->SetPositionX(stage3);

	swamp[1]->SetPositionX(stage3 + 8);
	swamp[1]->SetScaleX(0.04);

	swamp[2]->SetPositionX(stage3 + 66);
	swamp[2]->SetScaleX(0.2);

	//stage4
	swamp[3]->SetPositionX(stage4 + 23);
	swamp[3]->SetScaleX(0.15);

	swamp[4]->SetPositionX(stage4 + 48);
	swamp[4]->SetScaleX(0.38);


	swamp[5]->SetPositionX(stage4 + 96);
	swamp[5]->SetScaleX(0.02);

	swamp[6]->SetPositionX(stage4 + 100);
	swamp[6]->SetScaleX(0.02);

	swamp[7]->SetPositionX(stage4 + 104);
	swamp[7]->SetScaleX(0.02);

	swamp[8]->SetPositionX(stage4 + 108);
	swamp[8]->SetScaleX(0.02);


	swamp[9]->SetPositionX(stage4 + 143);
	swamp[9]->SetScaleX(0.15);

	for (int i = 0; i < ladderSize; i++) {

		ladder[i] = new Ladder();
		ladder[i]->Initialize(ladderModel, player);

	}

	ladder[0]->SetPosition({ stage5 + 23, 1, -1 });
	ladder[1]->SetPosition({ stage5 + 26, 1, -1 });
	ladder[2]->SetPosition({ stage5 + 26, 2, -1 });
	ladder[3]->SetPosition({ stage5 + 26, 3, -1 });
	ladder[4]->SetPosition({ stage5 + 26, 4, -1 });
	ladder[5]->SetPosition({ stage5 + 26, 5, -1 });


	//動く敵

	for (int i = 0; i < moveEnemySize; i++) {

		moveEnemy[i] = new MoveEnemy();
		moveEnemy[i]->Initialize(player);

	}

	moveEnemy[0]->SetPositionX(stage5 + 30.0f);
	moveEnemy[1]->SetPositionX(stage5 + 40.0f);

	//----------動かせるブロック----------
	PushBlock::SetInput(input_);
	/*PushBlock::SetDXInput(dxInput);*/
	for (int i = 0; i < pushBlockSize; i++) {
		pushBlockColBox[i] = new CubeObject3D();
		pushBlockColBox[i]->Initialize();
		pushBlockColBox[i]->SetModel(cubeModel);

		pushBlock[i] = new PushBlock();
		pushBlock[i]->Initialize(pushBlockModel, player, pushBlockColBox[i]);
	}

	pushBlock[0]->SetPositionX(stage5 + 5.0f);
	pushBlock[1]->SetPositionX(stage5 + 15.0f);
	/*pushBlock[2]->SetPositionX(15.0f);
	pushBlock[3]->SetPositionX(20.0f);
	pushBlock[4]->SetPositionX(30.0f);*/

	//----------ブロック----------
	for (int i = 0; i < blockSize; i++) {
		blockColBox[i]=new CubeObject3D();
		blockColBox[i]->Initialize();
		blockColBox[i]->SetModel(cubeModel);

		block[i] = new Block();
		block[i]->Initialize(blockModel, player, blockColBox[i]);
	}
	//stage1
	block[0]->SetPosition({ stage1 + 14,1,0 });
	block[1]->SetPosition({ stage1 + 20,1,0 });

	block[2]->SetPosition({ stage1 + 27,1,0 });
	block[3]->SetPosition({ stage1 + 37,1,0 });

	//stage2
	block[4]->SetPosition({ stage2 + 14,1,0 });
	block[5]->SetPosition({ stage2 + 20,1,0 });

	block[6]->SetPosition({ stage2 + 27,1,0 });
	block[7]->SetPosition({ stage2 + 37,1,0 });

	//stage3
	block[8]->SetPosition({ stage3 + 57,1,0 });
	block[9]->SetPosition({ stage3 + 63,1,0 });
	block[10]->SetPosition({ stage3 + 69,1,0 });
	block[11]->SetPosition({ stage3 + 75,1,0 });

	block[12]->SetPosition({ stage3 + 87,1,0 });
	block[13]->SetPosition({ stage3 + 93,1,0 });
	block[14]->SetPosition({ stage3 + 99,1,0 });
	block[15]->SetPosition({ stage3 + 105,1,0 });


	//block[0]->SetPositionX(17.0f);
	//block[1]->SetPositionX(33.0f);
	//block[2]->SetPositionX(40.0f);
	//block[3]->SetPositionX(50.0f);
	//block[4]->SetPositionX(60.0f);

	//----------押している間のスイッチ----------
	for (int i = 0; i < pushButtonSize; i++) {
		pushButtonColBox[i] = new CubeObject3D();
		pushButtonColBox[i]->Initialize();
		pushButtonColBox[i]->SetModel(cubeModel);

		pushButtonBlockColBox[i] = new CubeObject3D();
		pushButtonBlockColBox[i]->Initialize();
		pushButtonBlockColBox[i]->SetModel(cubeModel);
		

		pushButton[i] = new PushButton();
		pushButton[i]->Initialize(buttonModel, player, pushButtonColBox[i], pushButtonBlockColBox[i]);
	}

	pushButton[0]->SetPositionX(stage5 + 20);
	pushButton[0]->SetBlockPositionX(stage5 + 25);

	pushButton[1]->SetPositionX(stage5 + 10);
	pushButton[1]->SetBlockPositionX(stage5 + 30);


	//セーブ
	autoSave = new Autosave;
	autoSave->Initialize(player);

	//------テクスチャ------

	spriteManager->LoadFile(0, L"Resources/clear.png");
	spriteManager->LoadFile(1, L"Resources/gameover2.png");
	spriteManager->LoadFile(2, L"Resources/title.png");
	spriteManager->LoadFile(3, L"Resources/titleUI.png");
	spriteManager->LoadFile(4, L"Resources/color/white1x1.png");
	spriteManager->LoadFile(5, L"Resources/color/black1x1.png");
	spriteManager->LoadFile(6, L"Resources/playUI.png");

	//メニュー用
	spriteManager->LoadFile(7, L"Resources/Menu/menuBase.png");
	spriteManager->LoadFile(8, L"Resources/Menu/menuRestart.png");
	spriteManager->LoadFile(9, L"Resources/Menu/menuTitle.png");
	spriteManager->LoadFile(10, L"Resources/Menu/menuClose.png");

	//スプライト
	Sprite::SetDevice(dxCommon->GetDevice());
	Sprite::SetSpriteManager(spriteManager);
	Sprite::CreateGraphicsPipeLine();

	//---クリア---
	clearSprite = new Sprite();
	clearSprite->SetTextureNum(0);
	clearSprite->Initialize();
	//アンカーポイントをスプライトの中心に
	clearSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	clearSprite->SetScale(XMFLOAT2(600, 300));
	clearSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2));
	clearSprite->Update();
	//白染め
	whiteSprite = new Sprite();
	whiteSprite->SetTextureNum(4);
	whiteSprite->Initialize();
	whiteSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	whiteSprite->SetScale(XMFLOAT2(window_width, window_height));
	whiteSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2));
	whiteSprite->Update();


	//---ゲームオーバー---
	gameoverSprite = new Sprite();
	gameoverSprite->SetTextureNum(1);
	gameoverSprite->Initialize();
	gameoverSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	gameoverSprite->SetScale(XMFLOAT2(390, 254));
	gameoverSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2));
	gameoverSprite->Update();

	//黒染め
	/*alpha = 0;*/
	blackSprite = new Sprite();
	blackSprite->SetTextureNum(5);
	blackSprite->Initialize();
	blackSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	blackSprite->SetScale(XMFLOAT2(window_width, window_height));
	blackSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2));
	blackSprite->SetAlpha(alpha);
	blackSprite->Update();


	//---タイトル---
	titleSprite = new Sprite();
	titleSprite->SetTextureNum(2);
	titleSprite->Initialize();
	titleSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	titleSprite->SetScale(XMFLOAT2(309, 124));
	titleSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2 - 200));
	titleSprite->StartSway({ window_width / 2, window_height / 2 - 200 });
	titleSprite->Update();

	titleUISprite = new Sprite();
	titleUISprite->SetTextureNum(3);
	titleUISprite->Initialize();
	titleUISprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	titleUISprite->SetScale(XMFLOAT2(500, 100));
	titleUISprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2 + 200));
	titleUISprite->Update();

	//playUI
	playUISprite = new Sprite();
	playUISprite->SetTextureNum(6);
	playUISprite->Initialize();
	//playUISprite->SetAnchorPoint(XMFLOAT2(0.0f, 0.5f));
	playUISprite->SetScale(XMFLOAT2(250, 100));
	playUISprite->SetPosition(XMFLOAT2(0, window_height - 100));
	playUISprite->Update();

	//ゴール
	goal = new Goal();
	Goal::SetInput(input);
	Goal::SetDXInput(dxInput);
	goal->Initialize(whiteSprite, clearSprite, player);
	goal->SetClearPos(700);



	/*cube = new CubeObject3D();
	cube->Initialize();
	cube->SetModel(cubeModel);*/


	//オーディオ初期化
	AudioManager::StaticInitialize();
	//音
	titleBGM = new AudioManager();
	titleBGM->SoundLoadWave("Resources/Audio/titleBGM.wav");

	//メニュー
	menu = new Menu();
	menu->Initialize();
	Menu::SetInput(input_);
	Menu::SetDXInput(dxInput);

	//ツリー
	treeCsv = new CSVLoader;
	treeCsv->LoadCSV("Resources/csv/Tree1.csv");

	for (int i = 0; i < treeNum; i++)
	{
		std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
		newObject->Initialize();
		newObject->SetModel(modelTree);

		newObject->SetPosition(treeCsv->GetPosition(i));
		newObject->SetScale(treeCsv->GetScale(i));
		newObject->SetRotation(treeCsv->GetRotation(i));

		objectTree.push_back(std::move(newObject));
	}
}

void GameScene::Update()
{

	//タイトルBGM流す
	//titleBGM->SoundPlayWave(true, titleBGMVolume);
	//titleBGM->StopWave();

	//カメラ更新
	camera_->Update(player->GetPosition());
	//コントローラー更新
	dxInput->InputProcess();

	lightPos[0] = player->GetPosition().x;
	lightTarget[0] = player->GetPosition().x;

	//ライト
	light->SetEye(XMFLOAT3(lightPos));
	light->SetTarget(XMFLOAT3(lightTarget));
	light->SetDir(XMFLOAT3(lightDir));
	light->Update();

	//ライト更新
	lightGroup0->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup0->SetDirLightDir(0, XMVECTOR({ lightDir0[0],lightDir0[1], lightDir0[2],0 }));
	lightGroup0->SetDirLightColor(0, XMFLOAT3(lightColor0));

	lightGroup0->Update();

	//ライト更新
	lightGroup1->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup1->SetDirLightDir(0, XMVECTOR({ lightDir0[0],lightDir0[1], lightDir0[2],0 }));
	lightGroup1->SetDirLightColor(0, XMFLOAT3(lightColor0));
	lightGroup1->Update();

	//オブジェクト更新

	groundObject->SetPosition({ 90,0,0 });
	groundObject->SetScale({ 10.0f,0.01f,0.5f });
	groundObject->SetRotation({ 0.0f,0.0f,0.0f });
	groundObject->Update();

	switch (scene) {

	case PLAY:

		//シーン切り替え
		if (input_->PushKey(DIK_A) || input_->PushKey(DIK_D)) {
			titleSprite->StartFlipOut();
			scene = PLAY;
		}

		//シーン切り替え
		if (input_->PushKey(DIK_M) && !player->GetDeath()) {
			scene = MENU;
			menu->Reset();
		}
		else if (isClear) {
			scene = CLEAR;
		}


		////リセット
		//if (input_->PushKey(DIK_R)) {
		//	Reset();
		//}

		//ブロック

		for (int i = 0; i < blockSize; i++) {
			block[i]->SetScale({ 0.01f,0.01f,0.01f });
			block[i]->Update();
		}

		for (int i = 0; i < enemySize; i++) {
			enemy[i]->Update();
			enemydeg = enemy[0]->GetDeg();
		}

		for (int i = 0; i < cameraEnemySize; i++) {
			cameraEnemy[i]->Update();
			cameraEnemydeg = cameraEnemy[i]->GetDeg();
		}

		//ボタン
		for (int i = 0; i < buttonSize; i++) {
			button[i]->Update();
		}

		//爆弾
		for (int i = 0; i < bombSize; i++) {
			bomb[i]->Update();
		}

		//沼
		for (int i = 0; i < swampSize; i++) {
			swamp[i]->Update();
		}

		//梯子
		for (int i = 0; i < ladderSize; i++) {
			ladder[i]->Update();
		}

		//動く敵
		for (int i = 0; i < moveEnemySize; i++) {
			moveEnemy[i]->Update();
		}

		
		//タイトル
		titleSprite->Update();

	

	//動かせるブロック
	for (int i = 0; i < pushBlockSize; i++) {
		pushBlock[i]->Collision();
		if (pushBlock[i]->GetIsPush()) {
			//動かせるブロック同士の判定
			for (int j = 0; j < pushBlockSize; j++) {
				if (i != j) {
					pushBlock[i]->pushback(pushBlockColBox[j]);
					//押さないかどうか
					if (pushBlock[i]->GetNoPush()) {
						noPush = true;
					}
				}
			}
			//ギミックの判定追加場所
			//動かないブロックとの判定
			for (int j = 0; j < blockSize; j++) {
				if (i != j) {
					pushBlock[i]->pushback(blockColBox[j]);
					//押さないかどうか
					if (pushBlock[i]->GetNoPush()) {
						noPush = true;
					}
				}
			}
			//押している間のスイッチ
			for (int j = 0; j < pushButtonSize; j++) {
				if (i != j) {
					pushBlock[i]->pushback(pushButtonBlockColBox[j]);
					//押さないかどうか
					if (pushBlock[i]->GetNoPush()) {
						noPush = true;
					}
				}
			}

			//ギミックの判定追加場所ここまで
			if (noPush) {
				pushBlock[i]->NoPush();
			}
			else {
				pushBlock[i]->Push();
			}
		}

		pushBlock[i]->Update();
	}
	noPush = false;

	//押している間のスイッチ
	for (int i = 0; i < pushButtonSize; i++) {
		for (int j = 0; j < pushBlockSize; j++) {
			pushButton[i]->ButtonCol(pushBlockColBox[j]);
		}
		pushButton[i]->ButtonCol(playerColBox);
		pushButton[i]->Update();
	}

      //プレイヤー
		player->Update();

		//タイトルUI
		if (titleSprite->isflipEase == false) {
			titleUISprite->color.w = sin(clock() / 100);
			titleTimer++;
		}

		/*clearSprite->Update();*/
		gameoverSprite->Update();
		blackSprite->Update();
		titleUISprite->Update();
		playUISprite->Update();

		//オートセーブ
		autoSave->Update();

		//ゴール
		goal->Update();

		//ゲームオーバー演出
		if (player->GetDeath()) {
			if (isback == false) {
				alpha += 0.05f;
				player->SetAlpha(alpha);
				if (alpha >= 1.0f) {
					isback = true;
					//リセット
					Reset(false);
				}
			}
		}

		if (isback == true) {
			if (input_->PushKey(DIK_A) || input_->PushKey(DIK_D)) {
				//アルファ値がこの値を超えたら演出スキップできる(alphaは1から低くなっていく)
				if (alpha <= 0.8f) {
					alpha = 0.0f;
					player->SetAlpha(alpha);
					isback = false;
				}
			}
			player->SetisDeath(false);
			player->SetAlpha(alpha);
			alpha -= 0.005f;
			if (alpha <= 0.0f) {
				alpha = 0.0f;
				player->SetAlpha(alpha);
				isback = false;
			}
		}

		blackSprite->SetAlpha(alpha);

		break;

	case MENU:

		//メニュー
		menu->Update();


		//シーン切り替えが起こったら
		if (menu->GetIsSerect()) {

			scene = PLAY;

			if (menu->GetSerect() == MENURESET) {
				Reset(false);
			}
			else if (menu->GetSerect() == MENUTITLE) {
				Reset(true);
			}
			else if (menu->GetSerect() == MENUCLOSE) {
			}


		}

		break;

	case CLEAR:

		//シーン切り替え
		if (input_->PushKey(DIK_SPACE)) {
			Reset(true);
		}

	}

	//Tree1
	//スペースキーでファイル読み込み更新
	if (input_->TriggerKey(DIK_SPACE))
	{
		treeCsv->LoadCSV("Resources/Tree1.csv");
		int i = 0;
		for (std::unique_ptr<FbxObject3D>& object : objectTree)
		{
			object->SetPosition(treeCsv->GetPosition(i));
			object->SetScale(treeCsv->GetScale(i));
			object->SetRotation(treeCsv->GetRotation(i));
			i++;
		}
	}
	for (std::unique_ptr<FbxObject3D>& object : objectTree)
	{
		object->Update();
	}
}

void GameScene::Draw()
{
	//-------背景スプライト描画処理-------//

	//ImGui::Begin("Light");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(500, 200));
	//ImGui::InputFloat3("lightTarget", lightTarget);
	//ImGui::InputFloat3("lightPos", lightPos);
	//ImGui::End();

	DrawFBX();

	////-------前景スプライト描画処理-------//

	//DrawSprite();

}

void GameScene::DrawFBXLightView()
{
	/*if(camera_->GetEye().x )*/

	groundObject->DrawLightView(dxCommon_->GetCommandList());

	//Tree1
	for (std::unique_ptr<FbxObject3D>& object : objectTree)
	{
		object->DrawLightView(dxCommon_->GetCommandList());
	}

	//ブロック
	for (int i = 0; i < blockSize; i++) {
		if(camera_->GetEye().x - block[i]->GetPosition().x >= -20 && camera_->GetEye().x - block[i]->GetPosition().x <= 20)
		block[i]->DrawLightView(dxCommon_->GetCommandList());
	}

	//プレイヤー
	player->DrawLightView(dxCommon_->GetCommandList());

	//敵
	for (int i = 0; i < enemySize; i++) {
		if (camera_->GetEye().x - enemy[i]->GetPosition().x >= -20 && camera_->GetEye().x - enemy[i]->GetPosition().x <= 20)
		enemy[i]->DrawLightView(dxCommon_->GetCommandList());
	}

	//監視カメラ
	for (int i = 0; i < cameraEnemySize; i++) {
		if (camera_->GetEye().x - cameraEnemy[i]->GetPosition().x >= -20 && camera_->GetEye().x - cameraEnemy[i]->GetPosition().x <= 20)
		cameraEnemy[i]->DrawLightView(dxCommon_->GetCommandList());
	}

	//ボタン
	for (int i = 0; i < buttonSize; i++) {
		if (camera_->GetEye().x - button[i]->GetPosition().x >= -40 && camera_->GetEye().x - button[i]->GetPosition().x <= 40)
		button[i]->DrawLightView(dxCommon_->GetCommandList());
	}

	//爆弾
	for (int i = 0; i < bombSize; i++) {
		if (camera_->GetEye().x - bomb[i]->GetPosition().x >= -20 && camera_->GetEye().x - bomb[i]->GetPosition().x <= 20)
		bomb[i]->DrawLightView(dxCommon_->GetCommandList());
	}

	//沼
	for (int i = 0; i < swampSize; i++) {
		if (camera_->GetEye().x - swamp[i]->GetPosition().x >= -20 && camera_->GetEye().x - swamp[i]->GetPosition().x <= 20)
		swamp[i]->DrawLightView(dxCommon_->GetCommandList());
	}

	//梯子
	for (int i = 0; i < ladderSize; i++) {
		if (camera_->GetEye().x - ladder[i]->GetPosition().x >= -20 && camera_->GetEye().x - ladder[i]->GetPosition().x <= 20)
		ladder[i]->DrawLightView(dxCommon_->GetCommandList());
	}
	//動かせるブロック
	for (int i = 0; i < pushBlockSize; i++) {
		if (camera_->GetEye().x - pushBlock[i]->GetPosition().x >= -20 && camera_->GetEye().x - pushBlock[i]->GetPosition().x <= 20)
		pushBlock[i]->DrawLightView(dxCommon_->GetCommandList());
	}
	//押している間のスイッチ
	for (int i = 0; i < pushButtonSize; i++) {
		if (camera_->GetEye().x - pushButton[i]->GetPosition().x >= -40 && camera_->GetEye().x - pushButton[i]->GetPosition().x <= 40)
		pushButton[i]->DrawLightView(dxCommon_->GetCommandList());
	}

	//動く敵
	for (int i = 0; i < moveEnemySize; i++) {
		if (camera_->GetEye().x - moveEnemy[i]->GetPosition().x >= -20 && camera_->GetEye().x - moveEnemy[i]->GetPosition().x <= 20)
		moveEnemy[i]->DrawLightView(dxCommon_->GetCommandList());
	}
}

void GameScene::DrawFBX()
{
	groundObject->Draw(dxCommon_->GetCommandList());

	for (std::unique_ptr<FbxObject3D>& object : objectTree)
	{
		object->Draw(dxCommon_->GetCommandList());
	}

	//ブロック
	for (int i = 0; i < blockSize; i++) {
		if (camera_->GetEye().x - block[i]->GetPosition().x >= -20 && camera_->GetEye().x - block[i]->GetPosition().x <= 20)
		block[i]->Draw(dxCommon_->GetCommandList());
	}
	//プレイヤー
	player->Draw(dxCommon_->GetCommandList());
	//敵
	for (int i = 0; i < enemySize; i++) {
		if (camera_->GetEye().x - enemy[i]->GetPosition().x >= -20 && camera_->GetEye().x - enemy[i]->GetPosition().x <= 20)
		enemy[i]->Draw(dxCommon_->GetCommandList());
	}

	//監視カメラ
	for (int i = 0; i < cameraEnemySize; i++) {
		if (camera_->GetEye().x - cameraEnemy[i]->GetPosition().x >= -20 && camera_->GetEye().x - cameraEnemy[i]->GetPosition().x <= 20)
		cameraEnemy[i]->Draw(dxCommon_->GetCommandList());
	}

	//ボタン
	for (int i = 0; i < buttonSize; i++) {
		if (camera_->GetEye().x - button[i]->GetPosition().x >= -40 && camera_->GetEye().x - button[i]->GetPosition().x <= 40)
		button[i]->Draw(dxCommon_->GetCommandList());
	}

	//爆弾
	for (int i = 0; i < bombSize; i++) {
		if (camera_->GetEye().x - bomb[i]->GetPosition().x >= -20 && camera_->GetEye().x - bomb[i]->GetPosition().x <= 20)
		bomb[i]->Draw(dxCommon_->GetCommandList());
	}

	//沼
	for (int i = 0; i < swampSize; i++) {
		if (camera_->GetEye().x - swamp[i]->GetPosition().x >= -20 && camera_->GetEye().x - swamp[i]->GetPosition().x <= 20)
		swamp[i]->Draw(dxCommon_->GetCommandList());
	}

	//梯子
	for (int i = 0; i < ladderSize; i++) {
		if (camera_->GetEye().x - ladder[i]->GetPosition().x >= -20 && camera_->GetEye().x - ladder[i]->GetPosition().x <= 20)
		ladder[i]->Draw(dxCommon_->GetCommandList());
	}

	//動く敵
	for (int i = 0; i < moveEnemySize; i++) {
		if (camera_->GetEye().x - moveEnemy[i]->GetPosition().x >= -20 && camera_->GetEye().x - moveEnemy[i]->GetPosition().x <= 20)
		moveEnemy[i]->Draw(dxCommon_->GetCommandList());
	}
	//動かせるブロック
	for (int i = 0; i < pushBlockSize; i++) {
		if (camera_->GetEye().x - pushBlock[i]->GetPosition().x >= -20 && camera_->GetEye().x - pushBlock[i]->GetPosition().x <= 20)
		pushBlock[i]->Draw(dxCommon_->GetCommandList());
	}
	//押している間のスイッチ
	for (int i = 0; i < pushButtonSize; i++) {
		if (camera_->GetEye().x - pushButton[i]->GetPosition().x >= -40 && camera_->GetEye().x - pushButton[i]->GetPosition().x <= 40)
		pushButton[i]->Draw(dxCommon_->GetCommandList());
	}
}

void GameScene::DrawSprite()
{
	//-------前景スプライト描画処理-------//

	if (scene == PLAY) {

		if (titleSprite->endFlip == false) {
			titleSprite->Draw(dxCommon_->GetCommandList());
		}

		if (titleSprite->isflipEase == false && titleTimer >= titleAssistTime) {
			titleUISprite->Draw(dxCommon_->GetCommandList());
		}

		//blackSprite->Draw(dxCommon_->GetCommandList());

		goal->Draw(dxCommon_->GetCommandList());

		if (player->GetDeath() == false) {
			playUISprite->Draw(dxCommon_->GetCommandList());
		}
	}

	if (scene == MENU) {
		menu->Draw(dxCommon_->GetCommandList());
	}

}

void GameScene::Reset(bool isFirst)
{
	//プレイヤー
	XMFLOAT2 savePos = autoSave->GetSavePos();
	player->Reset(savePos);

	//動く敵
	for (int i = 0; i < moveEnemySize; i++) {
		moveEnemy[i]->Reset();
	}

	//最初からなら
	if (isFirst) {
		isClear = false;
		titleSprite->Reset();
		titleTimer = 0;

		//初期地点
		XMFLOAT2 startPos = autoSave->GetStartPos();
		player->Reset(startPos);

	}

}


DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}

void GameScene::SetSRV(ID3D12DescriptorHeap* SRV)
{
	/*for (std::unique_ptr<FbxObject3D>& object : objectStone)
	{
		object->SetSRV(SRV);
	}
	objectTree->SetSRV(SRV);
	object1->SetSRV(SRV);*/

	groundObject->SetSRV(SRV);

	for (std::unique_ptr<FbxObject3D>& object : objectTree)
	{
		object->SetSRV(SRV);
	}

	//ブロック
	for (int i = 0; i < blockSize; i++) {
		block[i]->SetSRV(SRV);
	}

	//プレイヤー
	player->SetSRV(SRV);

	//敵
	for (int i = 0; i < enemySize; i++) {
		enemy[i]->SetSRV(SRV);
	}

	//監視カメラ
	for (int i = 0; i < cameraEnemySize; i++) {
		cameraEnemy[i]->SetSRV(SRV);
	}

	//ボタン
	for (int i = 0; i < buttonSize; i++) {
		button[i]->SetSRV(SRV);
	}

	//爆弾
	for (int i = 0; i < bombSize; i++) {
		bomb[i]->SetSRV(SRV);
	}

	//沼
	for (int i = 0; i < swampSize; i++) {
		swamp[i]->SetSRV(SRV);
	}

	//梯子
	for (int i = 0; i < ladderSize; i++) {
		ladder[i]->SetSRV(SRV);
	}

	//動く敵
	for (int i = 0; i < moveEnemySize; i++) {
		moveEnemy[i]->SetSRV(SRV);
	}
	//動かせるブロック
	for (int i = 0; i < pushBlockSize; i++) {
		pushBlock[i]->SetSRV(SRV);
	}
	//押している間のスイッチ
	for (int i = 0; i < pushButtonSize; i++) {
		pushButton[i]->SetSRV(SRV);
	}
}
