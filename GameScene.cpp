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
	lightGroup0->SetShadowActive(0, false);

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
	lightGroup1->SetShadowActive(0, true);

	//FBX読み込み
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	model0 = FbxLoader::GetInstance()->LoadModelFromFile("key", "Resources/key.png");
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/white1x1.png");
	model2 = FbxLoader::GetInstance()->LoadModelFromFile("light", "Resources/white1x1.png");
	blockModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/gray1x1.png");
	playerModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/blue1x1.png");
	enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/red1x1.png");
	enemyModel2 = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/green1x1.png");
	enemyEyeModel = FbxLoader::GetInstance()->LoadModelFromFile("enemyEye", "Resources/transparentYellow1x1.png");
	cameraEnemyModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/yellow1x1.png");
	buttonModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/yellow1x1.png");
	boxModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/cube/Crate.jpg");

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLightGroup(lightGroup0);
	FbxObject3D::CreateGraphicsPipeline();

	//デバイスをセット
	FbxObject3D2::SetDevice(dxCommon_->GetDevice());
	FbxObject3D2::SetCamera(camera_.get());
	FbxObject3D2::SetLightGroup(lightGroup0);
	FbxObject3D2::CreateGraphicsPipeline();

	//オブジェクト初期化
	object0 = new FbxObject3D;
	object0->Initialize();
	object0->SetModel(model0);

	object1 = new FbxObject3D2;
	object1->Initialize();
	object1->SetModel(model1);

	object2 = new FbxObject3D;
	object2->Initialize();
	object2->SetModel(model2);

	//ブロック
	for (int i = 0; i < blockSize; i++) {
		blockObject[i] = new FbxObject3D2;
		blockObject[i]->Initialize();
		blockObject[i]->SetModel(blockModel);
	}

	//ブロック
	blockObject[0]->SetPosition({ 14,1,0 });
	blockObject[1]->SetPosition({ 20,1,0 });

	blockObject[2]->SetPosition({ 27,1,0 });
	blockObject[3]->SetPosition({ 37,1,0 });

	//プレイヤー
	playerColBox = new CubeObject3D();
	playerColBox->Initialize();
	playerColBox->SetModel(cubeModel);
	Player::SetInput(input_);
	Player::SetDXInput(dxInput);
	player = new Player;
	player->Initialize(playerModel, playerColBox);

	//敵
	for (int i = 0; i < enemySize; i++) {
		enemy[i] = new Enemy;
		enemy[i]->Initialize(enemyModel, enemyModel2, enemyEyeModel, player);
	}
	//敵
	enemy[0]->SetPosition({ 17,1,1 });
	enemy[1]->SetPosition({ 23,1,1 });
	enemy[2]->SetPosition({ 30,1,1 });
	enemy[3]->SetPosition({ 34,1,1 });
	enemy[4]->SetPosition({ 66,1,1 });

	enemy[5]->SetPosition({ 76,1,1 });
	enemy[6]->SetPosition({ 90.5,1,1 });

	enemy[7]->SetPosition({ 100.0,1,1 });

	enemy[8]->SetPosition({ 104.5,1,1 });
	enemy[9]->SetPosition({ 109.0,1,1 });

	//監視カメラ
	for (int i = 0; i < cameraEnemySize; i++) {
		cameraEnemy[i] = new CameraEnemy;
		cameraEnemy[i]->Initialize(cameraEnemyModel, enemyEyeModel, player);
	}
	cameraEnemy[0]->SetPosition({ 40,5,0 });

	//ボタン

	float startPos3 = 50;

	Button::SetInput(input_);
	Button::SetDXInput(dxInput);
	for (int i = 0; i < buttonSize; i++) {
		ButtonColBox[i] = new CubeObject3D();
		ButtonColBox[i]->Initialize();
		ButtonColBox[i]->SetModel(cubeModel);

		button[i] = new Button;
		button[i]->Initialize(buttonModel, player, ButtonColBox[i]);

	}

	button[0]->SetPositionX(startPos3);
	button[0]->SetBlockPositionX(startPos3 + 3);


	button[1]->SetPositionX(startPos3 + 9);
	button[1]->SetBlockPositionX(startPos3 + 21);

	button[2]->SetPositionX(startPos3 + 30);
	button[2]->SetBlockPositionX(startPos3 + 39);

	button[3]->SetPositionX(startPos3 + 30);
	button[3]->SetBlockPositionX(startPos3 + 42);


	button[4]->SetPositionX(startPos3 + 48);
	button[4]->SetBlockPositionX(startPos3 + 65);

	button[5]->SetPositionX(startPos3 + 52);
	button[5]->SetBlockPositionX(startPos3 + 68);

	button[6]->SetPositionX(startPos3 + 56);
	button[6]->SetBlockPositionX(startPos3 + 71);

	//box

	Box::SetInput(input_);
	Box::SetDXInput(dxInput);
	box = new Box();
	box->Initialize(boxModel, player);

	//------テクスチャ------

	spriteManager->LoadFile(0, L"Resources/clear.png");
	spriteManager->LoadFile(1, L"Resources/gameover2.png");
	spriteManager->LoadFile(2, L"Resources/title.png");
	spriteManager->LoadFile(3, L"Resources/titleUI.png");
	spriteManager->LoadFile(4, L"Resources/white1x1.png");

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
	clearSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2 ));
	clearSprite->Update();
	//白染め
	whiteSprite=new Sprite();
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
	titleUISprite->SetScale(XMFLOAT2(282, 71));
	titleUISprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2 + 200));
	titleUISprite->Update();

	//セーブ
	autoSave = new Autosave;
	autoSave->Initialize(player);

	//ゴール
	goal = new Goal();
	goal->Initialize(whiteSprite,clearSprite, player);
	goal->SetClearPos(startPos3 + 71 + 10);

}

