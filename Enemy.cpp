#include "Enemy.h"

void Enemy::Initialize(FbxModel* EnemyModel, FbxModel* enemyEyeModel)
{

	enemyObject = new FbxObject3D;
	enemyObject->Initialize();
	enemyObject->SetModel(EnemyModel);

	enemyEyeObject = new FbxObject3D;
	enemyEyeObject->Initialize();
	enemyEyeObject->SetModel(enemyEyeModel);

	target.x = position.x;
	target.y = position.y;
	target.z = position.z;
	target.z = target.z - 20;
}

void Enemy::Update()
{

	enemyObject->SetPosition(position);
	enemyObject->SetScale(scale);
	enemyObject->SetRotation(rotate);


	//”»’è—p
	vec.x = position.x;
	vec.y = position.y;
	vec.z = position.z;

	vec.x -= target.x;
	vec.y -= target.y;
	vec.z -= target.z;

	vec.normalize();

	enemyObject->Update();

	int size = 6;
	enemyEyeObject->SetPosition(XMFLOAT3(position.x, 1.5, position.z));
	enemyEyeObject->SetScale(XMFLOAT3(1, 1, 1));
	enemyEyeObject->SetRotation(rotate);
	enemyEyeObject->Update();

}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	enemyObject->Draw(cmdList);
	if (isback == false) {
		enemyEyeObject->Draw(cmdList);
	}
}


void Enemy::SetPosition(XMFLOAT3 position)
{
	this->position = position;
}

void Enemy::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
}

void Enemy::Setrotate(XMFLOAT3 rotate)
{
	this->rotate = rotate;
}

void Enemy::SetModel(FbxModel* enemyModel)
{
	enemyObject->SetModel(enemyModel);
}
