#include "Enemy.h"

void Enemy::Initialize(FbxModel* greenEnemyModel, FbxModel* redEnemyModel, FbxModel* enemyEyeModel, Player* player)
{
	//プレイヤー
	this->player = player;

	enemyObject = new FbxObject3D;
	enemyObject->Initialize();
	enemyObject->SetModel(greenEnemyModel);

	enemyModel = redEnemyModel;
	enemyModel2 = greenEnemyModel;

	enemyEyeObject = new FbxObject3D;
	enemyEyeObject->Initialize();
	enemyEyeObject->SetModel(enemyEyeModel);
	enemyEyeObject->SetAlpha(0.8f);

	target.x = position.x;
	target.y = position.y;
	target.z = position.z;
	target.z = target.z - 20;

	enemyangle = XMConvertToRadians(30.0f);

}

void Enemy::Update()
{

	enemyObject->SetPosition(position);
	enemyObject->SetScale(scale);
	enemyObject->SetRotation(rotate);


	//判定用
	vec.x = position.x;
	vec.y = position.y;
	vec.z = position.z;

	vec.x -= target.x;
	vec.y -= target.y;
	vec.z -= target.z;

	vec.normalize();

	time++;
	if (time >= maxTime) {
		if (isback == true) {
			isback = false;
		}
		else {
			enemyangle = XMConvertToRadians(30.0f);
			isback = true;
		}
		time = 0;
	}

	//判定用
	enemytargetvec.x = (position.x - player->GetPosition().x);
	enemytargetvec.y = (position.y - player->GetPosition().y);
	enemytargetvec.z = (position.z - player->GetPosition().z);
	enemytargetvec.normalize();
	enemydot = vec.dot(enemytargetvec);
	enemydeg = acos(enemydot) * (PI / 180);

	if (isback == false) {
		if (enemyangle >= XMConvertToRadians(-30.0f)) {
			//回転
			enemyangle -= XMConvertToRadians(0.5f);
		}
		//赤
		SetModel(enemyModel);
		//当たっている
		if (enemydeg * 1000 <= 6) {
			player->Death();
		}
	}
	else {
		//緑
		SetModel(enemyModel2);
		if (maxTime - 30 <= time) {
			if (time % 2 == 0) {
				//緑
				SetModel(enemyModel2);
			}
			else if (time % 2 == 1) {
				//赤
				SetModel(enemyModel);
			}
		}
	}
	//angleラジアンだけY軸まわりに回転。半径は-100
	target.x = position.x - (distance * sinf(enemyangle));
	target.z = position.z - (distance * cosf(enemyangle));

	Setrotate(XMFLOAT3{ 0,enemyangle,0 });

	enemyObject->SetPosition(position);
	enemyObject->SetScale(scale);
	enemyObject->SetRotation(rotate);

	enemyObject->Update();

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

void Enemy::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	enemyObject->DrawLightView(cmdList);
	/*enemyEyeObject->DrawLightView(cmdList);*/
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

void Enemy::SetSRV(ID3D12DescriptorHeap* SRV)
{
	enemyObject->SetSRV(SRV);
	enemyEyeObject->SetSRV(SRV);
}
