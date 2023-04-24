#include "Autosave.h"
#include "imgui.h"

void Autosave::Initialize(Player* player)
{
	//プレイヤー
	this->player = player;

	//セーブ地点
	savePos[0] = XMFLOAT2(0, 1);
	savePos[1] = XMFLOAT2(27, 1);
	savePos[2] = XMFLOAT2(42, 1);
	savePos[3] = XMFLOAT2(95.5, 1);

}

void Autosave::Update()
{

	float scale = 0.5;

	float pPosX1 = player->GetPosition().x - scale;
	float pPosX2 = player->GetPosition().x + scale;

	float pPosY1 = player->GetPosition().y - scale;
	float pPosY2 = player->GetPosition().y + scale;

	float sPosX1[saveSize];
	float sPosX2[saveSize];

	float sPosY1[saveSize];
	float sPosY2[saveSize];

	for (int i = 0; i < saveSize; i++) {

		sPosX1[i] = savePos[i].x - scale;
		sPosX2[i] = savePos[i].x + scale;

		sPosY1[i] = savePos[i].y - scale;
		sPosY2[i] = savePos[i].y + scale;

		if (pPosX1 < sPosX2[i] && sPosX1[i] < pPosX2) {

			if (pPosY1 < sPosY2[i] && sPosY1[i] < pPosY2) {

				//セーブ地点を更新
				nowSavePos = i;

			}
		}
	}

	//ImGui::Begin("nowSavePos");
	//ImGui::Text("nowSavePos = %d", nowSavePos);
	//ImGui::End();
}
