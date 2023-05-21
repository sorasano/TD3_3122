#include "WinApp.h"
#include "Message.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "DirectXTex.h"
#include "FPS.h"
#include "fbxsdk.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "imgui.h"
#include "MonochromeEffect.h"
#include "ReversalEffect.h"
#include "BlurEffect.h"
#include "MosaicEffect.h"
#include "ChromaticAberrationEffect.h"
#include "ShadowMap.h"
#include "DepthOfField.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//ウィンドウ生成
	WinApp* winApp = nullptr;
	winApp = WinApp::GetInstance();
	winApp->CreateWindow_(L"3122");

	//メッセージ
	Message* message;
	message = Message::GetInstance();

	//DirectX初期化処理
	DirectXCommon* dxCommon = nullptr;
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	//キーボード
	Input* input = nullptr;
	input = Input::GetInstance();
	input->Initialize(winApp);

	//ImGuiManager
	ImGuiManager* imGuiManager = nullptr;
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp,dxCommon);

	//ShadowMap
	ShadowMap* shadowMap = nullptr;
	ShadowMap::SetDevice(dxCommon->GetDevice());
	shadowMap = new ShadowMap;
	shadowMap->Initialize();
	shadowMap->CreateGraphicsPipeLine0();

	//被写界深度
	DepthOfField* depthOfField = nullptr;
	DepthOfField::SetDevice(dxCommon->GetDevice());
	depthOfField = new DepthOfField;
	depthOfField->Initialize();
	depthOfField->CreateGraphicsPipeLine();

	//ライト静的初期化
	LightGroup::StaticInitialize(dxCommon->GetDevice());

	//テクスチャマネージャーの初期化
	/*Texture::Initialize(dxCommon->GetDevice());*/
	//スプライト共通部の初期化
	/*SpriteManager* spriteManager = nullptr;
	spriteManager = SpriteManager::GetInstance();
	spriteManager->Initialize(dxCommon, window_width, window_height);*/

	//ゲームシーン
	GameScene* gameScene = nullptr;
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon, input);

	//FPSを固定
	FPS* fps = nullptr;
	fps = new FPS;
	fps->SetFrameRate(60.0f);
	fps->FpsControlBegin();

	//ゲームループ
	while (true)
	{
		//✖ボタンでゲームループ終了
		if (message->Update() == 1)break;

		fps->FpsControlBegin();

		//キーボード更新
		input->Update();

		imGuiManager->Begin();

		//shadowMap
		shadowMap->SetAlpha(1.0f);
		shadowMap->SetLightVP(gameScene->GetLightViewProjection());
		shadowMap->Update();

		//ゲームシーン更新
		gameScene->Update();

		//shadowMap
		shadowMap->PreDrawScene0(dxCommon->GetCommandList());
		gameScene->DrawFBXLightView();
		shadowMap->PostDrawScene0(dxCommon->GetCommandList());
		//ゲームシーンにSRVを渡す
		gameScene->SetSRV(shadowMap->GetSRV());

		// 4. 描画コマンド
		dxCommon->PreDraw();

		gameScene->Draw();

		imGuiManager->End();
		imGuiManager->Draw();

		dxCommon->PostDraw();

	fps->FpsControlEnd();

	}

	dxCommon->EndImgui();

	imGuiManager->Finalize();

	//ウィンドウクラスを登録解除
	winApp->deleteWindow();

	return 0;
}
