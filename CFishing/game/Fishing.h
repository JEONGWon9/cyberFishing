#pragma once

#include "iStd.h"

void loadFishing();
void freeFishing();
void drawFishing(float dt);
void keyFishing(iKeyState stat, iPoint point);

struct Maptile_F
{
	uint8 attr;
};

#define fishingtileNumX 20
#define fishingtileNumY 17
#define fishingtileW 20
#define fishingtileH 17


#define _fishDt 8.0f

struct Fish
{
	Fish();
	~Fish();
	void move(iPoint mp);
	void paint(float dt, iPoint off);


	Texture* tex;
	iSize size;//크기
	iPoint position;
	iImage* fishbubble;

	float hp, _hp;
	float speed;
	float fishDt;

	//char name;//이름


};

struct Rod
{

	float rodDegree;
	bool roding;

	iImage* rodimg;
	float atk;
	float rodMin, rodMax;// min ->낚음 max< = 놓침 
	float rodDt;
	uint8 catchfish;//0:낚시중  1: 잡았을때

};

///////////////////////////////////////////////////////////
///물고기낚음팝업.
///////////////////////////////////////////////////////////


void createPopFish();
void freePopFish();
void showPopFish(bool show);
void drawPopFish(float dt);
bool keyPopFish(iKeyState stat, iPoint point);



void createPopTension();
void freePopTension();
void showPopTension(bool show);
void drawPopTension(float dt);
bool keyPopTension(iKeyState stat, iPoint point);


/*
물고기생성 -랜덤하게 (6,rand%11+1(1~11));

낚시대 height점 =  낚시줄 sp위치
물고기생성위치 = 낚시줄의 ep위치  rodmax넘지않는선. (ocean 타일)

물고기 좌우이동/

마우스 클릭시
=물고기 hp감소 fishposition -->catchPoint 로 다가옴 (화면 살짝 줌/ 흔들림)
fishPosition == catchPoint  =  잡았다 팝업 뜸

마우스 클릭 해제
= fishposition -->catchPoint에서 멀어짐
hp조금씩 회복 (?)
rodMax  보다 길어지면 물고기 잡기실패
->seaboat로 돌아감.

*/