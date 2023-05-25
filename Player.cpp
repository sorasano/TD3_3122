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
	playerClimbModel = FbxLoader::GetInstance()->LoadModelFromFile("humanClimb", "Resources/color/blue1x1.png");

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


		//梯子に当たっているときにWかSを押したら梯子に登る
		if (colLadder) {
			if (input->PushKey(DIK_W) || input->PushKey(DIK_S))
			{
				onLadder = true;
			}
		}

		//梯子に乗っているか
		if (onLadder) {

			action = CLIMB;

			if (input->PushKey(DIK_W))
			{
				if (colLadder) {
					position.y += speed;
					rotate.y = 180 * (PI / 180);

					stopAnimation = false;
				}
				else {
					stopAnimation = true;
				}

			}
			else if (input->PushKey(DIK_S))
			{
				position.y -= speed;
				rotate.y = 180 * (PI / 180);

				if (position.y <= 1.0f) {
					//下降して地面についたら梯子から離れる
					onLadder = false;
				}

				stopAnimation = false;
			}
			else {
				stopAnimation = true;
			}

			//SPACEで梯子から離れる+ジャンプ
			if (input->TriggerKey(DIK_SPACE)) {
				if (isJump == false) {
					Jump();
					isJump = true;
					rotate.y = 360 * (PI / 180);
				}
				onLadder = false;
			}

			SetJump(false);

		}
		else {

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
				if (input->TriggerKey(DIK_SPACE)) {
					if (isJump == false) {
						Jump();
						isJump = true;
					}
				}
			}
		}
	}

	//リセット
	inSwamp = false;
	colLadder = false;

	//重力
	if (!onLadder) {
		position.y += gravity;
		gravity -= gravitySpeed;
	}

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
		else if (action == CLIMB) {

			playerObject->SetModel(playerClimbModel);
			playerObject->PlayAnimation();

		}
	}

	oldAction = action;

	if (stopAnimation != oldStopAnimation) {

		if (stopAnimation) {
			playerObject->StopAnimation();
		}
		else {
			playerObject->RestartAnimation();
		}

	}

	oldStopAnimation = stopAnimation;

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

void Player::Reset(XMFLOAT2 savePos)
{
	inSwamp = false;
	onLadder = false;
	isClear = false;
	colLadder = false;
	isDeath = false;

	position = { savePos.x,savePos.y,-1.0f };

	playerObject->SetModel(playerWaitModel);
	playerObject->PlayAnimation();
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

void Player::Ladder()
{
	colLadder = true;
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
			else if (input->PushKey(DIK_D)) {
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
			else if (input->PushKey(DIK_D)) {
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

bool Player::pushBlock(CubeObject3D* cubeObject)
{
	//落下中(ジャンプも含む)
	if (gravity < 0.0f) {
		newposition = position;
		newposition.y += gravity;
		cubeObject_->SetPosition(newposition);
		if (cubeObject_->CheakCollision(cubeObject)) {
			SetJump(false);
			gravity = 0.0f;
			return false;
		}
	}

	//横の判定
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
		//沼の中
		if (inSwamp) {
			if (input->PushKey(DIK_A)) {
				newposition = position;
				newposition.x = (position.x - swampSpeed);
				cubeObject_->SetPosition(newposition);
				if (cubeObject_->CheakCollision(cubeObject)) {
					return true;
				}
			}
			else if (input->PushKey(DIK_D)) {
				newposition = position;
				newposition.x = (position.x + swampSpeed);
				cubeObject_->SetPosition(newposition);
				if (cubeObject_->CheakCollision(cubeObject)) {
					return true;
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
					return true;
				}
			}
			else if (input->PushKey(DIK_D)) {
				newposition = position;
				newposition.x = (position.x + speed);
				cubeObject_->SetPosition(newposition);
				if (cubeObject_->CheakCollision(cubeObject)) {
					return true;
				}
			}

		}
	}
}
