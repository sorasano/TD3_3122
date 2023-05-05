#include "Bomb.h"

void Bomb::Initialize(FbxModel* bombModel,Player * player)
{

	this->player = player;

	bombObject = new FbxObject3D;
	bombObject->Initialize();
	bombObject->SetModel(bombModel);

}

void Bomb::Update()
{

	Colision();

	bombObject->SetPosition(position);
	bombObject->SetScale(scale);
	bombObject->SetRotation(rotate);
	bombObject->Update();

}

void Bomb::Draw(ID3D12GraphicsCommandList* cmdList)
{
	bombObject->Draw(cmdList);
}

void Bomb::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	bombObject->DrawLightView(cmdList);
}

void Bomb::Colision()
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

void Bomb::SetSRV(ID3D12DescriptorHeap* SRV)
{
	bombObject->SetSRV(SRV);
}



