#include "SelectMap.h"
#include "Menu.h"
#include "Loading.h"
#include "SeaBoat.h"

Texture* texmapBg;

void loadSelectMap()
{
	texmapBg = createImage("assets/map/koreaMap.jpg");


	createPopPoint();
	showPopPoint(true);
}

void freeSelectMap()
{
	freeImage(texmapBg);


	freePopPoint();

}

void drawSelectMap(float dt)
{
	//texmapBg = 627x650


	drawImage(texmapBg, devSize.width / 2, devSize.height / 2, 0+100, 0+100,
		texmapBg->width-200, texmapBg->height-200, VCENTER | HCENTER,0.7f,0.7f,2, 0, REVERSE_NONE);


	drawPopPoint(dt);

}


void keySelectMap(iKeyState stat, iPoint point)
{
	if (keyPopPoint(stat, point))
		return;

}


iPopup* fishpoint;
iImage** fp;
void createPopPoint()
{
	iPopup* pop = new iPopup(iPopupStyleZoom);
	iPoint r = iPointMake((devSize.width / 2) - (texmapBg->width / 2), (devSize.height / 2) - (texmapBg->height / 2));
	
	fp = (iImage**)malloc(sizeof(iImage*) * 3);
	for (int i = 0; i < 3; i++)
	{
		iImage* fishingImg = new iImage();
		for (int j = 0; j < 2; j++)
		{
			Texture* tex = createImage("assets/map/fishPoint%d.png",j);
			fishingImg->addObject(tex);
			freeImage(tex);
		}

		fishingImg->position = iPointMake(120, 50 * i);
		pop->addObject(fishingImg);
		fp[i] = fishingImg;

	}

	

	pop->openPosition = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePosition = iPointMake(devSize.width / 2, devSize.height / 2);

	fishpoint = pop;
}

void freePopPoint()
{

	delete fishpoint;
}

void showPopPoint(bool show)
{
	fishpoint->show(show);
}

void drawPopPoint(float dt)
{
	for (int i = 0; i < 3; i++)
		fp[i]->setTexAtIndex(i == fishpoint->selected);
	fishpoint->paint(dt);
	
}

bool keyPopPoint(iKeyState stat, iPoint point)
{

	if (fishpoint->bShow == false)
		return false;
	if (fishpoint->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStateBegan:
		i = fishpoint->selected;

		if (i == -1) break;
		if (i < 3)
		{
			METHOD_LOADING f[3] = { loadSeaBoat,loadSeaBoat,loadSeaBoat };
			setLoading(gs_seaboat, freeSelectMap, f[i]);
		}				
		break;

	case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(point, fp[i]->touchRect(fishpoint->closePosition)))
			{
				j = i;// audio//?
				break;
			}
		}
		fishpoint->selected = j;
		break;
	case iKeyStateEnded:

		break;

	}
	return false;
}
