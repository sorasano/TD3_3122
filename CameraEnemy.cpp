#include "CameraEnemy.h"

void CameraEnemy::Initialize(FbxModel* EnemyModel, FbxModel* enemyEyeModel, FbxModel* enemyRotModel, Player* player)
{
	//プレイヤー
	this->player = player;


	enemyObject = new FbxObject3D;
	enemyObject->Initialize();
	enemyObject->SetModel(EnemyModel);

	enemyEyeObject = new FbxObject3D;
	enemyEyeObject->Initialize();
	enemyEyeObject->SetModel(enemyEyeModel);

	enemyRotObject = new FbxObject3D;
	enemyRotObject->Initialize();
	enemyRotObject->SetModel(enemyRotModel);
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

	objrotate = { 0,0,-enemyangle };
	Setrotate(XMFLOAT3{ enemyangle,XMConvertToRadians(90.0f),0 });
	/*objrotate = rotate;
	objrotate.x += XMConvertToRadians(-90.0f);*/
	enemyObject->SetPosition(position);
	enemyObject->SetScale(scale);
	enemyObject->SetRotation(objrotate);

	enemyObject->Update();

	enemyEyeObject->SetPosition(XMFLOAT3(position.x, position.y, position.z));
	enemyEyeObject->SetScale(XMFLOAT3(1, 1, 1));
	enemyEyeObject->SetRotation(rotate);
	enemyEyeObject->Update();


	rotPosition = position;
	rotPosition.y += 0.3f;
	rotPosition.x += 0.5;
	enemyRotObject->SetPosition(rotPosition);
	enemyRotObject->SetScale(rotScale);
	enemyRotObject->SetRotation(rotRotate);
	enemyRotObject->Update();
}

void CameraEnemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	enemyObject->Draw(cmdList);
	enemyEyeObject->Draw(cmdList);
	enemyRotObject->Draw(cmdList);
}

void CameraEnemy::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	/*enemyObject->DrawLightView(cmdList);
	enemyRotObject->DrawLightView(cmdList);*/
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

void CameraEnemy::SetSRV(ID3D12DescriptorHeap* SRV)
{
	enemyObject->SetSRV(SRV);
	enemyEyeObject->SetSRV(SRV);
	enemyRotObject->SetSRV(SRV);
}