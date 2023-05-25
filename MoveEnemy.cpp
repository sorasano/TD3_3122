#include "MoveEnemy.h"
#include "FbxLoader.h"

void MoveEnemy::Initialize(Player* player)
{

	this->player = player;

	moveEnemyObject = new FbxObject3D;
	moveEnemyObject->Initialize();

	enemyWaitModel = FbxLoader::GetInstance()->LoadModelFromFile("humanWait", "Resources/color/red1x1.png");
	enemyWalkModel = FbxLoader::GetInstance()->LoadModelFromFile("humanWalk", "Resources/color/red1x1.png");
	moveEnemyObject->SetModel(enemyWaitModel);
	moveEnemyObject->PlayAnimation();

	//ラジアン
	rotate.y = 270 * (PI / 180);

	initPosition = position;
	initRotate = rotate;

}

void MoveEnemy::Update()
{

	action = WAIT;

	Colision();

	DiscoverColision();

	Move();

	//モデルの変更

	if (action != oldAction) {

		if (action == WAIT) {
			moveEnemyObject->SetModel(enemyWaitModel);
			moveEnemyObject->PlayAnimation();
		}
		else if (action == WALK) {
			moveEnemyObject->SetModel(enemyWalkModel);
			moveEnemyObject->PlayAnimation();
		}
	}

	oldAction = action;

	moveEnemyObject->SetPosition(position);
	moveEnemyObject->SetScale(scale);
	moveEnemyObject->SetRotation(rotate);
	moveEnemyObject->Update();

}

void MoveEnemy::Move()
{

	if (isDiscover) {
		
		action = WALK;

		//プレイヤーの座標より右の時
		if (player->GetPosition().x < position.x - scale.x * 100 / 2) {
			position.x -= speed;
			rotate.y = 270 * (PI / 180);
		}
		//左の時
		else if (player->GetPosition().x > position.x + scale.x * 100 / 2) {
			position.x += speed;
			rotate.y = 90 * (PI / 180);
		}
		//座標が重なったら
		else {
			action = WAIT;
		}

	}

}

void MoveEnemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	moveEnemyObject->Draw(cmdList);
}

void MoveEnemy::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	moveEnemyObject->DrawLightView(cmdList);
}

void MoveEnemy::Colision()
{
	float scaleX = scale.x * 100 / 2;
	float scaleY = scale.y * 100 / 2;
	float scaleZ = scale.z * 100 / 2;

	float bPosX1 = position.x - scaleX;
	float bPosX2 = position.x + scaleX;

	float bPosY1 = position.y - scaleY;
	float bPosY2 = position.y + scaleY;

	float bPosZ1 = position.z - scaleZ;
	float bPosZ2 = position.z + scaleZ;

	float pScaleX = 0.5f;
	float pScaleY = 0.5f;
	float pScaleZ = 0.5f;

	float pPosX1 = player->GetPosition().x - pScaleX;
	float pPosX2 = player->GetPosition().x + pScaleX;

	float pPosY1 = player->GetPosition().y - pScaleY;
	float pPosY2 = player->GetPosition().y + pScaleY;

	float pPosZ1 = player->GetPosition().z - pScaleZ;
	float pPosZ2 = player->GetPosition().z + pScaleZ;

	//当たったら
	if (pPosX1 < bPosX2 && bPosX1 < pPosX2) {

		if (pPosY1 < bPosY2 && bPosY1 < pPosY2) {

			if (pPosZ1 < bPosZ2 && bPosZ1 < pPosZ2) {

				player->Death();

			}
		}
	}
}

void MoveEnemy::DiscoverColision()
{
	float scaleX = discoverSize.x;
	float scaleY = discoverSize.x;
	float scaleZ = discoverSize.x;

	float bPosX1 = position.x - scaleX;
	float bPosX2 = position.x + scaleX;

	float bPosY1 = position.y - scaleY;
	float bPosY2 = position.y + scaleY;

	float bPosZ1 = position.z - scaleZ;
	float bPosZ2 = position.z + scaleZ;

	float pScaleX = 0.5f;
	float pScaleY = 0.5f;
	float pScaleZ = 0.5f;

	float pPosX1 = player->GetPosition().x - pScaleX;
	float pPosX2 = player->GetPosition().x + pScaleX;

	float pPosY1 = player->GetPosition().y - pScaleY;
	float pPosY2 = player->GetPosition().y + pScaleY;

	float pPosZ1 = player->GetPosition().z - pScaleZ;
	float pPosZ2 = player->GetPosition().z + pScaleZ;

	//見つかったフラグリセット
	isDiscover = false;

	//当たったら
	if (pPosX1 < bPosX2 && bPosX1 < pPosX2) {

		if (pPosY1 < bPosY2 && bPosY1 < pPosY2) {

			if (pPosZ1 < bPosZ2 && bPosZ1 < pPosZ2) {

				//見つかったフラグtrue
				isDiscover = true;

			}
		}
	}
}

void MoveEnemy::Reset()
{
	this->position = initPosition;
	this->rotate = initRotate;
}

void MoveEnemy::SetSRV(ID3D12DescriptorHeap* SRV)
{
	moveEnemyObject->SetSRV(SRV);
}



