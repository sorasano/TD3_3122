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
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 10,-10 });

	//当たり判定キューブオブジェクト
	CubeObject3D::SetCamera(camera_.get());
	CubeObject3D::SetDevice(dxCommon_->GetDevice());
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

	enemyModel = FbxLoader::GetInstance()->LoadModelFromFile("enemy", "Resources/color/red1x1.png");
	enemyModel2 = FbxLoader::GetInstance()->LoadModelFromFile("enemy", "Resources/color/green1x1.png");
	enemyEyeModel = FbxLoader::GetInstance()->LoadModelFromFile("enemyEye", "Resources/color/yellow1x1.png");
	cameraEnemyModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/yellow1x1.png");

	buttonUpModel = FbxLoader::GetInstance()->LoadModelFromFile("buttonUp", "Resources/color/yellow1x1.png");
	buttonDownModel = FbxLoader::GetInstance()->LoadModelFromFile("buttonDown", "Resources/color/yellow1x1.png");

	pushButtonUpModel = FbxLoader::GetInstance()->LoadModelFromFile("buttonUp", "Resources/color/blue1x1.png");
	pushButtonDownModel = FbxLoader::GetInstance()->LoadModelFromFile("buttonDown", "Resources/color/blue1x1.png");

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
	player = new Player;
	player->Initialize(playerColBox);

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

	////=============ギミック===============

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

	cameraEnemyCsv = new CSVLoader;
	cameraEnemyCsv->LoadCSV("Resources/csv/cameraEnemy.csv");

	for (int i = 0; i < cameraEnemySize; i++)
	{
		std::unique_ptr<CameraEnemy>newObject = std::make_unique<CameraEnemy>();
		newObject->Initialize(cameraEnemyModel, enemyEyeModel, player);
		newObject->SetModel(cameraEnemyModel);

		newObject->SetPosition(cameraEnemyCsv->GetPosition(i));
		newObject->SetScale(cameraEnemyCsv->GetScale(i));
		newObject->Setrotate(cameraEnemyCsv->GetRotation(i));

		cameraEnemys.push_back(std::move(newObject));
	}

	//--------ボタン----------

	buttonCsv = new CSVLoader;
	buttonCsv->LoadCSV("Resources/csv/button.csv");

	buttonBlockCsv = new CSVLoader;
	buttonBlockCsv->LoadCSV("Resources/csv/buttonBlock.csv");

	for (int i = 0; i < buttonSize; i++)
	{
		std::unique_ptr<CubeObject3D>newColObject = std::make_unique<CubeObject3D>();
		newColObject->Initialize();
		newColObject->SetModel(cubeModel);
		buttonColBoxs.push_back(std::move(newColObject));

		std::unique_ptr<Button>newObject = std::make_unique<Button>();
		newObject->Initialize(buttonUpModel, buttonDownModel, player, buttonColBoxs.back().get());

		newObject->SetPosition(buttonCsv->GetPosition(i));
		newObject->SetScale(buttonCsv->GetScale(i));
		newObject->Setrotate(buttonCsv->GetRotation(i));

		newObject->SetBlockPosition(buttonBlockCsv->GetPosition(i));
		newObject->SetBlockScale(buttonBlockCsv->GetScale(i));
		newObject->SetBlockrotate(buttonBlockCsv->GetRotation(i));

		buttons.push_back(std::move(newObject));
	}

	//--------爆弾--------	

	bombCsv = new CSVLoader;
	bombCsv->LoadCSV("Resources/csv/bomb.csv");

	for (int i = 0; i < bombSize; i++)
	{
		std::unique_ptr<Bomb>newObject = std::make_unique<Bomb>();
		newObject->Initialize(bombModel, player);

		newObject->SetPosition(bombCsv->GetPosition(i));
		newObject->SetScale(bombCsv->GetScale(i));
		newObject->Setrotate(bombCsv->GetRotation(i));

		bombs.push_back(std::move(newObject));
	}

	//----------沼----------

	swampCsv = new CSVLoader;
	swampCsv->LoadCSV("Resources/csv/swamp.csv");

	for (int i = 0; i < swampSize; i++)
	{
		std::unique_ptr<Swamp>newObject = std::make_unique<Swamp>();
		newObject->Initialize(swampModel, player);

		newObject->SetPosition(swampCsv->GetPosition(i));
		newObject->SetScale(swampCsv->GetScale(i));
		newObject->Setrotate(swampCsv->GetRotation(i));

		swamps.push_back(std::move(newObject));
	}

	//----------梯子--------


	ladderCsv = new CSVLoader;
	ladderCsv->LoadCSV("Resources/csv/ladder.csv");

	for (int i = 0; i < ladderSize; i++)
	{
		std::unique_ptr<Ladder>newObject = std::make_unique<Ladder>();
		newObject->Initialize(ladderModel, player);

		newObject->SetPosition(ladderCsv->GetPosition(i));
		newObject->SetScale(ladderCsv->GetScale(i));
		newObject->Setrotate(ladderCsv->GetRotation(i));

		ladders.push_back(std::move(newObject));
	}

	//--------動く敵-------

	moveEnemyCsv = new CSVLoader;
	moveEnemyCsv->LoadCSV("Resources/csv/moveEnemy.csv");

	for (int i = 0; i < moveEnemySize; i++)
	{
		std::unique_ptr<MoveEnemy>newObject = std::make_unique<MoveEnemy>();
		newObject->Initialize(player);

		newObject->SetPosition(moveEnemyCsv->GetPosition(i));
		newObject->SetScale(moveEnemyCsv->GetScale(i));
		newObject->Setrotate(moveEnemyCsv->GetRotation(i));

		moveEnemys.push_back(std::move(newObject));
	}

	//----------動かせるブロック----------
	PushBlock::SetInput(input_);

	pushBlockCsv = new CSVLoader;
	pushBlockCsv->LoadCSV("Resources/csv/pushBlock.csv");

	for (int i = 0; i < pushBlockSize; i++)
	{
		std::unique_ptr<CubeObject3D>newColObject = std::make_unique<CubeObject3D>();
		newColObject->Initialize();
		newColObject->SetModel(cubeModel);
		pushBlockColBoxs.push_back(std::move(newColObject));

		std::unique_ptr<PushBlock>newObject = std::make_unique<PushBlock>();
		newObject->Initialize(pushBlockModel, player, pushBlockColBoxs.back().get());

		newObject->SetPosition(pushBlockCsv->GetPosition(i));
		newObject->SetScale(pushBlockCsv->GetScale(i));
		newObject->Setrotate(pushBlockCsv->GetRotation(i));

		pushBlocks.push_back(std::move(newObject));
	}

	//----------ブロック----------

	blockCsv = new CSVLoader;
	blockCsv->LoadCSV("Resources/csv/block.csv");

	for (int i = 0; i < blockSize; i++)
	{
		std::unique_ptr<CubeObject3D>newColObject = std::make_unique<CubeObject3D>();
		newColObject->Initialize();
		newColObject->SetModel(cubeModel);
		blockColBoxs.push_back(std::move(newColObject));

		std::unique_ptr<Block>newObject = std::make_unique<Block>();
		newObject->Initialize(blockModel, player, blockColBoxs.back().get());

		newObject->SetPosition(blockCsv->GetPosition(i));
		newObject->SetScale(blockCsv->GetScale(i));
		newObject->Setrotate(blockCsv->GetRotation(i));

		blocks.push_back(std::move(newObject));
	}

	//----------押している間のスイッチ----------

	pushButtonCsv = new CSVLoader;
	pushButtonCsv->LoadCSV("Resources/csv/pushButton.csv");

	pushButtonBlockCsv = new CSVLoader;
	pushButtonBlockCsv->LoadCSV("Resources/csv/pushButtonBlock.csv");

	for (int i = 0; i < pushButtonSize; i++)
	{
		std::unique_ptr<CubeObject3D>newColObject = std::make_unique<CubeObject3D>();
		newColObject->Initialize();
		newColObject->SetModel(cubeModel);
		pushButtonColBoxs.push_back(std::move(newColObject));

		std::unique_ptr<CubeObject3D>newColObject2 = std::make_unique<CubeObject3D>();
		newColObject2->Initialize();
		newColObject2->SetModel(cubeModel);
		pushButtonBlockColBoxs.push_back(std::move(newColObject2));

		std::unique_ptr<PushButton>newObject = std::make_unique<PushButton>();
		newObject->Initialize(pushButtonUpModel, pushButtonDownModel, player, pushButtonColBoxs.back().get(), pushButtonBlockColBoxs.back().get());

		newObject->SetPosition(pushButtonCsv->GetPosition(i));
		newObject->SetScale(pushButtonCsv->GetScale(i));
		newObject->Setrotate(pushButtonCsv->GetRotation(i));

		newObject->SetBlockPosition(pushButtonBlockCsv->GetPosition(i));
		newObject->SetBlockScale(pushButtonBlockCsv->GetScale(i));
		newObject->SetBlockrotate(pushButtonBlockCsv->GetRotation(i));

		pushButtons.push_back(std::move(newObject));
	}


	//セーブ
	autoSave = new Autosave;
	autoSave->Initialize(player);

	//------テクスチャ------

	spriteManager->LoadFile(0, L"Resources/clear.png");
	//spriteManager->LoadFile(1, L"Resources/gameover2.png");
	spriteManager->LoadFile(2, L"Resources/title.png");
	//spriteManager->LoadFile(3, L"Resources/titleUI.png");
	spriteManager->LoadFile(4, L"Resources/color/white1x1.png");
	spriteManager->LoadFile(5, L"Resources/color/black1x1.png");
	//spriteManager->LoadFile(6, L"Resources/playUI.png");

	//メニュー用
	spriteManager->LoadFile(7, L"Resources/Menu/menuBase.png");
	spriteManager->LoadFile(8, L"Resources/Menu/menuRestart.png");
	spriteManager->LoadFile(9, L"Resources/Menu/menuTitle.png");
	spriteManager->LoadFile(10, L"Resources/Menu/menuClose.png");

	//操作説明UI
	spriteManager->LoadFile(11, L"Resources/operationUI/play.png");
	spriteManager->LoadFile(12, L"Resources/operationUI/clear.png");
	spriteManager->LoadFile(13, L"Resources/operationUI/menu.png");

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

	//操作説明UI

	playOpSprite = new Sprite();
	playOpSprite->SetTextureNum(11);
	playOpSprite->Initialize();
	playOpSprite->SetScale(XMFLOAT2(300, 150));
	playOpSprite->SetPosition(XMFLOAT2(0, window_height - 150));
	playOpSprite->Update();

	clearOpSprite = new Sprite();
	clearOpSprite->SetTextureNum(12);
	clearOpSprite->Initialize();
	clearOpSprite->SetScale(XMFLOAT2(300, 150));
	clearOpSprite->SetPosition(XMFLOAT2(0, window_height - 150));
	clearOpSprite->Update();

	menuOpSprite = new Sprite();
	menuOpSprite->SetTextureNum(13);
	menuOpSprite->Initialize();
	menuOpSprite->SetScale(XMFLOAT2(300, 150));
	menuOpSprite->SetPosition(XMFLOAT2(0, window_height - 150));
	menuOpSprite->Update();

	//ゴール
	goal = new Goal();
	goal->Initialize(whiteSprite, clearSprite, player);
	goal->SetClearPos(1000);


	/*cube = new CubeObject3D();
	cube->Initialize();
	cube->SetModel(cubeModel);*/


	//オーディオ初期化
	AudioManager::StaticInitialize();
	//音
	playBGM = new AudioManager();
	playBGM->SoundLoadWave("Resources/Audio/playBGM.wav");
	pickSE = new AudioManager();
	pickSE->SoundLoadWave("Resources/Audio/pickSE.wav");

	//メニュー
	menu = new Menu();
	menu->Initialize(input_);

}

