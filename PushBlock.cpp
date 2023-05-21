#include "PushBlock.h"

Input* PushBlock::input = nullptr;
DXInput* PushBlock::dxInput = nullptr;

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
	cubeObject->Draw(cmdList);
}

void PushBlock::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	blockObject->DrawLightView(cmdList);
}

void PushBlock::Collision()
{
	if (player->pushBlock(cubeObject)) {
		isPush = true;
	}
	else {
		isPush = false;
	}
}

void PushBlock::Push()
{
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
		if (player->GetinSwamp()) {
			if (input->PushKey(DIK_A)) {
				position.x -= player->GetSwampSpeed();
			}
			if (input->PushKey(DIK_D)) {
				position.x += player->GetSwampSpeed();
			}
		}
		else {
			if (input->PushKey(DIK_A)) {
				position.x -= player->GetSpeed();
			}
			if (input->PushKey(DIK_D)) {
				position.x += player->GetSpeed();
			}
		}
	}
}

void PushBlock::NoPush()
{
	/*if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
		if (player->GetinSwamp()) {
			if (input->PushKey(DIK_A)) {
				position.x += player->GetSwampSpeed();
			}
			if (input->PushKey(DIK_D)) {
				position.x -= player->GetSwampSpeed();
			}
		}
		else {
			if (input->PushKey(DIK_A)) {
				position.x += player->GetSpeed();
			}
			if (input->PushKey(DIK_D)) {
				position.x -= player->GetSpeed();
			}
		}
	}*/
	player->pushback(this->cubeObject);
}

void PushBlock::pushback(CubeObject3D* cubeObject)
{
	
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
		if (player->GetinSwamp()) {
			if (input->PushKey(DIK_A)) {
				newposition = position;
				newposition.x -= player->GetSwampSpeed();
			}
			if (input->PushKey(DIK_D)) {
				newposition = position;
				newposition.x += player->GetSwampSpeed();
			}
		}
		else {
			if (input->PushKey(DIK_A)) {
				newposition = position;
				newposition.x -= player->GetSpeed();
			}
			if (input->PushKey(DIK_D)) {
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
