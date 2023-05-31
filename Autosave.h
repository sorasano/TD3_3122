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

	//�Z�[�u�n�_��
	static const int saveSize = 30;
	XMFLOAT3 savePos[100];

	//���ǂ̃Z�[�u�n�_��
	int nowSavePos = 0;

	Player *player;

};

