#include "Autosave.h"
#include "imgui.h"

void Autosave::Initialize(Player* player)
{
	//プレイヤー
	this->player = player;

	//csv読み込み
	autoSaveCsv = new CSVLoader;
	autoSaveCsv->LoadCSV("Resources/csv/autoSave.csv");

	//セーブ地点
	for (int i = 0; i < saveSize; i++)
	{
		savePos[i] = autoSaveCsv->GetPosition(i);
	}

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

}

void Autosave::UpdateCSV()
{
	autoSaveCsv->LoadCSV("Resources/csv/autoSave.csv");
	for (int i = 0; i < saveSize; i++)
	{
		savePos[i] = autoSaveCsv->GetPosition(i);
	}

}

