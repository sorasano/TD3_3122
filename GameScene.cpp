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

	//カメラ初期化
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 10,-10 });

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


	buttonModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/yellow1x1.png");

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

	//プレイヤー
	Player::SetInput(input_);
	Player::SetDXInput(dxInput);
	player = new Player;
	player->Initialize(playerModel);

	//敵
	for (int i = 0; i < enemySize; i++) {
		enemy[i] = new Enemy;
		enemy[i]->Initialize(enemyModel);
	}

	//ボタン

	float startPos3 = 35;

	Button::SetInput(input_);
	Button::SetDXInput(dxInput);
	for (int i = 0; i < buttonSize; i++) {

		button[i] = new Button;
		button[i]->Initialize(buttonModel, player);

	}

	button[0]->SetPositionX(startPos3);
	button[0]->SetBlockPositionX(startPos3 + 3);


	button[1]->SetPositionX(startPos3 + 9);
	button[1]->SetBlockPositionX(startPos3 + 21);

	button[2]->SetPositionX(startPos3 + 27);
	button[2]->SetBlockPositionX(startPos3 + 39);

	button[3]->SetPositionX(startPos3 + 27);
	button[3]->SetBlockPositionX(startPos3 + 42);


	button[4]->SetPositionX(startPos3 + 48);
	button[4]->SetBlockPositionX(startPos3 + 65);

	button[5]->SetPositionX(startPos3 + 52);
	button[5]->SetBlockPositionX(startPos3 + 68);

	button[6]->SetPositionX(startPos3 + 56);
	button[6]->SetBlockPositionX(startPos3 + 71);

	//------テクスチャ------

	//---クリア---
	clearTexture = Texture::LoadTexture(L"Resources/clear.png");

	clearSprite = new Sprite();
	clearSprite->Initialize(clearTexture);
	//アンカーポイントをスプライトの中心に
	clearSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	clearSprite->SetPos(XMFLOAT2(WinApp::winW / 2, WinApp::winH / 2));
	clearSprite->Update();

	//---ゲームオーバー---
	gameoverTexture = Texture::LoadTexture(L"Resources/gameover.png");

	gameoverSprite = new Sprite();
	gameoverSprite->Initialize(gameoverTexture);
	//アンカーポイントをスプライトの中心に
	gameoverSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	gameoverSprite->SetPos(XMFLOAT2(WinApp::winW / 2, WinApp::winH / 2));
	gameoverSprite->Update();

	//---タイトル---
	titleTexture = Texture::LoadTexture(L"Resources/title.png");

	titleSprite = new Sprite();
	titleSprite->Initialize(titleTexture);
	titleSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	titleSprite->SetPos(XMFLOAT2(WinApp::winW / 2, WinApp::winH / 2 - 200));
	titleSprite->StartSway({ WinApp::winW / 2, WinApp::winH / 2 - 200 });
	titleSprite->Update();

	titleUITexture = Texture::LoadTexture(L"Resources/titleUI.png");

	titleUISprite = new Sprite();
	titleUISprite->Initialize(titleUITexture);
	titleUISprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	titleUISprite->SetPos(XMFLOAT2(WinApp::winW / 2, WinApp::winH / 2 + 200));
	titleUISprite->Update();
}

void GameScene::Update()
{
	//カメラ更新
	camera_->DebugUpdate();
	camera_->Update();
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
	blockObject[0]->SetPosition({ 4,1,0 });
	blockObject[1]->SetPosition({ 5,1,0 });

	blockObject[2]->SetPosition({ 9,1,0 });
	blockObject[3]->SetPosition({ 10,1,0 });

	blockObject[4]->SetPosition({ 14,1,0 });
	blockObject[5]->SetPosition({ 15,1,0 });
	blockObject[6]->SetPosition({ 16,1,0 });

	blockObject[7]->SetPosition({ 21,1,0 });
	blockObject[8]->SetPosition({ 22,1,0 });

	blockObject[9]->SetPosition({ 28,1,0 });


	for (int i = 0; i < blockSize; i++) {
		blockObject[i]->SetScale({ 0.01f,0.01f,0.01f });
		blockObject[i]->SetRotation({ 0,0,0 });
		blockObject[i]->Update();
	}

	//プレイヤー
	player->Update();
	playerpos = player->GetPosition();

	//敵
	enemy[0]->SetPosition({ 7,1,1 });
	enemy[1]->SetPosition({ 12,1,1 });
	enemy[2]->SetPosition({ 18.5,1,1 });
	enemy[3]->SetPosition({ 25,1,1 });

	for (int i = 0; i < enemySize; i++) {
		enemy[i]->Update();
		//判定用
		enemypos[i] = enemy[i]->GetPosition();
		enemyvec[i] = enemy[i]->Getvec();
		targetvec[i].x = (enemypos[i].x-playerpos.x ) ;
		targetvec[i].y = ( enemypos[i].y-playerpos.y );
		targetvec[i].z = (enemypos[i].z-playerpos.z );
		targetvec[i].normalize();
		dot[i] = enemyvec[i].dot(targetvec[i]);
		deg[i] = acos(dot[i]) * (PI / 180);
	}
  
	time++;
	if (time >= maxTime) {
		if (isback == true) {
			isback = false;
		}
		else {
			isback = true;

		}
		time = 0;
	}

	for (int i = 0; i < enemySize; i++) {
		if (isback == false) {
			enemy[i]->SetModel(enemyModel);
			if (deg[i] <= 13) {
				isHit = true;
			}
			else {
				isHit = false;
			}
		}
		else {
			enemy[i]->SetModel(enemyModel2);
			isHit = false;
		}
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
}

void GameScene::Draw()
{

	ImGui::Begin("Light");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 500));
	ImGui::ColorEdit3("ambientColor", ambientColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir0", lightDir0);
	ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);
	/*ImGui::InputFloat3("circleShadowDir", circleShadowDir);
	ImGui::InputFloat3("circleShadowAtten", circleShadowAtten);
	ImGui::ColorEdit3("pointLightColor", pointLightColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("pointLightPos", pointLightPos0);
	ImGui::InputFloat3("pointLightAtten", pointLightAtten0);*/
	ImGui::InputFloat3("lightPos", shadowLightPos);
	ImGui::InputFloat4("deg", deg);
	ImGui::End();

	//-------背景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();



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

	//ボタン
	for (int i = 0; i < buttonSize; i++) {
		button[i]->Draw(dxCommon_->GetCommandList());
	}

	//-------前景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	//clearSprite->Draw();
	if (titleSprite->endFlip == false) {
		titleSprite->Draw();
	}

	if (titleSprite->isflipEase == false && titleTimer >= titleAssistTime) {
		titleUISprite->Draw();
	}

	if (player->GetDeath()) {
		gameoverSprite->Draw();
	}

}
