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
	groundModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/white1x1.png");
	blockModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/gray1x1.png");
	playerModel = FbxLoader::GetInstance()->LoadModelFromFile("Walking", "Resources/color/blue1x1.png");
	enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/red1x1.png");
	enemyModel2 = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/green1x1.png");
	enemyEyeModel = FbxLoader::GetInstance()->LoadModelFromFile("enemyEye", "Resources/color/transparentYellow1x1.png");
	cameraEnemyModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/yellow1x1.png");
	buttonModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/yellow1x1.png");
	bombModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/red1x1.png");
	swampModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/brown1x1.png");

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
	player->Initialize(playerModel, playerColBox);

	//=============ギミック===============

	//各ステージの開始位置
	float stage1 = 0.0f;
	float stage2 = 150.0f;
	float stage3 = 290.0f;
	float stage4 = 400.0f;

	//--------ブロック----------
	for (int i = 0; i < blockSize; i++) {
		blockObject[i] = new FbxObject3D;
		blockObject[i]->Initialize();
		blockObject[i]->SetModel(blockModel);
	}

	//stage1
	blockObject[0]->SetPosition({ stage1 + 14,1,0 });
	blockObject[1]->SetPosition({ stage1 + 20,1,0 });

	blockObject[2]->SetPosition({ stage1 + 27,1,0 });
	blockObject[3]->SetPosition({ stage1 + 37,1,0 });

	//stage2
	blockObject[4]->SetPosition({ stage2 + 14,1,0 });
	blockObject[5]->SetPosition({ stage2 + 20,1,0 });

	blockObject[6]->SetPosition({ stage2 + 27,1,0 });
	blockObject[7]->SetPosition({ stage2 + 37,1,0 });

	//stage3
	blockObject[8]->SetPosition({ stage3 + 57,1,0 });
	blockObject[9]->SetPosition({ stage3 + 63,1,0 });
	blockObject[10]->SetPosition({ stage3 + 69,1,0 });
	blockObject[11]->SetPosition({ stage3 + 75,1,0 });

	blockObject[12]->SetPosition({ stage3 + 87,1,0 });
	blockObject[13]->SetPosition({ stage3 + 93,1,0 });
	blockObject[14]->SetPosition({ stage3 + 99,1,0 });
	blockObject[15]->SetPosition({ stage3 + 105,1,0 });

	//---------敵---------
	for (int i = 0; i < enemySize; i++) {
		enemy[i] = new Enemy;
		enemy[i]->Initialize(enemyModel, enemyModel2, enemyEyeModel, player);
	}

	//stage1
	enemy[0]->SetPosition({ stage1 + 17,1,1 });
	enemy[1]->SetPosition({ stage1 + 23,1,1 });
	enemy[2]->SetPosition({ stage1 + 30,1,1 });
	enemy[3]->SetPosition({ stage1 + 34,1,1 });

	//stage2
	enemy[4]->SetPosition({ stage2 + 17,1,1 });
	enemy[5]->SetPosition({ stage2 + 23,1,1 });
	enemy[6]->SetPosition({ stage2 + 30,1,1 });
	enemy[7]->SetPosition({ stage2 + 34,1,1 });
	enemy[8]->SetPosition({ stage2 + 66,1,1 });

	enemy[9]->SetPosition({ stage2 + 76,1,1 });
	enemy[10]->SetPosition({ stage2 + 90.5f,1,1 });

	enemy[11]->SetPosition({ stage2 + 100,1,1 });

	enemy[12]->SetPosition({ stage2 + 104.5f,1,1 });
	enemy[13]->SetPosition({ stage2 + 109,1,1 });

	//stage3
	enemy[14]->SetPosition({ stage3 + 60,1,1 });
	enemy[15]->SetPosition({ stage3 + 66,1,1 });
	enemy[16]->SetPosition({ stage3 + 72,1,1 });

	enemy[17]->SetPosition({ stage3 + 90,1,1 });
	enemy[18]->SetPosition({ stage3 + 96,1,1 });
	enemy[19]->SetPosition({ stage3 + 102,1,1 });

	//stage4
	enemy[20]->SetPosition({ stage4 + 5,1,1 });
	enemy[21]->SetPosition({ stage4 + 10,1,1 });
	enemy[22]->SetPosition({ stage4 + 15,1,1 });
	enemy[23]->SetPosition({ stage4 + 20,1,1 });
	enemy[24]->SetPosition({ stage4 + 25,1,1 });
	enemy[25]->SetPosition({ stage4 + 30,1,1 });

	enemy[26]->SetPosition({ stage4 + 74,1,1 });
	enemy[27]->SetPosition({ stage4 + 86,1,1 });

	enemy[28]->SetPosition({ stage4 + 162,1,1 });
	enemy[29]->SetPosition({ stage4 + 163,1,1 });
	enemy[30]->SetPosition({ stage4 + 164,1,1 });
	enemy[31]->SetPosition({ stage4 + 165,1,1 });
	enemy[32]->SetPosition({ stage4 + 166,1,1 });
	enemy[33]->SetPosition({ stage4 + 167,1,1 });
	enemy[34]->SetPosition({ stage4 + 168,1,1 });
	enemy[35]->SetPosition({ stage4 + 169,1,1 });
	enemy[36]->SetPosition({ stage4 + 170,1,1 });
	enemy[37]->SetPosition({ stage4 + 171,1,1 });
	enemy[38]->SetPosition({ stage4 + 172,1,1 });
	enemy[39]->SetPosition({ stage4 + 173,1,1 });
	enemy[40]->SetPosition({ stage4 + 174,1,1 });
	enemy[41]->SetPosition({ stage4 + 175,1,1 });
	enemy[42]->SetPosition({ stage4 + 176,1,1 });

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
		ButtonColBox[i] = new CubeObject3D();
		ButtonColBox[i]->Initialize();
		ButtonColBox[i]->SetModel(cubeModel);

		button[i] = new Button;
		button[i]->Initialize(buttonModel, player, ButtonColBox[i]);

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
	goal->SetClearPos(590);
}