void GameScene::Update()
{

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

		//BGM流す
		playBGM->SoundPlayWave(true, playBGMVolume);

		//タイトルスプライト動かす
		if (input_->IsKeyPress(DIK_A) || input_->IsKeyPress(DIK_D) || input_->IsDownLStickLeft() || input_->IsDownLStickRight()) {
			titleSprite->StartFlipOut();
			scene = PLAY;
		}

		//シーン切り替え
		if (!player->GetDeath()) {
			if (input_->IsKeyTrigger(DIK_M) || input_->IsPadTrigger(XINPUT_GAMEPAD_START)) {
				scene = MENU;
				menu->Reset();


				pickSE->StopWave();
				pickSE->SoundPlayWave(false, pickSEVolume);
				//BGM停止
				playBGM->StopWave();
			}
		}
		else if (goal->GetIsClear()) {
			scene = CLEAR;
		}

		//ブロック

		for (std::unique_ptr<Block>& block : blocks)
		{
			block->Update();
		}

		//敵

		for (std::unique_ptr<Enemy>& enemy : enemys)
		{
			enemy->Update();
		}

		//監視カメラ
		for (std::unique_ptr<CameraEnemy>& cameraEnemy : cameraEnemys)
		{
			cameraEnemy->Update();
		}

		//ボタン
		for (std::unique_ptr<Button>& button : buttons)
		{
			button->Update();
		}

		//爆弾
		for (std::unique_ptr<Bomb>& bomb : bombs)
		{
			bomb->Update();
		}

		//沼
		for (std::unique_ptr<Swamp>& swamp : swamps)
		{
			swamp->Update();
		}

		//梯子
		for (std::unique_ptr<Ladder>& ladder : ladders)
		{
			ladder->Update();
		}

		//動く敵
		for (std::unique_ptr<MoveEnemy>& moveEnemy : moveEnemys)
		{
			moveEnemy->Update();
		}


		//タイトル
		titleSprite->Update();



		//動かせるブロック
		i = 0;
		for (auto itrBlocks = pushBlocks.begin(); itrBlocks != pushBlocks.end(); ++itrBlocks) {
			itrBlocks->get()->Collision();
			if (itrBlocks->get()->GetIsPush()) {
				//動かせるブロック同士の判定
				j = 0;
				for (auto itr = pushBlockColBoxs.begin(); itr != pushBlockColBoxs.end(); ++itr) {
					if (i != j) {
						itrBlocks->get()->pushback(itr->get());
						//押さないかどうか
						if (itrBlocks->get()->GetNoPush()) {
							noPush = true;
						}
					}
					j++;
				}
				//ギミックの判定追加場所
				//動かないブロックとの判定
				for (auto itr = blockColBoxs.begin(); itr != blockColBoxs.end(); ++itr) {
					itrBlocks->get()->pushback(itr->get());
					//押さないかどうか
					if (itrBlocks->get()->GetNoPush()) {
						noPush = true;
					}
				}
				//押している間のスイッチ
				for (auto itr = pushButtonBlockColBoxs.begin(); itr != pushButtonBlockColBoxs.end(); ++itr) {
					itrBlocks->get()->pushback(itr->get());
					//押さないかどうか
					if (itrBlocks->get()->GetNoPush()) {
						noPush = true;
					}
				}

				//ギミックの判定追加場所ここまで
				if (noPush) {
					itrBlocks->get()->NoPush();
				}
				else {
					itrBlocks->get()->Push();
				}
			}

			itrBlocks->get()->Update();
			i++;
		}
		noPush = false;

		//押している間のスイッチ
		for (std::unique_ptr<PushButton>& pushButton : pushButtons) {
			for (auto itr = pushBlockColBoxs.begin(); itr != pushBlockColBoxs.end(); ++itr) {
				pushButton->ButtonCol(itr->get());
			}
			pushButton->Update();
		}

		for (std::unique_ptr<FbxObject3D>& object : objectTree)
		{
			object->Update();
		}

		//Tree1
		//スペースキーでファイル読み込み更新
		if (input_->IsKeyTrigger(DIK_RETURN))
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

			//---------敵---------

			enemyCsv->LoadCSV("Resources/csv/enemy.csv");
			i = 0;

			for (std::unique_ptr<Enemy>& enemy : enemys)
			{
				enemy->SetPosition(enemyCsv->GetPosition(i));
				enemy->SetScale(enemyCsv->GetScale(i));
				enemy->Setrotate(enemyCsv->GetRotation(i));
				i++;

			}

			//---------監視カメラ---------

			cameraEnemyCsv->LoadCSV("Resources/csv/cameraEnemy.csv");
			i = 0;

			for (std::unique_ptr<CameraEnemy>& cameraEnemy : cameraEnemys)
			{
				cameraEnemy->SetPosition(cameraEnemyCsv->GetPosition(i));
				cameraEnemy->SetScale(cameraEnemyCsv->GetScale(i));
				cameraEnemy->Setrotate(cameraEnemyCsv->GetRotation(i));
				i++;

			}

			//--------ボタン----------

			buttonCsv->LoadCSV("Resources/csv/button.csv");
			buttonBlockCsv->LoadCSV("Resources/csv/buttonBlock.csv");
			i = 0;

			for (std::unique_ptr<Button>& button : buttons)
			{

				button->SetPosition(buttonCsv->GetPosition(i));
				button->SetScale(buttonCsv->GetScale(i));
				button->Setrotate(buttonCsv->GetRotation(i));

				button->SetBlockPosition(buttonBlockCsv->GetPosition(i));
				button->SetBlockScale(buttonBlockCsv->GetScale(i));
				button->SetBlockrotate(buttonBlockCsv->GetRotation(i));
				i++;

			}

			//--------爆弾--------	

			bombCsv->LoadCSV("Resources/csv/bomb.csv");
			i = 0;

			for (std::unique_ptr<Bomb>& bomb : bombs)
			{

				bomb->SetPosition(bombCsv->GetPosition(i));
				bomb->SetScale(bombCsv->GetScale(i));
				bomb->Setrotate(bombCsv->GetRotation(i));
				i++;

			}

			//----------沼----------

			swampCsv->LoadCSV("Resources/csv/swamp.csv");
			i = 0;

			for (std::unique_ptr<Swamp>& swamp : swamps)
			{

				swamp->SetPosition(swampCsv->GetPosition(i));
				swamp->SetScale(swampCsv->GetScale(i));
				swamp->Setrotate(swampCsv->GetRotation(i));

				i++;

			}

			//----------梯子--------

			ladderCsv->LoadCSV("Resources/csv/ladder.csv");
			i = 0;

			for (std::unique_ptr<Ladder>& ladder : ladders)
			{
				ladder->SetPosition(ladderCsv->GetPosition(i));
				ladder->SetScale(ladderCsv->GetScale(i));
				ladder->Setrotate(ladderCsv->GetRotation(i));

				i++;
			}

			//--------動く敵-------

			moveEnemyCsv->LoadCSV("Resources/csv/moveEnemy.csv");
			i = 0;

			for (std::unique_ptr<MoveEnemy>& moveEnemy : moveEnemys)
			{
				moveEnemy->SetPosition(moveEnemyCsv->GetPosition(i));
				moveEnemy->SetScale(moveEnemyCsv->GetScale(i));
				moveEnemy->Setrotate(moveEnemyCsv->GetRotation(i));
				i++;

			}

			//----------動かせるブロック----------

			pushBlockCsv->LoadCSV("Resources/csv/pushBlock.csv");
			i = 0;

			for (std::unique_ptr<PushBlock>& pushBlock : pushBlocks)
			{
				pushBlock->SetPosition(pushBlockCsv->GetPosition(i));
				pushBlock->SetScale(pushBlockCsv->GetScale(i));
				pushBlock->Setrotate(pushBlockCsv->GetRotation(i));
				i++;


			}

			//----------ブロック----------

			blockCsv->LoadCSV("Resources/csv/block.csv");
			i = 0;
			for (std::unique_ptr<Block>& block : blocks)
			{
				block->SetPosition(blockCsv->GetPosition(i));
				block->SetScale(blockCsv->GetScale(i));
				block->Setrotate(blockCsv->GetRotation(i));
				i++;

			}

			//----------押している間のスイッチ----------

			pushButtonCsv->LoadCSV("Resources/csv/pushButton.csv");
			pushButtonBlockCsv->LoadCSV("Resources/csv/pushButtonBlock.csv");
			i = 0;
			for (std::unique_ptr<PushButton>& pushButton : pushButtons)
			{

				pushButton->SetPosition(pushButtonCsv->GetPosition(i));
				pushButton->SetScale(pushButtonCsv->GetScale(i));
				pushButton->Setrotate(pushButtonCsv->GetRotation(i));

				pushButton->SetBlockPosition(pushButtonBlockCsv->GetPosition(i));
				pushButton->SetBlockScale(pushButtonBlockCsv->GetScale(i));
				pushButton->SetBlockrotate(pushButtonBlockCsv->GetRotation(i));
				i++;


			}

			autoSave->UpdateCSV();

		}

		//プレイヤー
		player->Update();
		gameoverSprite->Update();
		blackSprite->Update();
		//操作UI
		playOpSprite->Update();
		menuOpSprite->Update();
		clearOpSprite->Update();

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
			if (input_->IsKeyPress(DIK_A) || input_->IsKeyPress(DIK_D) || input_->IsDownLStickLeft() || input_->IsDownLStickRight()) {
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

		goal->Update();

		//シーン切り替え
		if (goal->GetIsEnd()) {
			if (input_->IsKeyPress(DIK_SPACE) || input_->IsPadTrigger(XINPUT_GAMEPAD_A)) {
				scene = PLAY;
				Reset(true);

				pickSE->StopWave();
				pickSE->SoundPlayWave(false, pickSEVolume);
			}
		}
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

	//ブロック
	for (std::unique_ptr<Block>& block : blocks) {
		if (camera_->GetEye().x - block->GetPosition().x >= -20 && camera_->GetEye().x - block->GetPosition().x <= 20)
			block->DrawLightView(dxCommon_->GetCommandList());
	}

	//プレイヤー
	player->DrawLightView(dxCommon_->GetCommandList());

	//敵
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		if (camera_->GetEye().x - enemy->GetPosition().x >= -20 && camera_->GetEye().x - enemy->GetPosition().x <= 20)
			enemy->DrawLightView(dxCommon_->GetCommandList());
	}

	//監視カメラ
	for (std::unique_ptr<CameraEnemy>& cameraEnemy : cameraEnemys) {
		if (camera_->GetEye().x - cameraEnemy->GetPosition().x >= -20 && camera_->GetEye().x - cameraEnemy->GetPosition().x <= 20)
			cameraEnemy->DrawLightView(dxCommon_->GetCommandList());
	}

	//ボタン
	for (std::unique_ptr<Button>& button : buttons) {
		if (camera_->GetEye().x - button->GetPosition().x >= -40 && camera_->GetEye().x - button->GetPosition().x <= 40)
			button->DrawLightView(dxCommon_->GetCommandList());
	}

	//爆弾
	for (std::unique_ptr<Bomb>& bomb : bombs) {
		if (camera_->GetEye().x - bomb->GetPosition().x >= -20 && camera_->GetEye().x - bomb->GetPosition().x <= 20)
			bomb->DrawLightView(dxCommon_->GetCommandList());
	}

	//沼
	for (std::unique_ptr<Swamp>& swamp : swamps) {
		if (camera_->GetEye().x - swamp->GetPosition().x >= -20 && camera_->GetEye().x - swamp->GetPosition().x <= 20)
			swamp->DrawLightView(dxCommon_->GetCommandList());
	}

	//梯子
	for (std::unique_ptr<Ladder>& ladder : ladders) {
		if (camera_->GetEye().x - ladder->GetPosition().x >= -20 && camera_->GetEye().x - ladder->GetPosition().x <= 20)
			ladder->DrawLightView(dxCommon_->GetCommandList());
	}
	//動かせるブロック
	for (std::unique_ptr<PushBlock>& pushBlock : pushBlocks) {
		if (camera_->GetEye().x - pushBlock->GetPosition().x >= -20 && camera_->GetEye().x - pushBlock->GetPosition().x <= 20)
			pushBlock->DrawLightView(dxCommon_->GetCommandList());
	}
	//押している間のスイッチ
	for (std::unique_ptr<PushButton>& pushButton : pushButtons) {
		if (camera_->GetEye().x - pushButton->GetPosition().x >= -40 && camera_->GetEye().x - pushButton->GetPosition().x <= 40)
			pushButton->DrawLightView(dxCommon_->GetCommandList());
	}

	//動く敵
	for (std::unique_ptr<MoveEnemy>& moveEnemy : moveEnemys) {
		if (camera_->GetEye().x - moveEnemy->GetPosition().x >= -20 && camera_->GetEye().x - moveEnemy->GetPosition().x <= 20)
			moveEnemy->DrawLightView(dxCommon_->GetCommandList());
	}
}

