#pragma once
#include "Player.h"
#include "CSVLoader.h"

class Autosave
{

public:

	void Initialize(Player* player);

	void Update();
	void UpdateCSV();

	XMFLOAT2 GetSavePos() { return XMFLOAT2{ savePos[nowSavePos].x, savePos[nowSavePos].y }; }
	XMFLOAT2 GetStartPos() { return XMFLOAT2{ savePos[0].x, savePos[0].y }; }

private:
	
	CSVLoader* autoSaveCsv = nullptr;

	//セーブ地点個数
	static const int saveSize = 30;
	XMFLOAT3 savePos[100];

	//今どのセーブ地点か
	int nowSavePos = 0;

	Player *player;

};

