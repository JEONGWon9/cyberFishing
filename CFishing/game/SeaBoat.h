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
#define SeaTileWidth 60
#define SeaTileHeight 60


#define tile_color_canMove		0, 0.8f, 1, 1
#define tile_color_canNotMove	0.8f, 0.5f, 0.3f, 1
#define tile_color_end			1, 1, 0, 1

#define natureSpeed 100

struct Boat
{
	iPoint position;
	iSize size;
	float speed;

	bool keep;
	iPoint positionKeep;
	float keepDt;
	int direction;



	void move(iPoint movement);

};

struct gage
{
	bool gage;
	iPoint gagePoint;
	
	float gageDt;


};

#define MapCharWidth 30
#define MapCharHeight 30

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

/////////////////////////////////////////////////////////////////
//PopFishingfloat
/////////////////////////////////////////////////////////////////

void createPopFishingfloat();
void freePopFishingfloat();
void showPopFishingfloat(bool show);
void drawPopFishingfloat(float dt);
bool keyPopFishingfloat(iKeyState stat, iPoint point);
/*

 낚시줄던지기 // 게이지바 

 drawLine(sp(,ep
*/