void GameScene::DrawFBX()
{
	groundObject->Draw(dxCommon_->GetCommandList());

	//for (std::unique_ptr<FbxObject3D>& object : objectTree)
	//{
	//	object->Draw(dxCommon_->GetCommandList());
	//}

	//ブロック
	for (std::unique_ptr<Block>& block : blocks) {
		if (camera_->GetEye().x - block->GetPosition().x >= -20 && camera_->GetEye().x - block->GetPosition().x <= 20)
			block->Draw(dxCommon_->GetCommandList());
	}
	//プレイヤー
	player->Draw(dxCommon_->GetCommandList());
	//敵
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		if (camera_->GetEye().x - enemy->GetPosition().x >= -20 && camera_->GetEye().x - enemy->GetPosition().x <= 20)
			enemy->Draw(dxCommon_->GetCommandList());
	}

	//監視カメラ
	for (std::unique_ptr<CameraEnemy>& cameraEnemy : cameraEnemys) {
		if (camera_->GetEye().x - cameraEnemy->GetPosition().x >= -20 && camera_->GetEye().x - cameraEnemy->GetPosition().x <= 20)
			cameraEnemy->Draw(dxCommon_->GetCommandList());
	}

	//ボタン
	for (std::unique_ptr<Button>& button : buttons) {
		if (camera_->GetEye().x - button->GetPosition().x >= -40 && camera_->GetEye().x - button->GetPosition().x <= 40)
			button->Draw(dxCommon_->GetCommandList());
	}

	//爆弾
	for (std::unique_ptr<Bomb>& bomb : bombs) {
		if (camera_->GetEye().x - bomb->GetPosition().x >= -20 && camera_->GetEye().x - bomb->GetPosition().x <= 20)
			bomb->Draw(dxCommon_->GetCommandList());
	}

	//沼
	for (std::unique_ptr<Swamp>& swamp : swamps) {
		if (camera_->GetEye().x - swamp->GetPosition().x >= -20 && camera_->GetEye().x - swamp->GetPosition().x <= 20)
			swamp->Draw(dxCommon_->GetCommandList());
	}

	//梯子
	for (std::unique_ptr<Ladder>& ladder : ladders) {
		if (camera_->GetEye().x - ladder->GetPosition().x >= -20 && camera_->GetEye().x - ladder->GetPosition().x <= 20)
			ladder->Draw(dxCommon_->GetCommandList());
	}

	//動く敵
	for (std::unique_ptr<MoveEnemy>& moveEnemy : moveEnemys) {
		if (camera_->GetEye().x - moveEnemy->GetPosition().x >= -20 && camera_->GetEye().x - moveEnemy->GetPosition().x <= 20)
			moveEnemy->Draw(dxCommon_->GetCommandList());
	}
	//動かせるブロック
	for (std::unique_ptr<PushBlock>& pushBlock : pushBlocks) {
		if (camera_->GetEye().x - pushBlock->GetPosition().x >= -20 && camera_->GetEye().x - pushBlock->GetPosition().x <= 20)
			pushBlock->Draw(dxCommon_->GetCommandList());
	}
	//押している間のスイッチ
	for (std::unique_ptr<PushButton>& pushButton : pushButtons) {
		if (camera_->GetEye().x - pushButton->GetPosition().x >= -40 && camera_->GetEye().x - pushButton->GetPosition().x <= 40)
			pushButton->Draw(dxCommon_->GetCommandList());
	}
}

