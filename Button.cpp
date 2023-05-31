#include "Button.h"
#include "imgui.h"
#include "FbxLoader.h"

void Button::Initialize(FbxModel* buttonUpModel, FbxModel* buttonDownModel, Player* player, CubeObject3D* cubeObject)
{
	//プレイヤー
	this->player = player;

	//ボタン
	buttonObject = new FbxObject3D;
	buttonObject->Initialize();
	buttonObject->SetModel(buttonUpModel);
	this->buttonUpModel = buttonUpModel;
	this->buttonDownModel = buttonDownModel;

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

	//音
	blockUpSE = new AudioManager();
	blockUpSE->SoundLoadWave("Resources/Audio/blockupSE.wav");
}

void Button::Update()
{
	if (reSound) {
		blockUpSE->StopWave();
		blockUpSE->SoundPlayWave(true, blockUpSEVolume);
		reSound = false;
	}

	colPosition = position;
	/*colPosition.y += 1.2f;*/

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
	/*cubeObject->Draw(cmdList);*/
}

void Button::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	buttonObject->DrawLightView(cmdList);
	blockObject->DrawLightView(cmdList);

}

void Button::ButtonCol()
{
	/*if (player->OntheBlock(cubeObject)) {
		if (push == false) {
			push = true;
			pushTimer = 0;
		}
	}

	player->pushback(cubeObject);*/

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

				if (push == false)
				{

					//押された
					push = true;
					pushTimer = 0;

					blockUpSE->StopWave();
					blockUpSE->SoundPlayWave(true, blockUpSEVolume);
					isSound = true;

					this->scale = { 0.3,0.3,0.3};
					this->position.y = 1.2;
					buttonObject->SetModel(buttonDownModel);
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

	/*if (cubeObject->CheakCollision(player->GetCubeObject())) {
		player->Death();
	};*/

}

void Button::PushButton()
{

	pushTimer++;

	MoveBlock();

	if (pushTimer >= pushCollTime) {
		//動き終わった
		push = false;

		blockUpSE->StopWave();
		isSound = false;

		this->scale = { 0.5,0.5,0.1 };
		this->position.y = 1.0;
		buttonObject->SetModel(buttonUpModel);
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

void Button::Reset()
{
	position = savePosition;
	blockPosition = saveBlockPosition;

	push = false;

	blockUpSE->StopWave();

	this->scale = { 0.5,0.5,0.1 };
	this->position.y = 1.0;
	buttonObject->SetModel(buttonUpModel);
}

void Button::StopSound()
{
	if (isSound) {
		blockUpSE->StopWave();
		reSound = true;
	}
}
