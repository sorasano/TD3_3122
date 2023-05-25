#include "Menu.h"
Input* Menu::input = nullptr;
DXInput* Menu::dxInput = nullptr;

void Menu::Initialize()
{

	baseSprite = new Sprite();
	baseSprite->SetTextureNum(7);
	baseSprite->Initialize();
	baseSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	baseSprite->SetScale(XMFLOAT2(400, 500));
	baseSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2));
	baseSprite->Update();

	restartSprite = new Sprite();
	restartSprite->SetTextureNum(8);
	restartSprite->Initialize();
	restartSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	restartSprite->SetScale(XMFLOAT2(400, 100));
	restartSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2 - 50));
	restartSprite->Update();

	titleSprite = new Sprite();
	titleSprite->SetTextureNum(9);
	titleSprite->Initialize();
	titleSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	titleSprite->SetScale(XMFLOAT2(400, 100));
	titleSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2 + 50));
	titleSprite->Update();

	closeSprite = new Sprite();
	closeSprite->SetTextureNum(10);
	closeSprite->Initialize();
	closeSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	closeSprite->SetScale(XMFLOAT2(400, 100));
	closeSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2 + 150));
	closeSprite->Update();

	this->position = baseSprite->GetPosition();
}

void Menu::Update()
{

	//メニュー操作

	if (input->TriggerKey(DIK_W))
	{
		if (serect != MENURESET) {
			serect--;
		}
	}
	else if (input->TriggerKey(DIK_S))
	{
		if (serect != MENUCLOSE) {
			serect++;
		}
	}


	//選択中を強調
	if (serect == MENURESET) {
		restartSprite->color = { 1,1,0,1 };

		titleSprite->color = { 1,1,1,1 };
		closeSprite->color = { 1,1,1,1 };
	}
	else if (serect == MENUTITLE) {
		titleSprite->color = { 1,1,0,1 };

		restartSprite->color = { 1,1,1,1 };
		closeSprite->color = { 1,1,1,1 };
	}
	else if (serect == MENUCLOSE) {
		closeSprite->color = { 1,1,0,1 };

		titleSprite->color = { 1,1,1,1 };
		restartSprite->color = { 1,1,1,1 };
	}

	//スペースを押したらシーン変更
	if (input->PushKey(DIK_SPACE)) {
		isSerect = true;
	}



	this->position = baseSprite->GetPosition();

	baseSprite->Update();
	restartSprite->Update();
	titleSprite->Update();
	closeSprite->Update();
}

void Menu::Draw(ID3D12GraphicsCommandList* cmdList)
{

	baseSprite->Draw(cmdList);
	restartSprite->Draw(cmdList);
	titleSprite->Draw(cmdList);
	closeSprite->Draw(cmdList);

}

void Menu::SetPosition(XMFLOAT2 position)
{
	//ベースを移動
	baseSprite->SetPosition(position);
	//スプライトの移動量
	XMFLOAT2 positionMove = XMFLOAT2{ this->position.x - position.x , this->position.y - position.y };

	//移動しただけ追従して移動させる
	XMFLOAT2 restartPosition = XMFLOAT2{ restartSprite->GetPosition().x - positionMove.x ,  restartSprite->GetPosition().y - positionMove.y };
	restartSprite->SetPosition(restartPosition);

	XMFLOAT2 titlePosition = XMFLOAT2{ titleSprite->GetPosition().x - positionMove.x ,  titleSprite->GetPosition().y - positionMove.y };
	titleSprite->SetPosition(titlePosition);

	XMFLOAT2 closePosition = XMFLOAT2{ closeSprite->GetPosition().x - positionMove.x ,  closeSprite->GetPosition().y - positionMove.y };
	closeSprite->SetPosition(closePosition);
}

void Menu::Reset()
{
	serect = MENURESET;
	isSerect = false;
}
