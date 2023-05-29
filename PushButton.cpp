#include "PushButton.h"
#include "imgui.h"
#include "FbxLoader.h"

void PushButton::Initialize(FbxModel* ButtonModel, Player* player, CubeObject3D* buttonObject, CubeObject3D* blockObject)
{
	//プレイヤー
	this->player = player;

	//ボタン
	this->buttonObject = new FbxObject3D;
	this->buttonObject->Initialize();
	this->buttonObject->SetModel(ButtonModel);

	//ブロック
	blockModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/red1x1.png");
	this->blockObject = new FbxObject3D;
	this->blockObject->Initialize();
	this->blockObject->SetModel(blockModel);

	this->blockObject->SetPosition(blockPosition);
	this->blockObject->SetScale(blockScale);
	this->blockObject->SetRotation(blockRotate);
	this->blockObject->Update();

	//判定
	buttonColBox = buttonObject;
	buttonColBox->SetScale(XMFLOAT3(scale.x * 100.0f, scale.y * 100.0f, scale.z * 100.0f));

	blockColBox = blockObject;
	blockColBox->SetScale(XMFLOAT3(blockScale.x * 100.0f, blockScale.y * 100.0f, blockScale.z * 100.0f));
}

void PushButton::Update()
{

	buttonColPosition = savepos;
	buttonColPosition.y += 0.5f;
	blockColPosition = blockPosition;
	blockColPosition.y += 1.5f;
	/*colPosition.y -= 0.2f;*/

	//判定
	buttonColBox->SetPosition(buttonColPosition);
	buttonColBox->Update();
	blockColBox->SetPosition(blockColPosition);
	blockColBox->Update();

	BlockCol();

	if (buttonColBox->CheakCollision(player->GetCubeObject())) {
		push = true;
	}

	if (push) {
		Push();
	}
	else {
		MoveBlock();
		position.y = 0.5f;
	}
	push = false;

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

void PushButton::Draw(ID3D12GraphicsCommandList* cmdList)
{
	buttonObject->Draw(cmdList);
	blockObject->Draw(cmdList);
	/*buttonColBox->Draw(cmdList);
	blockColBox->Draw(cmdList);*/
}

void PushButton::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	buttonObject->DrawLightView(cmdList);
	blockObject->DrawLightView(cmdList);

}

void PushButton::ButtonCol(CubeObject3D* cubeobject)
{

	//横の判定
	if (buttonColBox->CheakCollision(cubeobject)) {
		push = true;
	}
	
	

	//float scale = 0.5;

	//float bPosX1 = position.x - scale;
	//float bPosX2 = position.x + scale;

	//float bPosY1 = position.y - scale;
	//float bPosY2 = position.y + scale;

	//float bPosZ1 = position.z - scale;
	//float bPosZ2 = position.z + scale;

	//float pPosX1 = player->GetPosition().x - scale;
	//float pPosX2 = player->GetPosition().x + scale;

	//float pPosY1 = player->GetPosition().y - scale;
	//float pPosY2 = player->GetPosition().y + scale;

	//float pPosZ1 = player->GetPosition().z - scale;
	//float pPosZ2 = player->GetPosition().z + scale;



	////当たってスペースを押したら
	//if (pPosX1 < bPosX2 && bPosX1 < pPosX2) {

	//	if (pPosY1 < bPosY2 && bPosY1 < pPosY2) {

	//		if (pPosZ1 < bPosZ2 && bPosZ1 < pPosZ2) {

	//			if (push == false)
	//			{
	//				push = true;
	//				pushTimer = 0;

	//			}

	//		}
	//	}
	//}

}

void PushButton::BlockCol()
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

	/*if (cubeObject->CheakCollision(player->GetCubeObject())) {
		player->Death();
	};*/

}

void PushButton::Push()
{
	position.y = 0.2;
	MoveBlock();
}

void PushButton::MoveBlock()
{

	int harfTime = pushCollTime / 2;
	//1フレーム当たりの移動幅
	float flameMove = (upHight / pushCollTime) * 2;

	if (push) {
		//上の制限
		if (blockPosition.y < upHight) {
			//経過時間が全体の半分以下だったら上がる
			blockPosition.y += flameMove;
		}
	}
	else {
		//下の制限
		if (blockPosition.y > 1.0f) {
			//経過時間が全体の半分以上だったら下がる
			blockPosition.y -= flameMove;
		}
	}

}

void PushButton::Reset()
{
	position = savepos;
	blockPosition = saveblockpos;
}

void PushButton::SetSRV(ID3D12DescriptorHeap* SRV)
{
	buttonObject->SetSRV(SRV);
	blockObject->SetSRV(SRV);
}
