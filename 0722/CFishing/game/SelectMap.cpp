#include "SelectMap.h"
#include "Menu.h"
#include "Loading.h"
#include "SeaBoat.h"

Texture* texmapBg;

void loadSelectMap()
{
	texmapBg = createImage("assets/map/koreaMap.jpg");


	createPopPoint();
	createPopFishingInfo();

	showPopPoint(true);
}

void freeSelectMap()
{
	freeImage(texmapBg);


	freePopPoint();
	freePopFishingInfo();

}

void drawSelectMap(float dt)
{
	//texmapBg = 627x650


	drawImage(texmapBg, devSize.width / 2, devSize.height / 2, 0+100, 0+100,
		texmapBg->width-200, texmapBg->height-200, VCENTER | HCENTER,0.7f,0.7f,2, 0, REVERSE_NONE);


	drawPopPoint(dt);
	drawPopFishingInfo(dt);

}


void keySelectMap(iKeyState stat, iPoint point)
{
	if (keyPopPoint(stat, point))
		return;
	if (keyPopFishingInfo(stat, point))
		return;

}


iPopup* fishpoint;
iImage** fp;
int bShowFishInfo;
void drawPopPointBefore(iPopup* me, iPoint p, float dt);

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

		fishingImg->position = iPointMake(130, 50 * i);
		pop->addObject(fishingImg);
		fp[i] = fishingImg;

	}

	

	pop->openPosition = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePosition = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->methodDrawBefore = drawPopPointBefore;

	fishpoint = pop;

	bShowFishInfo = -1;
}

void freePopPoint()
{

	delete fishpoint;
}

void showPopPoint(bool show)
{
	fishpoint->show(show);
}

void drawPopPointBefore(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < 3; i++)
		fp[i]->setTexAtIndex(i == fishpoint->selected);

	if (bShowFishInfo != -1)
	{
		stScreen->setString("%d", bShowFishInfo);
		showPopFishingInfo(true);
	}
	else
	{
		showPopFishingInfo(false);
	}
}

void drawPopPoint(float dt)
{
	fishpoint->paint(dt);
}

bool keyPopPoint(iKeyState stat, static iPoint point)
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
		fishpoint->selected = -1;
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
		bShowFishInfo = j;
		break;
	case iKeyStateEnded:

		break;

	}
	return false;
}

/////////////////////////////////////////////////////////////////
//PopFishingInfo
/////////////////////////////////////////////////////////////////

iPopup* finfo;

iImage* screen;
iStrTex* stScreen;
Texture* methodStScreen(const char* str);

void createPopFishingInfo()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	finfo = pop;

	stScreen = new iStrTex(methodStScreen);
	stScreen->setString("0");
	screen = new iImage();
	screen->addObject(stScreen->tex);
	pop->addObject(screen);

	pop->openPosition = iPointMake(devSize.width/2+200 , devSize.height/2-200);

	pop->closePosition = iPointMake(devSize.width/2+200, devSize.height/2-200);
}
	
Texture* methodStScreen(const char* str)
{
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(250, 250);
	g->init(size);

	setRGBA(0, 0, 1, 0.8f);
	g->fillRect(0, 0, size.width, size.height, 20);

	int i = atoi(str);

	const char* strPoint[3] = {
		"낚시터 1", "낚시터 2", "낚시터 3"
	};
	const char* strfish[3][3] = {
		{"농어","참치","청새치"},{"농어","은어","상어"},{"농어","볼락","다금바리"}
	};
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, 10, TOP | HCENTER, strPoint[i]);
	for (int j = 0; j < 3; j++)
	{

		setStringSize(20);
		setStringRGBA(0, 1, 1, 1);
		setStringBorder(0);
		g->drawString(size.width / 2, size.height / 2 + 50 * j + 1,
			VCENTER | HCENTER, strfish[i][j]);
	}
	return g->getTexture();
}

void freePopFishingInfo()
{
	delete finfo;
	delete stScreen;
}

void showPopFishingInfo(bool show)
{
	finfo->show(show);
}

void drawPopFishingInfo(float dt)
{
	finfo->paint(dt);
}


bool keyPopFishingInfo(iKeyState stat, iPoint point)
{
	
	return false;
}



