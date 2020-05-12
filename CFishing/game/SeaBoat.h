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

	void move(iPoint movement);

};
#define MapCharWidth 30
#define MapCharHeight 30

// 한점에서 직선까지의 거리
float getDistanceLine0(iPoint p, iPoint sp, iPoint ep);
// 한점에서 선분까지의 거리
float getDistanceLine1(iPoint p, iPoint sp, iPoint ep);

/////////////////////////////////////////////////////////////////
//PopFishing
/////////////////////////////////////////////////////////////////

void createPopFishing();
void freePopFishing();
void showPopFishing(bool show);
void drawPopFishing(float dt);
bool keyPopFishing(iKeyState stat, iPoint point);

