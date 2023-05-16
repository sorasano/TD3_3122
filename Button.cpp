#include "Button.h"
#include "imgui.h"
#include "FbxLoader.h"

void Button::Initialize(FbxModel* ButtonModel, Player* player, CubeObject3D* cubeObject)
{
	//プレイヤー
	this->player = player;

	//ボタン
	buttonObject = new FbxObject3D;
	buttonObject->Initialize();
	buttonObject->SetModel(ButtonModel);

	//ブロック
	blockModel = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/color/red1x1.png");
	blockObject = new FbxObject3D;
	blockObject->Initialize();
	blockObject->SetModel(blockModel);

	blockObject->SetPosition(blockPosition);
	blockObject->SetScale(blockScale);
	blockObject->SetRotation(blockRotate);
	blockObject->Update();

	//判定
	this->cubeObject = cubeObject;
	this->cubeObject->SetScale(XMFLOAT3(scale.x * 100.0f, scale.y * 100.0f, scale.z * 100.0f));
}

void Button::Update()
{

	colPosition = position;
	/*colPosition.y -= 0.2f;*/

	//判定
	cubeObject->SetPosition(colPosition);
	cubeObject->Update();

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
	cubeObject->Draw(cmdList);
}

void Button::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	buttonObject->DrawLightView(cmdList);
	blockObject->DrawLightView(cmdList);

}

void Button::ButtonCol()
{
	if (player->OntheBlock(cubeObject)) {
		if (push == false) {
			push = true;
			pushTimer = 0;
		}
	}
	player->pushback(cubeObject);
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

	/*if (cubeObject->CheakCollision(player->GetCubeObject())) {
		player->Death();
	};*/

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

void Button::SetSRV(ID3D12DescriptorHeap* SRV)
{
	buttonObject->SetSRV(SRV);
	blockObject->SetSRV(SRV);
}
