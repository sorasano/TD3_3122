#include "PushBlock.h"

Input* PushBlock::input = nullptr;

void PushBlock::Initialize(FbxModel* blockModel, Player* player, CubeObject3D* cubeObject)
{
	this->player = player;

	//�u���b�N
	blockObject = new FbxObject3D;
	blockObject->Initialize();
	blockObject->SetModel(blockModel);
	blockObject->Update();

	//����
	this->cubeObject = cubeObject;
	this->cubeObject->SetScale(XMFLOAT3(scale.x * 100.0f, scale.y * 100.0f, scale.z * 100.0f));

	//��
	pushSE = new AudioManager();
	pushSE->SoundLoadWave("Resources/Audio/PushBlock.wav");
}

void PushBlock::Update()
{
	////����
	//Collision();
	////��������
	//if (isPush) {
	//	Push();
	//}
	//�u���b�N
	blockObject->SetPosition(position);
	blockObject->SetScale(scale);
	blockObject->SetRotation(rotate);
	blockObject->Update();

	//����
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
	//���ɂ�������
	this->cubeObject->SetPosition(newposition);
	//���̃L���[�u�ɓ�����Ɠ����Ȃ�
	if (this->cubeObject->CheakCollision(cubeObject)) {
		/*NoPush();*/
		/*this->cubeObject->SetPosition(position);*/

		noPush = true;
	}
	//������
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