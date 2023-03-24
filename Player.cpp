#include "Player.h"

Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

void Player::Initialize(FbxModel* playerModel)
{

	playerObject = new FbxObject3D;
	playerObject->Initialize();
	playerObject->SetModel(playerModel);

}

void Player::Update()
{

	if (input->PushKey(DIK_A))
	{
		position.x -= 0.1;
	}
	if (input->PushKey(DIK_D))
	{
		position.x += 0.1;
	}

	playerObject->SetPosition(position);
	playerObject->SetScale(scale);
	playerObject->SetRotation(rotate);
	playerObject->Update();

}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	playerObject->Draw(cmdList);
}

void Player::SetPosition(XMFLOAT3 position)
{
	this->position = position;
}

void Player::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
}

void Player::Setrotate(XMFLOAT3 rotate)
{
	this->rotate = rotate;
}
