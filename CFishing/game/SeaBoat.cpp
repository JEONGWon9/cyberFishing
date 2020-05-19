#include "SeaBoat.h"

#include "Menu.h"
#include "Loading.h"
#include "SelectMap.h"



uint8 tile[SeaTileX * SeaTileY] = {
	1,1,1,1,1,1,1,1,1,1,
	0,0,0,3,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,2,2,0,0,0,0,
	0,0,0,0,2,2,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,3,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,
};

MapTile* maptile;
Boat* bt;

iPoint offMt;

void loadSeaBoat()
{
	int i, num = SeaTileX * SeaTileY;
	maptile = (MapTile*)malloc(sizeof(MapTile) * num);
	for (i = 0; i < num; i++)
	{
		MapTile* t = &maptile[i];
		t->attr = tile[i];
	}
	offMt = iPointZero;

	bt = (Boat*)malloc(sizeof(Boat));

	bt->position = iPointMake(SeaTileWidth,SeaTileHeight);
	bt->size = iSizeMake(MapCharWidth, MapCharHeight);
	bt->speed = 300;
	
	createPopBackBtn();
	createPopFishingGage();
	

}

void freeSeaBoat()
{
	free(maptile);
	free(bt);

	freePopBackBtn();
	freePopFishingGage();
}

void drawSeaBoat(float dt)
{

	int i, num = SeaTileX * SeaTileY;
	for (i = 0; i < num; i++)
	{
		MapTile* t = &maptile[i];
		switch (t->attr) {
		case 0: setRGBA(tile_color_canMove); break;
		case 1: setRGBA(tile_color_canNotMove); break;
		case 2: setRGBA(tile_color_end); break;
		case 3: setRGBA(0, 0, 0, 1); break;
		}
		float x = offMt.x + SeaTileWidth * (i %  SeaTileX);
		float y = offMt.y + SeaTileHeight * (i / SeaTileX);
		fillRect(x, y, SeaTileWidth, SeaTileHeight);
		setRGBA(0, 0, 0, 1);
		drawRect(x + 2, y + 2, SeaTileWidth - 4, SeaTileHeight - 4);
	}

	//boat//
	setRGBA(1, 0, 0, 1);
	fillRect(bt->position.x + offMt.x - bt->size.width / 2,
		bt->position.y + offMt.y - bt->size.height / 2,
		bt->size.width, bt->size.height);

	/////////////////////////////////////////////////////////////////////////////

	//iPoint movement = iPointMake(0, 0) * dt;

	if (getKeyDown() & keyboard_space)
	{
		showPopFishingGage(true);

	}
#if 1
	uint32 keyStat = getKeyStat();

	iPoint movement = iPointMake(0, 0) * dt;
	iPoint v = iPointZero;
	if (keyStat & keyboard_left) v.x = -1;
	else if (keyStat & keyboard_right) v.x = 1;
	if (keyStat & keyboard_up) v.y = -1;
	else if (keyStat & keyboard_down) v.y = 1;
	if (v != iPointZero)
	{
		v /= iPointLength(v);
		 iPoint mp = v * (bt->speed * dt);
		bt->move(mp + movement);

		iPoint vp = offMt + bt->position;
		if (vp.x < devSize.width * 0.333333f)
		{
			// 왼쪽으로 넘어갔을 경우
			offMt.x += (devSize.width * 0.333333f - vp.x);
			if (offMt.x > 0)
				offMt.x = 0;
		}
		else if (vp.x > devSize.width * 0.666667f)
		{
			// 오른쪽으로 넘어갔을 경우
			offMt.x += (devSize.width * 0.666667f - vp.x);
			if (offMt.x < devSize.width - SeaTileWidth * SeaTileX)
			offMt.x = devSize.width - SeaTileWidth * SeaTileX;
		}
		if (vp.y < devSize.height * 0.333333f)
		{
			// 위로 넘어갔을 경우
			offMt.y += (devSize.height * 0.333333f - vp.y);
			if (offMt.y > 0)
				offMt.y = 0;
		}
		else if (vp.y > devSize.height * 0.666667f)
		{
			// 아래로 넘어갔을 경우
			offMt.y += (devSize.height * 0.666667f - vp.y);
			if (offMt.y < devSize.height - SeaTileHeight * SeaTileY)
				offMt.y = devSize.height - SeaTileHeight * SeaTileY;
		}
	}
#else

#endif

	drawPopBackBtn(dt);
	drawPopFishingGage(dt);
}


