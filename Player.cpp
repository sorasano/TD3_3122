#include "Player.h"
#include "imgui.h"
#include "FbxLoader.h"

Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

#define PI 3.1415

void Player::Initialize(CubeObject3D* cubeObject)
{

	playerObject = new FbxObject3D;
	playerObject->Initialize();

	playerWaitModel = FbxLoader::GetInstance()->LoadModelFromFile("humanWait", "Resources/color/blue1x1.png");
	playerWalkModel = FbxLoader::GetInstance()->LoadModelFromFile("humanWalk", "Resources/color/blue1x1.png");
	playerJumpModel = FbxLoader::GetInstance()->LoadModelFromFile("humanJump", "Resources/color/blue1x1.png");

	playerObject->SetModel(playerWaitModel);
	playerObject->PlayAnimation();
	
	//ラジアン
	rotate.y = 90 * (PI / 180);

	//判定
	this->cubeObject_ = cubeObject;
	this->cubeObject_->SetScale(XMFLOAT3(0.5, 1.5, 1));
}

void Player::Update()
{

	action = WAIT;

	if (alpha == 0.0f) {
		//沼に入っているか
		if (inSwamp) {
			if (input->PushKey(DIK_A))
			{
				position.x -= swampSpeed;
				rotate.y = 270 * (PI / 180);

				action = WALK;
			}
			else if (input->PushKey(DIK_D))
			{
				position.x += swampSpeed;
				rotate.y = 90 * (PI / 180);

				action = WALK;
			}
		}
		else {
			if (input->PushKey(DIK_A) && position.x > -5)
			{
				position.x -= speed;
				rotate.y = 270 * (PI / 180);

				action = WALK;
			}
			else if (input->PushKey(DIK_D))
			{
				position.x += speed;
				rotate.y = 90 * (PI / 180);

				action = WALK;
			}

			//ジャンプ
			if (input->PushKey(DIK_SPACE)) {
				if (isJump == false) {
					Jump();
					isJump = true;

				}
			}

		}
	}

	//リセット
	inSwamp = false;

	//重力
	position.y += gravity;
	gravity -= gravitySpeed;

	//1.0f以下になったら(地面に当たっていれば)
	if (position.y <= 1.0f) {
		SetJump(false);
		gravity = 0.0f;
		position.y = 1.0f;
	}

	if (isJump == true) {
		action = JUMP;
	}

	//モデルの変更

	if (action != oldAction) {

		if (action == WAIT) {
			playerObject->SetModel(playerWaitModel);
			playerObject->PlayAnimation();
		}
		else if (action == WALK) {
			playerObject->SetModel(playerWalkModel);
			playerObject->PlayAnimation();
		}
		else if (action == JUMP) {
			playerObject->SetModel(playerJumpModel);
			playerObject->PlayAnimation();
		}
	}

	oldAction = action;

	//ImGui::Begin("pPos");
	//ImGui::Text("pPosX = %f \n",position.x);
	//ImGui::End();

	//判定
	colposition = position;
	colposition.y += 0.5f;
	cubeObject_->SetPosition(colposition);
	cubeObject_->Update();

	playerObject->SetPosition(position);
	playerObject->SetScale(scale);
	playerObject->SetRotation(rotate);
	playerObject->Update();

}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (isDeath == false) {

		playerObject->Draw(cmdList);
		/*cubeObject_->Draw(cmdList);*/
	}

}

void Player::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	playerObject->DrawLightView(cmdList);
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

void Player::SetSRV(ID3D12DescriptorHeap* SRV)
{
	playerObject->SetSRV(SRV);
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

void Player::pushback(CubeObject3D* cubeObject)
{

	//横の判定
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
		//沼の中
		if (inSwamp) {
			if (input->PushKey(DIK_A)) {
				newposition = position;
				newposition.x = (position.x - swampSpeed);
				cubeObject_->SetPosition(newposition);
				if (cubeObject_->CheakCollision(cubeObject)) {
					position.x += swampSpeed;
				}
			}
			if (input->PushKey(DIK_D)) {
				newposition = position;
				newposition.x = (position.x + swampSpeed);
				cubeObject_->SetPosition(newposition);
				if (cubeObject_->CheakCollision(cubeObject)) {
					position.x -= swampSpeed;
				}
			}
			cubeObject_->SetPosition(newposition);
		}
		else {
			if (input->PushKey(DIK_A)) {
				newposition = position;
				newposition.x = (position.x - speed);
				cubeObject_->SetPosition(newposition);
				if (cubeObject_->CheakCollision(cubeObject)) {
					position.x += speed;
				}
			}
			if (input->PushKey(DIK_D)) {
				newposition = position;
				newposition.x = (position.x + speed);
				cubeObject_->SetPosition(newposition);
				if (cubeObject_->CheakCollision(cubeObject)) {
					position.x -= speed;
				}
			}

		}
	}
	//落下中(ジャンプも含む)
	if (gravity <= 0.0f) {
		newposition = position;
		newposition.y += gravity;
		cubeObject_->SetPosition(newposition);
		if (cubeObject_->CheakCollision(cubeObject)) {
			SetJump(false);
			gravity = 0.0f;
		}
	}

}

bool Player::OntheBlock(CubeObject3D* cubeObject)
{
	//落下中(ジャンプも含む)
	if (gravity <= 0.0f) {
		newposition = position;
		newposition.y += gravity;
		cubeObject_->SetPosition(newposition);
		if (cubeObject_->CheakCollision(cubeObject)) {
			/*SetJump(false);
			gravity = 0.0f;*/
			return true;
		}
	}
	return false;
	
}
