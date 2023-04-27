#include "Autosave.h"
#include "imgui.h"

void Autosave::Initialize(Player* player)
{
	//プレイヤー
	this->player = player;

	//セーブ地点
	savePos[0] = XMFLOAT2(0, 1);

	//stage1
	savePos[1] = XMFLOAT2(27, 1);
	savePos[2] = XMFLOAT2(37, 1);
	savePos[3] = XMFLOAT2(60, 1);
	savePos[4] = XMFLOAT2(115, 1);

	//stage2
	savePos[5] = XMFLOAT2(160, 1);
	savePos[6] = XMFLOAT2(177, 1);
	savePos[7] = XMFLOAT2(192, 1);
	savePos[8] = XMFLOAT2(245.5, 1);

	//stage3
	savePos[9] = XMFLOAT2(285.5, 1);
	savePos[10] = XMFLOAT2(300, 1);
	savePos[11] = XMFLOAT2(320, 1);
	savePos[12] = XMFLOAT2(340, 1);
	savePos[13] = XMFLOAT2(370, 1);

	//stage4
	savePos[14] = XMFLOAT2(400, 1);
	savePos[15] = XMFLOAT2(435, 1);
	savePos[16] = XMFLOAT2(470, 1);

	savePos[17] = XMFLOAT2(490, 1);
	savePos[18] = XMFLOAT2(511, 1);
	savePos[19] = XMFLOAT2(470, 1);
	savePos[20] = XMFLOAT2(532.5f, 1);
	savePos[21] = XMFLOAT2(551, 1);

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

		sPosY1[i] = savePos[i].y - 100;
		sPosY2[i] = savePos[i].y + 100;

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
