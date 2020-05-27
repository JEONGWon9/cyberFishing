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

#define fishingtileNumX 10
#define fishingtileNumY 13
#define fishingtileW 10
#define fishingtileH 13


struct Fish
{
	Texture* tex;
	iSize size;//크기
	iPoint position;

	float hp,_hp;
	float speed;

	//char name;//이름

	void move(iPoint mp);
	
};

struct Rod
{
	Rod();
	~Rod();

	void paint(float dt, iPoint off);

	iImage* rod;
	uint8 catchfish;//0:낚시중  1: 잡았을때

	float atk;
	float rodMin, rodMax;
	float rodDt;
	float rodDegree;
	bool roding;
};
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