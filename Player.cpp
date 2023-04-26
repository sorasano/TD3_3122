#include "Player.h"

Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

#define PI 3.1415

void Player::Initialize(FbxModel* playerModel, CubeObject3D* cubeObject)
{

	playerObject = new FbxObject3D;
	playerObject->Initialize();
	playerObject->SetModel(playerModel);
	playerObject->PlayAnimation();
	//���W�A���Ŋp�x�ύX
	rotate.y = 90 * (PI / 180);

	//����
	this->cubeObject = cubeObject;
	this->cubeObject->SetScale(XMFLOAT3(1, 1, 1));
}

void Player::Update()
{

	if (isDeath == false) {

		//���ɓ����Ă��邩
		if (inSwamp) {

			if (input->PushKey(DIK_A))
			{
				position.x -= swampSpeed;
			}
			if (input->PushKey(DIK_D))
			{
				position.x += swampSpeed;
			}

		}
		else {

			if (input->PushKey(DIK_A))
			{
				position.x -= speed;
			}
			if (input->PushKey(DIK_D))
			{
				position.x += speed;
			}
			//�W�����v
			if (input->PushKey(DIK_SPACE)) {
				if (isJump == false) {
					Jump();
					isJump = true;
				}
			}

		}
	}

	//���Z�b�g
	inSwamp = false;

	//�d��
	position.y += gravity;
	gravity -= gravitySpeed;

	//1.0f�ȉ��ɂȂ�����(�n�ʂɓ������Ă����)
	if (position.y <= 1.0f) {
		SetJump(false);
		gravity = 0.0f;
		position.y = 1.0f;
	}

	//����
	cubeObject->SetPosition(position);
	cubeObject->Update();

	playerObject->SetPosition(position);
	playerObject->SetScale(scale);
	playerObject->SetRotation(rotate);
	playerObject->Update();

}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (isDeath == false) {
		playerObject->Draw(cmdList);
		//cubeObject->Draw(cmdList);
	}

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

void Player::Death()
{
	isDeath = true;
}

void Player::Jump()
{
	gravity = jumpGravity;
}

void Player::Swamp()
{
	inSwamp = true;
}
