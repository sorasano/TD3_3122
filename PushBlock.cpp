#include "PushBlock.h"

Input* PushBlock::input = nullptr;

void PushBlock::Initialize(FbxModel* blockModel, Player* player, CubeObject3D* cubeObject)
{
	this->player = player;

	//ブロック
	blockObject = new FbxObject3D;
	blockObject->Initialize();
	blockObject->SetModel(blockModel);
	blockObject->Update();

	//判定
	this->cubeObject = cubeObject;
	this->cubeObject->SetScale(XMFLOAT3(scale.x * 100.0f, scale.y * 100.0f, scale.z * 100.0f));

	//音
	pushSE = new AudioManager();
	pushSE->SoundLoadWave("Resources/Audio/PushBlock.wav");
}

void PushBlock::Update()
{
	////判定
	//Collision();
	////押す判定
	//if (isPush) {
	//	Push();
	//}
	//ブロック
	blockObject->SetPosition(position);
	blockObject->SetScale(scale);
	blockObject->SetRotation(rotate);
	blockObject->Update();

	//判定
	cubeObject->SetPosition(position);
	cubeObject->Update();
}

void PushBlock::Draw(ID3D12GraphicsCommandList* cmdList)
{
	blockObject->Draw(cmdList);
	/*cubeObject->Draw(cmdList);*/
}

void PushBlock::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	blockObject->DrawLightView(cmdList);
}

void PushBlock::Reset()
{
	position = savepos;
}

void PushBlock::Collision()
{
	if (player->pushBlock(cubeObject)) {
		isPush = true;
	}
	else {
		pushSE->StopWave();
		isPush = false;
	}
}

void PushBlock::Push()
{
	if (input->IsKeyPress(DIK_A) || input->IsKeyPress(DIK_D) || input->IsDownLStickLeft() || input->IsDownLStickRight()) {
		if (player->GetinSwamp()) {
			if (input->IsKeyPress(DIK_A) || input->IsDownLStickLeft()) {
				position.x -= player->GetSwampSpeed();
			}
			else if (input->IsKeyPress(DIK_D) || input->IsDownLStickRight()) {
				position.x += player->GetSwampSpeed();
			}
		}
		else {
			if (input->IsKeyPress(DIK_A) || input->IsDownLStickLeft()) {
				position.x -= player->GetSpeed();
			}
			else if (input->IsKeyPress(DIK_D) || input->IsDownLStickRight()) {
				position.x += player->GetSpeed();
			}
		}
		pushSE->SoundPlayWave(true,pushSEVolume);
	}
}

void PushBlock::NoPush()
{
	/*if (input->IsKeyPress(DIK_A) || input->IsKeyPress(DIK_D)) {
		if (player->GetinSwamp()) {
			if (input->IsKeyPress(DIK_A)) {
				position.x += player->GetSwampSpeed();
			}
			if (input->IsKeyPress(DIK_D)) {
				position.x -= player->GetSwampSpeed();
			}
		}
		else {
			if (input->IsKeyPress(DIK_A)) {
				position.x += player->GetSpeed();
			}
			if (input->IsKeyPress(DIK_D)) {
				position.x -= player->GetSpeed();
			}
		}
	}*/
	pushSE->StopWave();
	player->pushback(this->cubeObject);
}

void PushBlock::pushback(CubeObject3D* cubeObject)
{

	if (input->IsKeyPress(DIK_A) || input->IsKeyPress(DIK_D) || input->IsDownLStickLeft() || input->IsDownLStickRight()) {
		if (player->GetinSwamp()) {
			if (input->IsKeyPress(DIK_A) || input->IsDownLStickLeft()) {
				newposition = position;
				newposition.x -= player->GetSwampSpeed();
			}
			else if (input->IsKeyPress(DIK_D) || input->IsDownLStickRight()) {
				newposition = position;
				newposition.x += player->GetSwampSpeed();
			}
		}
		else {
			if (input->IsKeyPress(DIK_A) || input->IsDownLStickLeft()) {
				newposition = position;
				newposition.x -= player->GetSpeed();
			}
			else if (input->IsKeyPress(DIK_D) || input->IsDownLStickRight()) {
				newposition = position;
				newposition.x += player->GetSpeed();
			}
		}
	}
	//仮にうごかす
	this->cubeObject->SetPosition(newposition);
	//他のキューブに当たると動かない
	if (this->cubeObject->CheakCollision(cubeObject)) {
		/*NoPush();*/
		/*this->cubeObject->SetPosition(position);*/

		noPush = true;
	}
	//動かす
	else {
		/*player->pushback(this->cubeObject);*/
		/*Push();*/
		noPush = false;
	}

}

void PushBlock::SetSRV(ID3D12DescriptorHeap* SRV)
{
	blockObject->SetSRV(SRV);
}