void keySeaBoat(iKeyState stat, iPoint point)
{

	keyPopBackBtn(stat, point);
	keyPopFishingGage(stat, point);
}

#if 0
void Boat::Vec(iPoint v, float dt)
{
	iPoint movement = iPointMake(0, 0) * dt;

	v = iPointZero;

	uint32 keyStat = getKeyStat();
	if (keyStat & keyboard_left)
	{
		v.x = -1;
	}
	else if (keyStat & keyboard_right)
	{
		v.x = 1;
	}
	if (keyStat & keyboard_up)
	{
		v.y = -1;
	}

	else if (keyStat & keyboard_down)
	{
		v.y = 1;
	}
	///////
	//camera
	///////
	if (v != iPointZero)
	{
		v /= iPointLength(v);
		iPoint mp = v * (bt->speed * dt);
		bt->move(mp + movement);

		iPoint vp = offMt + bt->position;
		if (vp.x < devSize.width * 0.3333f)
		{
			// 왼쪽으로 넘어갔을 경우
			offMt.x += (devSize.width * 0.333333f - vp.x);
			if (offMt.x > 0)
				offMt.x = 0;
		}
		else if (vp.x > devSize.width * 0.666667f)
		{
			// 오른쪽으로 넘어갔을 경우
			offMt.x += (devSize.width * 0.666667f - vp.x);
			if (offMt.x < devSize.width - SeaTileWidth * SeaTileX)
				offMt.x = devSize.width - SeaTileWidth * SeaTileX;
		}
		if (vp.y < devSize.height * 0.333333f)
		{
			// 위로 넘어갔을 경우
			offMt.y += (devSize.height * 0.333333f - vp.y);
			if (offMt.y > 0)
				offMt.y = 0;
		}
		else if (vp.y > devSize.height * 0.666667f)
		{
			// 아래로 넘어갔을 경우
			offMt.y += (devSize.height * 0.666667f - vp.y);
			if (offMt.y < devSize.height - SeaTileHeight * SeaTileY)
				offMt.y = devSize.height - SeaTileHeight * SeaTileY;
		}

	}
}
#else
#endif