void GameScene::DrawSprite()
{
	//-------前景スプライト描画処理-------//

	if (scene == PLAY) {

		if (titleSprite->endFlip == false) {
			titleSprite->Draw(dxCommon_->GetCommandList());
		}

		blackSprite->Draw(dxCommon_->GetCommandList());

		if (player->GetDeath() == false) {
			playOpSprite->Draw(dxCommon_->GetCommandList());
		}
	}

	else if (scene == MENU) {
		menu->Draw(dxCommon_->GetCommandList());
		menuOpSprite->Draw(dxCommon_->GetCommandList());
	}

	else if (scene == CLEAR) {
		goal->Draw(dxCommon_->GetCommandList());
		if (goal->GetIsEnd()) {
			clearOpSprite->Draw(dxCommon_->GetCommandList());
		}
	}

}

void GameScene::Reset(bool isFirst)
{
	//プレイヤー
	XMFLOAT2 savePos = autoSave->GetSavePos();
	player->Reset(savePos);

	//動く敵
	for (std::unique_ptr<MoveEnemy>& moveEnemy : moveEnemys) {
		moveEnemy->Reset();
	}
	//押せるブロック
	for (std::unique_ptr<PushBlock>& pushBlock : pushBlocks) {
		pushBlock->Reset();
	}//押している間のスイッチ
	for (std::unique_ptr<PushButton>& pushButton : pushButtons) {
		pushButton->Reset();
	}

	//最初からなら
	if (isFirst) {
		isClear = false;
		titleSprite->Reset();
		titleTimer = 0;

		//初期地点
		XMFLOAT2 startPos = autoSave->GetStartPos();
		player->Reset(startPos);

		goal->Reset();
	}

}


DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}

void GameScene::SetSRV(ID3D12DescriptorHeap* SRV)
{

	groundObject->SetSRV(SRV);

	for (std::unique_ptr<FbxObject3D>& object : objectTree)
	{
		object->SetSRV(SRV);
	}

	//ブロック
	for (std::unique_ptr<Block>& block : blocks) {
		block->SetSRV(SRV);
	}

	//プレイヤー
	player->SetSRV(SRV);

	//敵
	for (std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->SetSRV(SRV);
	}

	//監視カメラ
	for (std::unique_ptr<CameraEnemy>& cameraEnemy : cameraEnemys) {
		cameraEnemy->SetSRV(SRV);
	}

	//ボタン
	for (std::unique_ptr<Button>& button : buttons) {
		button->SetSRV(SRV);
	}

	//爆弾
	for (std::unique_ptr<Bomb>& bomb : bombs) {
		bomb->SetSRV(SRV);
	}

	//沼
	for (std::unique_ptr<Swamp>& swamp : swamps) {
		swamp->SetSRV(SRV);
	}

	//梯子
	for (std::unique_ptr<Ladder>& ladder : ladders) {
		ladder->SetSRV(SRV);
	}

	//動く敵
	for (std::unique_ptr<MoveEnemy>& moveEnemy : moveEnemys) {
		moveEnemy->SetSRV(SRV);
	}
	//動かせるブロック
	for (std::unique_ptr<PushBlock>& pushBlock : pushBlocks) {
		pushBlock->SetSRV(SRV);
	}
	//押している間のスイッチ
	for (std::unique_ptr<PushButton>& pushButton : pushButtons) {
		pushButton->SetSRV(SRV);
	}
}
