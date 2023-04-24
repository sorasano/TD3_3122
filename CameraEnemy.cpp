#include "CameraEnemy.h"

void CameraEnemy::Initialize(FbxModel* EnemyModel, FbxModel* enemyEyeModel, Player* player)
{
	//プレイヤー
	this->player = player;


	enemyObject = new FbxObject3D;
	enemyObject->Initialize();
	enemyObject->SetModel(EnemyModel);

	enemyEyeObject = new FbxObject3D;
	enemyEyeObject->Initialize();
	enemyEyeObject->SetModel(enemyEyeModel);
}

void CameraEnemy::Update()
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
		enemyangle -= XMConvertToRadians(0.25f);
		//当たっている
		if (enemydeg * 1000 <= 3) {
			if (player->GetPosition().x + 9.0f >= position.x) {
				player->Death();
			}
		}
	}
	else {
		//回転
		enemyangle += XMConvertToRadians(0.25f);
		//当たっている
		if (enemydeg * 1000 <= 3) {
			if (player->GetPosition().x + 9.0f >= position.x) {
				player->Death();
			}
		}
	}


	//angleラジアンだけY軸まわりに回転。半径は-100
	target.x = position.x - (distance * cosf(enemyangle));
	target.y = position.y + (distance * sinf(enemyangle));

	Setrotate(XMFLOAT3{ enemyangle,XMConvertToRadians(90.0f),0 });

	enemyObject->SetPosition(position);
	enemyObject->SetScale(scale);
	enemyObject->SetRotation(rotate);

	enemyObject->Update();

	enemyEyeObject->SetPosition(XMFLOAT3(position.x, position.y + 0.5f, position.z));
	enemyEyeObject->SetScale(XMFLOAT3(1, 1, 1));
	enemyEyeObject->SetRotation(rotate);
	enemyEyeObject->Update();

}

void CameraEnemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	enemyObject->Draw(cmdList);
	enemyEyeObject->Draw(cmdList);
}

void CameraEnemy::SetPosition(XMFLOAT3 position)
{
	this->position = position;
}

void CameraEnemy::SetScale(XMFLOAT3 scale)
{
	this->scale = scale;
}

void CameraEnemy::Setrotate(XMFLOAT3 rotate)
{
	this->rotate = rotate;
}

void CameraEnemy::SetModel(FbxModel* enemyModel)
{
	enemyObject->SetModel(enemyModel);
}