void Boat::move(iPoint mp)
{
	if (mp.x < 0)
	{
		// left
		int tlx = position.x - size.width / 2; tlx /= SeaTileWidth;
		int tly = position.y - size.height / 2; tly /= SeaTileHeight;
		int bly = position.y + size.height / 2; bly /= SeaTileHeight;
		int min = 0;
		for (int x = tlx - 1; x > -1; x--)
		{
			bool col = false;
			for (int y = tly; y < bly + 1; y++)
			{
				if (maptile[SeaTileX * y + x].attr == 1)
				{
					col = true;
					min = SeaTileWidth * (x + 1);
					break;
				}
				if (maptile[SeaTileX * y + x].attr == 3)
				{
					col = true;
					min = SeaTileWidth * (x + 3);

					showPopBackBtn(true);
					break;
				}
				else
					showPopBackBtn(false);	break;
			
			}
			if (col)
				break;
		}
		//printf("min = %d, curr = %f\n", min, position.x);
		position.x += mp.x;
		if (position.x < min + size.width / 2)
			position.x = min + size.width / 2;
	}
	else if (mp.x > 0)
	{
		// right
		int TRX = position.x + size.width / 2; TRX /= SeaTileWidth;
		int TRY = position.y - size.height / 2; TRY /= SeaTileHeight;
		int BRY = position.y + size.height / 2; BRY /= SeaTileHeight;
		int min = SeaTileWidth * SeaTileX;
		for (int x = TRX + 1; x < SeaTileX; x++)
		{
			bool col = false;
			for (int y = TRY; y < BRY + 1; y++)
			{
				if (maptile[SeaTileX * y + x].attr == 1)
				{
					col = true;
					min = SeaTileWidth * x - 1;
					break;
				}
				if (maptile[SeaTileX * y + x].attr == 3)
				{
					col = true;
					min = SeaTileWidth * x - 3;

					showPopBackBtn(true);
					break;
				}
				else			
					showPopBackBtn(false);	break;
				

			}
			if (col)
				break;
		}
		//printf("min = %d, curr = %f\n", min, position.x);
		position.x += mp.x;
		if (position.x > min - size.width / 2)
			position.x = min - size.width / 2;
	}


	if (mp.y < 0)
	{
		// up
		int TLY = position.y - size.height / 2; TLY /= SeaTileHeight;
		int TLX = position.x - size.width / 2; TLX /= SeaTileWidth;
		int TRX = position.x + size.width / 2; TRX /= SeaTileWidth;
		int min = 0;
		for (int y = TLY - 1; y > -1; y--)
		{
			bool col = false;
			for (int x = TLX; x < TRX + 1; x++)
			{
				if (maptile[SeaTileX * y + x].attr == 1)
				{
					col = true;
					min = SeaTileHeight * (y + 1);
					break;
				}
				 if (maptile[SeaTileX * y + x].attr == 3)
				{
					col = true;
					min = SeaTileHeight * (y + 3);

					showPopBackBtn(true);
					break;
				}
				 else
					 showPopBackBtn(false);	break;
				 
			}
			if (col)
				break;
		}
		//printf("min = %d, curr = %f\n", min, position.x);
		position.y += mp.y;
		if (position.y < min + size.height / 2)
			position.y = min + size.height / 2;
	}
	else if (mp.y > 0)
	{
		// down
		int TLY = position.y + size.height / 2; TLY /= SeaTileHeight;
		int TLX = position.x - size.width / 2; TLX /= SeaTileWidth;
		int TRX = position.x + size.width / 2; TRX /= SeaTileWidth;
		int min = SeaTileHeight * SeaTileY;
		for (int y = TLY + 1; y < SeaTileY; y++)
		{
			bool col = false;
			for (int x = TLX; x < TRX + 1; x++)
			{
				if (maptile[SeaTileX * y + x].attr == 1)
				{
					col = true;
					min = SeaTileHeight * y - 1;
					break;
				}
				if (maptile[SeaTileX * y + x].attr == 3)
				{
					col = true;
					min = SeaTileHeight * y - 3;

					showPopBackBtn(true);
					break;
				}
				else
					showPopBackBtn(false);	break;
				
			}
			if (col)
				break;
		}
		//printf("min = %d, curr = %f\n", min, position.x);
		position.y += mp.y;
		if (position.y > min - size.height / 2)
			position.y = min - size.height / 2;

		
	}
}

/////////////////////////////////////////////////////////////////
//PopBackBtn
/////////////////////////////////////////////////////////////////

iPopup* bB;
iImage* backBtn;
void createPopBackBtn()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	bB = pop;

	

	iGraphics* g = iGraphics::instance();
	backBtn = new iImage();
	{
		iSize size = iSizeMake(50, 50);
		g->init(size);

		setRGBA(1, 0.8f, 0, 0.8f);
		g->fillRect(0, 0, size.width, size.height, 15);

		setStringSize(13);
		setStringRGBA(0, 0, 0, 1);
		setStringBorder(0);
		g->drawString(size.width/2, size.height/2, VCENTER | HCENTER, "뒤로가기");

		Texture* tex = g->getTexture();
		backBtn->addObject(tex);
		freeImage(tex);

	}
	pop->addObject(backBtn);

	pop->openPosition = iPointMake(5,5);
	pop->closePosition = iPointMake(5,5);

}



