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
#define SeaTileWidth 50
#define SeaTileHeight 50


#define tile_color_canMove		0, 0.8f, 1, 1
#define tile_color_canNotMove	0.8f, 0.5f, 0.3f, 1
#define tile_color_end			1, 1, 0, 1

#define natureSpeed 100

struct Boat
{
	iPoint position;
	iSize size;
	float speed;
	Texture* tex;

	bool keep;
	iPoint positionKeep;
	int direction;

	void Vec(iPoint v,float dt);

	void move(iPoint movement);


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

#define _gageDt 1.0f
#define _throwDt 1.0f

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

->3초 뒤 네모(찌) 사라지고 게임시작


*/