void GameScene::Update()
{
	//カメラ更新
	//camera_->DebugUpdate();
	camera_->Update(player->GetPosition());
	//コントローラー更新
	dxInput->InputProcess();

	//ライト更新
	lightGroup0->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup0->SetDirLightDir(0, XMVECTOR({ lightDir0[0],lightDir0[1], lightDir0[2],0 }));
	lightGroup0->SetDirLightColor(0, XMFLOAT3(lightColor0));

	/*lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos0));
	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor0));
	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten0));*/

	/*lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0],circleShadowDir[1], circleShadowDir[2],0 }));
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3({0,1,0}));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));*/

	/*lightGroup0->SetShadowLightPos(0, XMFLOAT3(shadowLightPos), camera_->GetTraget(), camera_->GetUp());*/

	lightGroup0->Update();

	//ライト更新
	lightGroup1->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup1->SetDirLightDir(0, XMVECTOR({ lightDir0[0],lightDir0[1], lightDir0[2],0 }));
	lightGroup1->SetDirLightColor(0, XMFLOAT3(lightColor0));
	lightGroup1->SetShadowLightPos(0, XMFLOAT3(shadowLightPos), camera_->GetTraget(), camera_->GetUp());
	lightGroup1->Update();

	//オブジェクト更新
	rotation0.y += 0.02;
	object0->SetPosition({ 0,3,0 });
	object0->SetScale({ 0.2f,0.1f,0.4f });
	object0->SetRotation(rotation0);
	object0->Update();

	object1->SetPosition({ 0,0,0 });
	object1->SetScale({ 10.0f,0.01f,0.5f });
	object1->SetRotation({ 0.0f,0.0f,0.0f });
	object1->Update();

	object2->SetPosition(XMFLOAT3(shadowLightPos));
	object2->SetScale({ 0.2f,0.1f,0.4f });
	object2->SetRotation({ 0,0,0 });
	object2->Update();

	//ブロック

	for (int i = 0; i < blockSize; i++) {
		blockObject[i]->SetScale({ 0.01f,0.01f,0.01f });
		blockObject[i]->SetRotation({ 0,0,0 });
		blockObject[i]->Update();
	}

	//プレイヤー
	player->Update();
	playerpos = player->GetPosition();

	//復活
	if (input_->PushKey(DIK_E)) {
		player->SetisDeath(false);
		if (isClear) {
			isClear = false;
			player->SetPosition(XMFLOAT3(0, 1, -1));
		}
		else {
			XMFLOAT2 savePos = autoSave->GetSavePos();
			player->SetPosition(XMFLOAT3(savePos.x, savePos.y, -1));
		}
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

	//スプライト

	//タイトル
	if (input_->PushKey(DIK_A) || input_->PushKey(DIK_D)) {
		titleSprite->StartFlipOut();
	}
	titleSprite->Update();

	//タイトルUI
	if (titleSprite->isflipEase == false) {
		titleUISprite->color.w = sin(clock() / 100);
		titleTimer++;
	}

	/*clearSprite->Update();*/
	gameoverSprite->Update();
	titleUISprite->Update();


	//オートセーブ
	autoSave->Update();

	//ゴール
	goal->Update();
}

void GameScene::Draw()
{
	//-------背景スプライト描画処理-------//


	//object0->Draw(dxCommon_->GetCommandList());
	object1->Draw(dxCommon_->GetCommandList());
	//object2->Draw(dxCommon_->GetCommandList());

	//ブロック
	for (int i = 0; i < blockSize; i++) {
		blockObject[i]->Draw(dxCommon_->GetCommandList());
	}

	//プレイヤー
	player->Draw(dxCommon_->GetCommandList());

	//敵
	for (int i = 0; i < enemySize; i++) {
		enemy[i]->Draw(dxCommon_->GetCommandList());
	}

	//監視カメラ
	for (int i = 0; i < cameraEnemySize; i++) {
		cameraEnemy[i]->Draw(dxCommon_->GetCommandList());
	}

	//ボタン
	for (int i = 0; i < buttonSize; i++) {
		button[i]->Draw(dxCommon_->GetCommandList());
	}

	/*cubeObject->Draw(dxCommon_->GetCommandList());*/

	//-------前景スプライト描画処理-------//

	if (titleSprite->endFlip == false) {
		titleSprite->Draw(dxCommon_->GetCommandList());
	}

	if (titleSprite->isflipEase == false && titleTimer >= titleAssistTime) {
		titleUISprite->Draw(dxCommon_->GetCommandList());
	}

	if (player->GetDeath() == true) {
		gameoverSprite->Draw(dxCommon_->GetCommandList());
	}
	goal->Draw(dxCommon_->GetCommandList());

}
