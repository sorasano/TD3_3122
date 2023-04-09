#include "Enemy.h"

void Enemy::Initialize(FbxModel* EnemyModel)
{

	enemyObject = new FbxObject3D;
	enemyObject->Initialize();

	enemyObject->SetModel(EnemyModel);

}

void Enemy::Update()
{
	enemyObject->SetPosition(position);
	enemyObject->SetScale(scale);
	enemyObject->SetRotation(rotate);
	//”»’è—p
	target.x = position.x;
	target.y = position.y;
	target.z = position.z;
	target.z = target.z - 20;

	vec.x = position.x;
	vec.y = position.y;
	vec.z = position.z;

	vec.x -= target.x;
	vec.y -= target.y;
	vec.z -= target.z;

	vec.normalize();

	enemyObject->Update();

}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	enemyObject->Draw(cmdList);
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
