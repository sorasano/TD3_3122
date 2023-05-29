#include "Ladder.h"
#define PI 3.1415

void Ladder::Initialize(FbxModel* LadderModel, Player* player)
{

	this->player = player;

	ladderObject = new FbxObject3D;
	ladderObject->Initialize();
	ladderObject->SetModel(LadderModel);

	//ƒ‰ƒWƒAƒ“
	rotate.y = 90 * (PI / 180);
}

void Ladder::Update()
{
	rotate.y = 90 * (PI / 180);

	Colision();

	ladderObject->SetPosition(position);
	ladderObject->SetScale(scale);
	ladderObject->SetRotation(rotate);
	ladderObject->Update();

}

void Ladder::Draw(ID3D12GraphicsCommandList* cmdList)
{
	ladderObject->Draw(cmdList);
}

void Ladder::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	ladderObject->DrawLightView(cmdList);
}

void Ladder::Colision()
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

	float pScaleX = 0.2f;
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

				player->Ladder();

			}
		}
	}


}

void Ladder::SetSRV(ID3D12DescriptorHeap* SRV)
{
	ladderObject->SetSRV(SRV);
}



