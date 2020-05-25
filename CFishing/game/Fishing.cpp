#include "Fishing.h"
#include "Menu.h"
#include "Loading.h"
#include "SeaBoat.h"
#include "Game.h"

Texture* fishingBg;
Texture* fishingrod;
float rodDegree;
bool roding;
void loadFishing()
{
	fishingBg = createImage("assets/map/fishingmap.png");
	fishingrod = createImage("assets/fish/fishingrod.png");
	rodDegree = 0;
	roding = false;

}
void freeFishing()
{
	freeImage(fishingBg);
	freeImage(fishingrod);
}

void drawFishing(float dt)
{
	float r0 = devSize.width / fishingBg->width;
	float r1 = devSize.height / fishingBg->height;
	float r = max(r0, r1);
	setRGBA(1, 1, 1, 1);
	drawImage(fishingBg, devSize.width / 2, devSize.height / 2, 0, 0, fishingBg->width, fishingBg->height, VCENTER | HCENTER,
		r,0.7,0, 0, REVERSE_NONE);

	iPoint hand = iPointMake(devSize.width / 2, devSize.height-50);
	iPoint c = hand - iPointMake(0, fishingrod->height / 2);
	c = iPointRotate(c, hand, rodDegree);
	drawImage(fishingrod, c.x, c.y,
		0, 0, fishingrod->width, fishingrod->height, VCENTER | HCENTER, 1.0f, 1.0f,
		2, rodDegree, REVERSE_NONE);
}

void keyFishing(iKeyState stat, iPoint point)
{
	if (stat == iKeyStateBegan)
	{
		roding = true;
	}
	else if (stat == iKeyStateMoved)
	{
		if (roding)
		{
			rodDegree = 45 * (devSize.width/2 - point.x) / 200;
			if (rodDegree > 45)
				rodDegree = 45;
			else if (rodDegree < -45)
				rodDegree = -45;
		}
	}
	else if (stat == iKeyStateEnded)
	{
		roding = false;
	}
}
