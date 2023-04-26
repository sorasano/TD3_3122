#include "Goal.h"

Input* Goal::input = nullptr;
DXInput* Goal::dxInput = nullptr;


void Goal::Initialize(Sprite* whiteSprite,Sprite* clearSprite, Player* player)
{
	this->whiteSprite = whiteSprite;
	this->clearSprite = clearSprite;
	this->player = player;

	isClear = false;
	clearPos = 10;
}

void Goal::Update()
{
	if (player->GetPosition().x >= clearPos) {
		isClear = true;
		player->SetisClear(isClear);
	}
	if (isClear) {
		if (alpha < 1) {
			whiteSprite->SetAlpha(alpha);
			clearSprite->SetAlpha(alpha);
			alpha += 0.005f;
			player->SetAlpha(alpha);
		}
		else {
			//クリア演出後スペースで戻る
			if (input->TriggerKey(DIK_SPACE)) {
				isClear = false;
				alpha = 0.0f;
				player->SetisClear(isClear);
				player->SetAlpha(alpha);
				player->SetPosition(XMFLOAT3(0, 1, -1));
			}
		}
	}

	whiteSprite->Update();
	clearSprite->Update();
}

void Goal::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (isClear) {
		whiteSprite->Draw(cmdList);
		clearSprite->Draw(cmdList);
	}
}
