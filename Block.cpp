#include "Block.h"

Input* Block::input = nullptr;
DXInput* Block::dxInput = nullptr;

void Block::Initialize(FbxModel* blockModel, Player* player, CubeObject3D* cubeObject)
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

void Block::Update()
{
	//����
	Collision();
	//�u���b�N
	blockObject->SetPosition(position);
	blockObject->SetScale(scale);
	blockObject->SetRotation(rotate);
	blockObject->Update();

	//����
	cubeObject->SetPosition(position);
	cubeObject->Update();
}

void Block::Draw(ID3D12GraphicsCommandList* cmdList)
{
	blockObject->Draw(cmdList);
	/*cubeObject->Draw(cmdList);*/
}

void Block::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	blockObject->DrawLightView(cmdList);
}

void Block::Collision()
{
	player->pushback(cubeObject);

}


void Block::pushback(CubeObject3D* cubeObject)
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

void Block::SetSRV(ID3D12DescriptorHeap* SRV)
{
	blockObject->SetSRV(SRV);
}

