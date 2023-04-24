#include "Box.h"
#include "FbxLoader.h"

Input* Box::input = nullptr;
DXInput* Box::dxInput = nullptr;

void Box::Initialize(FbxModel* BoxModel, Player* player)
{
	//プレイヤー
	this->player = player;

	//box
	boxObject = new FbxObject3D;
	boxObject->Initialize();
	boxObject->SetModel(BoxModel);

}

void Box::Update()
{
	Colision();

	//ボタン
	boxObject->SetPosition(position);
	boxObject->SetScale(scale);
	boxObject->SetRotation(rotate);
	boxObject->Update();

}

void Box::Draw(ID3D12GraphicsCommandList* cmdList)
{
	boxObject->Draw(cmdList);
}

void Box::Colision()
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



	//当たったら
	if (pPosX1 < bPosX2 && bPosX1 < pPosX2) {

		if (pPosY1 < bPosY2 && bPosY1 < pPosY2) {

			if (pPosZ1 < bPosZ2 && bPosZ1 < pPosZ2) {

				

			}
		}
	}

}

