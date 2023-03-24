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
