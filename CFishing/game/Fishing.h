#pragma once

#include "iStd.h"

void loadFishing();
void freeFishing();
void drawFishing(float dt);
void keyFishing(iKeyState stat, iPoint point);


struct Fish
{
	Texture* tex;
	iSize size;//크기
	iPoint position;

	float hp,_hp;
	float speed;

	//char name;//이름

	
};

struct rod
{
	iImage* rod;
	float atk;
	float rodMin, rodMax;
	float rodDt;

};