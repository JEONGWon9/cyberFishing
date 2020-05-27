#include "Fishing.h"
#include "Menu.h"
#include "Loading.h"
#include "SeaBoat.h"
#include "Game.h"

Texture* fishingBg;


Maptile_F* fishingmap;
iPoint offFt;
float devfishing_width, devfishing_height;


Rod* rod;
Fish* fish;


uint8 fishingtile[fishingtileNumX * fishingtileNumY] =
{
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,// 1 == sky
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,// 0 == ocean
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,2,2,2,2,2,2,0,0,//2 == catch 포인트
};



void loadFishing()
{
	int i, num = fishingtileNumX * fishingtileNumY;
	fishingmap = (Maptile_F*)malloc(sizeof(Maptile_F) * num);
	for (i = 0; i < num; i++)
	{
		Maptile_F* fm = &fishingmap[i];
		fm->attr = fishingtile[i];
	}
	offFt = iPointZero;


	fishingBg = createImage("assets/map/fishingmap.png");

	rod->rodDegree = 0;
	rod->roding = false;

	fish = new Fish();
	rod = new Rod();

	//fish->position = iPointMake(100,100);
	//fish->speed = 100;


}
void freeFishing()
{
	free(fishingmap);
	freeImage(fishingBg);


	delete fish;
	delete rod;

}

void drawFishing(float dt)
{
	devfishing_width = devSize.width / fishingtileW;
	devfishing_height = devSize.height / fishingtileH;

	///////////////////////////////////////////////////////////
	///배경
	///////////////////////////////////////////////////////////

	float r0 = devSize.width / fishingBg->width;
	float r1 = devSize.height / fishingBg->height;
	float r = max(r0, r1);
	setRGBA(1, 1, 1, 1);
	drawImage(fishingBg, devSize.width / 2, devSize.height / 2, 0, 0, fishingBg->width, fishingBg->height, VCENTER | HCENTER,
		r, 0.7, 0, 0, REVERSE_NONE);
	///////////////////////////////////////////////////////////
	///타일
	///////////////////////////////////////////////////////////
	int i, num = fishingtileNumX * fishingtileNumY;
	for (i = 0; i < num; i++)
	{
		Maptile_F* fm = &fishingmap[i];
		switch (fm->attr) {
		case 0: setRGBA(0, 0.8f, 1, 0.5); break;
		case 1: setRGBA(0, 0, 1, 0.5); break;
		case 2: setRGBA(1, 0, 0, 0.5); break;
		}

		float x = offFt.x + devfishing_width * (i % fishingtileNumX);
		float y = offFt.y + devfishing_height * (i / fishingtileNumX);
		fillRect(x, y, devfishing_width, devfishing_height);
		setRGBA(0, 0, 0, 1);
		drawRect(x, y, devfishing_width, devfishing_height);

	}
	///////////////////////////////////////////////////////////
	///낚시대
	///////////////////////////////////////////////////////////

	rod->paint(dt, offFt);


}

void keyFishing(iKeyState stat, iPoint point)
{
	if (stat == iKeyStateBegan)
	{
		rod->roding = true;
	}
	else if (stat == iKeyStateMoved)
	{
		if (rod->roding)
		{
			rod->rodDegree = 45 * (devSize.width / 2 - point.x) / 200;
			if (rod->rodDegree > 45)
				rod->rodDegree = 45;
			else if (rod->rodDegree < -45)
				rod->rodDegree = -45;
		}
	}
	else if (stat == iKeyStateEnded)
	{
		rod->rodDegree = 0;
		rod->roding = false;
	}


}



Rod::Rod()
{
	iImage* img = new iImage();
	Texture* fishingrod = createImage("assets/fish/fishingrod.png");
	img->addObject(fishingrod);
	freeImage(fishingrod);

	rod = img;

	iPoint hand = iPointMake(devSize.width / 2, devSize.height - 50);
	iPoint c = hand - iPointMake(0, rod->height / 2);
	c = iPointRotate(c, hand, rodDegree);
	setRGBA(1, 1, 1, 1);
	drawImage(fishingrod, c.x, c.y,
		0, 0, rod->width, rod->height, VCENTER | HCENTER, 1.0f, 1.0f,
		2, rodDegree, REVERSE_NONE);

}
Rod::~Rod()
{
	free(rod);

}

void Rod::paint(float dt, iPoint off)
{
	rod->paint(dt, off);

}
