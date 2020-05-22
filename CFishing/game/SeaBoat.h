#pragma once

#include "iStd.h"



void loadSeaBoat();
void freeSeaBoat();
void drawSeaBoat(float dt);
void keySeaBoat(iKeyState stat, iPoint point);

struct MapTile
{
	uint8 attr;
};

#define SeaTileX 10
#define SeaTileY 13
#define SeaTileWidth 30
#define SeaTileHeight 30


#define tile_color_canMove		0, 0.8f, 1, 1
#define tile_color_canNotMove	0.8f, 0.5f, 0.3f, 1
#define tile_color_end			1, 1, 0, 1

#define natureSpeed 100

enum BoatBehave
{
	BoatBehave_right = 0,
	BoatBehave_left,
	BoatBehave_up,
	BoatBehave_down,
	
	BoatBehave_right_up,
	BoatBehave_left_up,
	BoatBehave_right_down,
	BoatBehave_left_down,

	BoatBehave_num,
};

struct Boat
{
	Boat();
	~Boat();

	void setBehave(BoatBehave be);
	void setBehave(iPoint v);
	void paint(float dt, iPoint off,iPoint v);
	void move(iPoint movement);

	BoatBehave behave;
	iImage** imgs;
	iImage* img;
	iSize size;// for collision
	iPoint position;
	float speed;

	uint8 throwing;// 0:찾기 1:던질준비, 2: 던짐& 3: 낚시준비 
	float throwResult, throwMin, throwMax;
	float throwAniDt;
};



#define MapCharWidth 20
#define MapCharHeight 20

// 한점에서 직선까지의 거리
float getDistanceLine0(iPoint p, iPoint sp, iPoint ep);
// 한점에서 선분까지의 거리
float getDistanceLine1(iPoint p, iPoint sp, iPoint ep);

/////////////////////////////////////////////////////////////////
//PopBackBtn
/////////////////////////////////////////////////////////////////

void createPopBackBtn();
void freePopBackBtn();
void showPopBackBtn(bool show);
void drawPopBackBtn(float dt);
bool keyPopBackBtn(iKeyState stat, iPoint point);




/////////////////////////////////////////////////////////////////
//PopFishingGage
/////////////////////////////////////////////////////////////////

void createPopFishingGage();
void freePopFishingGage();
void showPopFishingGage(bool show);
void drawPopFishingGage(float dt);
bool keyPopFishingGage(iKeyState stat, iPoint point);

#define _gageDt 1.0f
#define _throwAniDt 2.0f
#define _biteDt 3.0f
#define _floatDt 2.0f
/////////////////////////////////////////////////////////////////
//PopFishingfloat
/////////////////////////////////////////////////////////////////

void createFishingfloat();
void freeFishingfloat();
void showFishingfloat(bool show);
void drawFishingfloat(float dt);
bool keyFishingfloat(iKeyState stat, iPoint point);


/*



높이값 비율로 보트 원점에서 선이 돌면서 바라보는 방향으로 날라감(방향조절)
선이 멈추면 그자리에작은 네모(찌)

->3초 뒤 네모(찌) 사라짐 ->클릭시 ->화면전환
						->타이밍에클릭 못하면 화면전환안하고 기본으로 



낚시줄 던짐 (보트상태에서)

-> new 화면 -> 낚시대 쥐고 마우스클릭(게이지)->찌 팝업->타이밍 따라 클릭 ->hit!->
화면전환 ->클릭 누르고있음으로 낚시 진행
->조건만족(물고기hp  0 / 거리 0) -> 팝업 ->물괴기를 낚음


게이지량만큼 *바라보는방향 * (최소길이 +(최대길이-최소길이))

*/



