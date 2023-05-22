#include "MoveEnemy.h"

void MoveEnemy::Initialize(FbxModel* MoveEnemyModel, Player* player)
{

	this->player = player;

	moveEnemyObject = new FbxObject3D;
	moveEnemyObject->Initialize();
	moveEnemyObject->SetModel(MoveEnemyModel);
	moveEnemyObject->PlayAnimation();

	//ƒ‰ƒWƒAƒ“
	rotate.y = 270 * (PI / 180);
}

void MoveEnemy::Update()
{

	Colision();

	moveEnemyObject->SetPosition(position);
	moveEnemyObject->SetScale(scale);
	moveEnemyObject->SetRotation(rotate);
	moveEnemyObject->Update();

}

void MoveEnemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	moveEnemyObject->Draw(cmdList);
}

void MoveEnemy::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	moveEnemyObject->DrawLightView(cmdList);
}

void MoveEnemy::Colision()
{
	float scaleX = scale.x * 100 / 2;
	float scaleY = scale.y * 100 / 2;
	float scaleZ = scale.z * 100 / 2;

	float bPosX1 = position.x - scaleX;
	float bPosX2 = position.x + scaleX;

	float bPosY1 = position.y - scaleY;
	float bPosY2 = position.y + scaleY;

	float bPosZ1 = position.z - scaleZ;
	float bPosZ2 = position.z + scaleZ;

	float pScaleX = 0.5f;
	float pScaleY = 0.5f;
	float pScaleZ = 0.5f;

	float pPosX1 = player->GetPosition().x - pScaleX;
	float pPosX2 = player->GetPosition().x + pScaleX;

	float pPosY1 = player->GetPosition().y - pScaleY;
	float pPosY2 = player->GetPosition().y + pScaleY;

	float pPosZ1 = player->GetPosition().z - pScaleZ;
	float pPosZ2 = player->GetPosition().z + pScaleZ;

	//“–‚½‚Á‚½‚ç
	if (pPosX1 < bPosX2 && bPosX1 < pPosX2) {

		if (pPosY1 < bPosY2 && bPosY1 < pPosY2) {

			if (pPosZ1 < bPosZ2 && bPosZ1 < pPosZ2) {

				player->Death();

			}
		}
	}
}

void MoveEnemy::DiscoverColision()
{
	float scaleX = scale.x * 100 / 2;
	float scaleY = scale.y * 100 / 2;
	float scaleZ = scale.z * 100 / 2;

	float bPosX1 = position.x - scaleX;
	float bPosX2 = position.x + scaleX;

	float bPosY1 = position.y - scaleY;
	float bPosY2 = position.y + scaleY;

	float bPosZ1 = position.z - scaleZ;
	float bPosZ2 = position.z + scaleZ;

	float pScaleX = 0.5f;
	float pScaleY = 0.5f;
	float pScaleZ = 0.5f;

	float pPosX1 = player->GetPosition().x - pScaleX;
	float pPosX2 = player->GetPosition().x + pScaleX;

	float pPosY1 = player->GetPosition().y - pScaleY;
	float pPosY2 = player->GetPosition().y + pScaleY;

	float pPosZ1 = player->GetPosition().z - pScaleZ;
	float pPosZ2 = player->GetPosition().z + pScaleZ;

	//“–‚½‚Á‚½‚ç
	if (pPosX1 < bPosX2 && bPosX1 < pPosX2) {

		if (pPosY1 < bPosY2 && bPosY1 < pPosY2) {

			if (pPosZ1 < bPosZ2 && bPosZ1 < pPosZ2) {

				player->Death();

			}
		}
	}
}

void MoveEnemy::SetSRV(ID3D12DescriptorHeap* SRV)
{
	moveEnemyObject->SetSRV(SRV);
}