void GameScene::Update()
{
	//カメラ更新
	camera_->Update(player->GetPosition());
	//コントローラー更新
	dxInput->InputProcess();

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

	//ブロック

	for (int i = 0; i < blockSize; i++) {
		blockObject[i]->SetScale({ 0.01f,0.01f,0.01f });
		blockObject[i]->SetRotation({ 0,0,0 });
		blockObject[i]->Update();
	}

	//プレイヤー
	player->Update();

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

	//スプライト

	//	//復活
	//if (input_->PushKey(DIK_E)) {
	//	player->SetisDeath(false);
	//	if (isClear) {
	//		isClear = false;
	//		player->SetPosition(XMFLOAT3(0, 1, -1));
	//	}
	//	else {
	//		XMFLOAT2 savePos = autoSave->GetSavePos();
	//		player->SetPosition(XMFLOAT3(savePos.x, savePos.y, -1));
	//	}
	//}


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
			alpha += 0.005f;
			player->SetAlpha(alpha);
			if (alpha >= 1.0f) {
				isback = true;
				XMFLOAT2 savePos = autoSave->GetSavePos();
				player->SetPosition(XMFLOAT3(savePos.x, savePos.y, -1));
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

}

void GameScene::Draw()
{
	//-------背景スプライト描画処理-------//

	ImGui::Begin("Light");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 200));
	ImGui::InputFloat3("lightTarget", lightTarget);
	ImGui::InputFloat3("lightPos", lightPos);
	ImGui::End();

	DrawFBX();

	////object0->Draw(dxCommon_->GetCommandList());
	//groundObject->Draw(dxCommon_->GetCommandList());
	////object2->Draw(dxCommon_->GetCommandList());

	////ブロック
	//for (int i = 0; i < blockSize; i++) {
	//	blockObject[i]->Draw(dxCommon_->GetCommandList());
	//}

	////プレイヤー
	//player->Draw(dxCommon_->GetCommandList());

	////敵
	//for (int i = 0; i < enemySize; i++) {
	//	enemy[i]->Draw(dxCommon_->GetCommandList());
	//}

	////監視カメラ
	//for (int i = 0; i < cameraEnemySize; i++) {
	//	cameraEnemy[i]->Draw(dxCommon_->GetCommandList());
	//}

	////ボタン
	//for (int i = 0; i < buttonSize; i++) {
	//	button[i]->Draw(dxCommon_->GetCommandList());
	//}

	////爆弾
	//for (int i = 0; i < bombSize; i++) {
	//	bomb[i]->Draw(dxCommon_->GetCommandList());
	//}

	////沼
	//for (int i = 0; i < swampSize; i++) {
	//	swamp[i]->Draw(dxCommon_->GetCommandList());
	//}

	/*cubeObject->Draw(dxCommon_->GetCommandList());*/

	//-------前景スプライト描画処理-------//

	if (titleSprite->endFlip == false) {
		titleSprite->Draw(dxCommon_->GetCommandList());
	}

	if (titleSprite->isflipEase == false && titleTimer >= titleAssistTime) {
		titleUISprite->Draw(dxCommon_->GetCommandList());
	}

	/*if (player->GetDeath() == true) {
		gameoverSprite->Draw(dxCommon_->GetCommandList());
	}*/
	blackSprite->Draw(dxCommon_->GetCommandList());

	goal->Draw(dxCommon_->GetCommandList());

	if (player->GetDeath() == false) {
		playUISprite->Draw(dxCommon_->GetCommandList());
	}

}

void GameScene::DrawFBXLightView()
{
	//object0->Draw(dxCommon_->GetCommandList());
	/*groundObject->DrawLightView(dxCommon_->GetCommandList());*/
	//object2->Draw(dxCommon_->GetCommandList());

	//ブロック
	for (int i = 0; i < blockSize; i++) {
		blockObject[i]->DrawLightView(dxCommon_->GetCommandList());
	}

	//プレイヤー
	player->DrawLightView(dxCommon_->GetCommandList());

	////敵
	//for (int i = 0; i < enemySize; i++) {
	//	enemy[i]->Draw(dxCommon_->GetCommandList());
	//}

	////監視カメラ
	//for (int i = 0; i < cameraEnemySize; i++) {
	//	cameraEnemy[i]->Draw(dxCommon_->GetCommandList());
	//}

	////ボタン
	//for (int i = 0; i < buttonSize; i++) {
	//	button[i]->Draw(dxCommon_->GetCommandList());
	//}

	////爆弾
	//for (int i = 0; i < bombSize; i++) {
	//	bomb[i]->Draw(dxCommon_->GetCommandList());
	//}

	////沼
	//for (int i = 0; i < swampSize; i++) {
	//	swamp[i]->Draw(dxCommon_->GetCommandList());
	//}

}

void GameScene::DrawFBX()
{
	//ブロック
	for (int i = 0; i < blockSize; i++) {
		blockObject[i]->Draw(dxCommon_->GetCommandList());
	}
	//プレイヤー
	player->Draw(dxCommon_->GetCommandList());
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

	//ブロック
	for (int i = 0; i < blockSize; i++) {
		blockObject[i]->SetSRV(SRV);
	}

	//プレイヤー
	player->SetSRV(SRV);
}