void freePopBackBtn()
{
	delete bB;
}

void showPopBackBtn(bool show)
{
	bB->show(show);
}

void drawPopBackBtn(float dt)
{
	bB->paint(dt);

}

bool keyPopBackBtn(iKeyState stat, iPoint point)
{
	if(bB->bShow == false)
		return false;
	if (bB->stat != iPopupStatProc)
		return true;
	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
		i = bB->selected;
		if (i == 0)
		{
			METHOD_LOADING f[1] = { loadSelectMap };
			setLoading(gs_selectmap, freeSeaBoat, f[0]);
		}
		break;

	case iKeyStateMoved:
		if (containPoint(point, backBtn->touchRect(bB->closePosition)))
		{
			j = 0;
		}
		
		bB->selected = j;
		break;
		
	case iKeyStateEnded:
		break;

	}
	return false;
}

/////////////////////////////////////////////////////////////////
//PopFishingGage
/////////////////////////////////////////////////////////////////
iPopup* gagebar;
iImage* gagebarRect;
float gageDt;
float throwDt;


void drawPopFishingGageAfter(iPopup* me, float dt);
void createPopFishingGage()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	gagebar = pop;

	gagebarRect = new iImage();
	iGraphics* g = iGraphics::instance();
	{
		iSize size = iSizeMake(25, 100);
		g->init(size);

		setRGBA(0, 0, 0, 1);
		g->drawRect(0, 0, size.width, size.height, 0);
	
		Texture* tex = g->getTexture();
		gagebarRect->addObject(tex);
		freeImage(tex);
	}
	pop->addObject(gagebarRect);

	pop->methodDrawAfter = drawPopFishingGageAfter;
	
}


void freePopFishingGage()
{
	delete gagebar;
//	delete gagebarRect;
}

void showPopFishingGage(bool show)
{
	gagebar->show(show);
	if (show)
	{
		gageDt = 0.0f;
		throwDt = 0.0f;
	}
}

void drawPopFishingGageAfter(iPopup* me, float dt)
{
	if (me->stat != iPopupStatProc)
		return;

	gageDt += dt;
	if (gageDt > _gageDt)
		gageDt -= _gageDt;
	iPoint p = gagebarRect->position + me->closePosition;

	setRGBA(0, 1, 0, 1);
	// 23 x 98
	iRect rt;
	rt.size.width = 23;
	rt.size.height = 15 + 83 * _sin(180 * gageDt / _gageDt);
	rt.origin.x = 1;
	rt.origin.y = 100 - rt.size.height;
	fillRect(rt);


	throwDt += dt;
		
	setRGBA(1, 0, 1, 1);
	iPoint tp0 = bt->position + offMt;
	iPoint tp1 = tp0 * (r*100);
	drawLine(tp0, tp1 * _sin(180 * throwDt));


}

void drawPopFishingGage(float dt)
{
	gagebar->paint(dt);
	iPoint gp = bt->position + offMt;

	gagebar->openPosition = iPointMake(gp.x+30 , gp.y-50);
	gagebar->closePosition = iPointMake(gp.x+30 , gp.y-50 );

	

}

bool keyPopFishingGage(iKeyState stat, iPoint point)
{

	if (gagebar->bShow == false)
		return false;
	if (gagebar->stat != iPopupStatProc)
		return true;
		
	switch (stat)
	{
	case iKeyStateBegan:
	
		float r = fabsf(gageDt - _gageDt / 2);
		printf("ability = %0.f %\n", r * 100);
		showPopFishingGage(false);
				
		break;

	case iKeyStateMoved:

		break;

	case iKeyStateEnded:

		

		break;

		

	}
	return true;
}

iImage** throwline;

void createFishingfloat()
{


}

void freeFishingfloat()
{

}

void showFishingfloat(bool show)
{

}


void drawFishingfloat(float dt)
{

}

bool keyFishingfloat(iKeyState stat, iPoint point)
{
	return false;
}

