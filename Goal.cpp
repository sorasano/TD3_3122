#include "Goal.h"

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
		}
		else {

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
