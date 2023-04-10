#include "Button.h"
#include "imgui.h"
#include "FbxLoader.h"

Input* Button::input = nullptr;
DXInput* Button::dxInput = nullptr;

void Button::Initialize(FbxModel* ButtonModel, Player* player)
{
	//プレイヤー
	this->player = player;

	//ボタン
	buttonObject = new FbxObject3D;
	buttonObject->Initialize();
	buttonObject->SetModel(ButtonModel);

	//ブロック
	blockModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/gray1x1.png");
	blockObject = new FbxObject3D2;
	blockObject->Initialize();
	blockObject->SetModel(blockModel);

	blockObject->SetPosition(blockPosition);
	blockObject->SetScale(blockScale);
	blockObject->SetRotation(blockRotate);
	blockObject->Update();

}

void Button::Update()
{

	ButtonCol();

	BlockCol();

	if (push) {
		PushButton();
	}

	//ボタン
	buttonObject->SetPosition(position);
	buttonObject->SetScale(scale);
	buttonObject->SetRotation(rotate);
	buttonObject->Update();

	//ブロック
	blockObject->SetPosition(blockPosition);
	blockObject->SetScale(blockScale);
	blockObject->SetRotation(blockRotate);
	blockObject->Update();
}

void Button::Draw(ID3D12GraphicsCommandList* cmdList)
{
	buttonObject->Draw(cmdList);
	blockObject->Draw(cmdList);
}

void Button::ButtonCol()
{

	float scale = 0.5;

	float bPosX1 = position.x - scale;
	float bPosX2 = position.x + scale;

	float bPosY1 = position.y - scale;
	float bPosY2 = position.y + scale;

	float bPosZ1 = position.z - scale;
	float bPosZ2 = position.z + scale;

	float pPosX1 = player->GetPosition().x - scale;
	float pPosX2 = player->GetPosition().x + scale;

	float pPosY1 = player->GetPosition().y - scale;
	float pPosY2 = player->GetPosition().y + scale;

	float pPosZ1 = player->GetPosition().z - scale;
	float pPosZ2 = player->GetPosition().z + scale;



	//当たってスペースを押したら
	if (pPosX1 < bPosX2 && bPosX1 < pPosX2) {

		if (pPosY1 < bPosY2 && bPosY1 < pPosY2) {

			if (pPosZ1 < bPosZ2 && bPosZ1 < pPosZ2) {

				if (input->PushKey(DIK_SPACE) && push == false)
				{
					push = true;
					pushTimer = 0;

				}

			}
		}
	}

}

void Button::BlockCol()
{
	float scale = 1.0;

	float bPosX1 = blockPosition.x - scale;
	float bPosX2 = blockPosition.x + scale;

	float bPosY1 = blockPosition.y;
	float bPosY2 = blockPosition.y + scale;

	float bPosZ1 = blockPosition.z - scale;
	float bPosZ2 = blockPosition.z + scale;

	float pPosX1 = player->GetPosition().x - scale;
	float pPosX2 = player->GetPosition().x + scale;

	float pPosY1 = player->GetPosition().y - scale;
	float pPosY2 = player->GetPosition().y + scale;

	float pPosZ1 = player->GetPosition().z - scale;
	float pPosZ2 = player->GetPosition().z + scale;

	//当たったら
	if (pPosX1 < bPosX2 && bPosX1 < pPosX2) {

		if (pPosY1 < bPosY2 && bPosY1 < pPosY2) {

			if (pPosZ1 < bPosZ2 && bPosZ1 < pPosZ2) {

				player->Death();

			}
		}
	}

}

void Button::PushButton()
{

	pushTimer++;
	position.y = 0.2;

	MoveBlock();

	if (pushTimer >= pushCollTime) {
		push = false;
		position.y = 0.5;
	}

}

void Button::MoveBlock()
{

	int harfTime = pushCollTime / 2;
	//1フレーム当たりの移動幅
	float flameMove = (upHight / pushCollTime) * 2;

	if (pushTimer <= harfTime) {
		//経過時間が全体の半分以下だったら上がる
		blockPosition.y += flameMove;
	}
	else {
		//経過時間が全体の半分以上だったら下がる
		blockPosition.y -= flameMove;
	}

}
