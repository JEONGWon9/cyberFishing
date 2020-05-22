#include "SeaBoat.h"

#include "Menu.h"
#include "Loading.h"
#include "SelectMap.h"



uint8 tile[SeaTileX * SeaTileY] = {
	1,1,1,1,1,1,1,1,1,1,
	0,0,3,0,0,0,0,0,0,0,
	0,0,3,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,3,0,0,0,
	0,0,0,0,0,0,3,0,0,0,
	1,1,1,1,1,1,1,1,1,1,
};

MapTile* maptile;
Boat* bt;

iPoint offMt;
float gageDt;
float throwDt;
float floatDt;
float biteDt;
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

	bt = new Boat();

	bt->position = iPointMake(SeaTileWidth,SeaTileHeight);
	bt->size = iSizeMake(MapCharWidth, MapCharHeight);
	bt->speed = 100;
	
	createPopBackBtn();
	createPopFishingGage();
	createFishingfloat();


}

void freeSeaBoat()
{
	free(maptile);
	delete bt;

	freePopBackBtn();
	freePopFishingGage();
	freeFishingfloat();
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
	setRGBA(1, 1, 1, 1);

	/////////////////////////////////////////////////////////////////////////////

	//iPoint movement = iPointMake(0, 0) * dt;

	uint32 keyStat = getKeyStat();

	iPoint movement = iPointMake(0, 0) * dt;
	iPoint v = iPointZero;

	if (keyStat & keyboard_left) v.x = -1;
	else if (keyStat & keyboard_right) v.x = 1;
	if (keyStat & keyboard_up) v.y = -1;
	else if (keyStat & keyboard_down) v.y = 1;
	//printf("%d \n", bt->behave);

	if (bt->throwing==0 && v != iPointZero)
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
		bt->setBehave(v);
	}
	

	//boat//
	bt->paint(dt, offMt,v);

	if (getKeyDown() & keyboard_space)
	{
		showPopFishingGage(true);
		
	}

	drawPopBackBtn(dt);
	drawPopFishingGage(dt);
	drawFishingfloat(dt);

}


void keySeaBoat(iKeyState stat, iPoint point)
{

	keyPopBackBtn(stat, point);
	keyPopFishingGage(stat, point);
	keyFishingfloat(stat, point);
}

Boat::Boat()
{
	imgs = (iImage**)malloc(sizeof(iImage*) * BoatBehave_num);

	for (int i = 0; i < BoatBehave_num; i++)
	{
		switch (i)
		{
			case BoatBehave_right:case BoatBehave_up:case BoatBehave_down:
			case BoatBehave_right_up:case BoatBehave_right_down:
			{
				int index[BoatBehave_num] = { 0, 0, 1, 2, 3, 3, 4, 4 };
				iImage* img = new iImage();

				Texture** tex = createDivideImage(9, 1, "assets/Boat/boat_%d.png", index[i]);
				for (int j = 0; j < 9; j++)
				{
					img->addObject(tex[j]);
					freeImage(tex[j]);
				}
				free(tex);
				img->position = iPointMake(-30, -40);
				imgs[i] = img;
				break;
			}
			case BoatBehave_left:case BoatBehave_left_up:case BoatBehave_left_down:
			{
				imgs[i] = imgs[i - 1]->copy();
				imgs[i]->leftRight = 1;
				break;
			}
		}
		imgs[i]->_aniDt = 0.3f;
		imgs[i]->startAnimation();
	}
	img = imgs[BoatBehave_left];
	behave = BoatBehave_left;

	throwing = 0;
	throwResult = 20;// init
	throwMin = 20;
	throwMax = 70;
}

Boat::~Boat()
{
	for (int i = 0; i < BoatBehave_num; i++)
		delete imgs[i];
	free(imgs);
}

void Boat::setBehave(BoatBehave be)
{
	if (be == behave)
		return;

	behave = be;
	img = imgs[be];
}

void Boat::setBehave(iPoint v)
{
	BoatBehave be = behave;
	if (v.x < 0.0f)
	{
		if (v.y < 0.0f)			be = BoatBehave_left_up;
		else if (v.y == 0.0f)	be = BoatBehave_left;
		else					be = BoatBehave_left_down;
	}
	else if (v.x == 0.0f)
	{
		if (v.y < 0.0f)			be = BoatBehave_up;
		else if (v.y == 0.0f);
		else					be = BoatBehave_down;
	}
	else
	{
		if (v.y < 0.0f)			be = BoatBehave_right_up;
		else if (v.y == 0.0f)	be = BoatBehave_right;
		else					be = BoatBehave_right_down;
	}
	setBehave(be);

}

void Boat::paint(float dt, iPoint off,iPoint v)
{
	img->paint(dt, position + off);
	
		
	if (throwing == 0)
	{
		
	}
	else if (throwing == 1)
	{
			
	}
	else if (throwing == 2)
	{

#if 1
		behave;
		throwResult;//   bt->throwMin + r * (bt->throwMax - bt->throwMin);
		throwAniDt += dt;
		
		

		iPoint sp = iPointMake(position.x + off.x, position.y + off.y);
		
		iPoint ep = iPointMake(sp.x+throwResult *v.x , sp.y + throwResult * v.y );
		
		if(throwAniDt>_throwAniDt)
		{ 
		
			throwing = 3;
		}
		
		drawLine(sp,ep);
	
		Texture* tex = createImage("assets/Boat/fishing_float.png");
		drawImage(tex, ep.x, ep.y, HCENTER | VCENTER);
		freeImage(tex);
		
		
#else 
	

		behave;
		throwResult;
		throwAniDt += dt;
		if (behave == BoatBehave_left)				v.x < 0.0f  && v.y == 0.0f;
		else if (behave == BoatBehave_left_up)		v.x < 0.0f  && v.y < 0.0f;
		else if (behave == BoatBehave_left_down)	v.x < 0.0f  && v.y> 0.0f;
		if (behave == BoatBehave_up)				v.x == 0.0f && v.y < 0.0f;
		else if (behave == BoatBehave_down)			v.x == 0.0f && v.y > 0.0f;
		if (behave == BoatBehave_right)				v.x > 0.0f  && v.y == 0.0f;
		else if (behave == BoatBehave_right_up)		v.x > 0.0f  && v.y < 0.0f;
		else if (behave == BoatBehave_right_down)	v.x > 0.0f  && v.y > 0.0f;


		if (throwAniDt > 2.0)
			throwing = 3;
		iPoint sp = iPointMake(position.x + off.x, position.y + off.y);

		iPoint ep = iPointMake(sp.x + throwResult * v.x, sp.y + throwResult *v.y);

	
		//printf("%f,%f\n",ep.x,ep.y);
		drawLine(sp, ep);

		Texture* tex = createImage("assets/Boat/fishing_float.png");
		drawImage(tex, ep.x, ep.y, VCENTER | HCENTER);
		freeImage(tex);
#endif

	}
	else if(throwing == 3)
	{
		biteDt += dt;

		showFishingfloat(true);
		if (biteDt > _biteDt)
		{
			showFishingfloat(false);
			throwing == 4;
		}

	}
	else if (throwing == 4)
	{
		throwing = 0;
	}
}



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
					min = SeaTileWidth * (x + 1);

					showPopBackBtn(true);
					break;
				}			
				else
				{
					showPopBackBtn(false);
				}
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
					min = SeaTileWidth * x - 1;

					showPopBackBtn(true);
					break;
				}
				else
				{
					showPopBackBtn(false);
				}

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
				else if (maptile[SeaTileX * y + x].attr == 3)
				{
					col = true;
					min = SeaTileHeight * (y + 1);

					showPopBackBtn(true);
					break;
				}		
				else
				{
					showPopBackBtn(false);
				}
				 
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
				else if (maptile[SeaTileX * y + x].attr == 3)
				{
					col = true;
					min = SeaTileHeight * y - 1;

					showPopBackBtn(true);
					break;
				}
				else
				{
					showPopBackBtn(false);
				}
				
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
		bt->throwing = 1;
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
	
		float r = fabsf(gageDt - _gageDt);
		bt->throwing = 2;
		bt->throwAniDt = 1.0f;
		bt->throwResult = bt->throwMin + r * (bt->throwMax - bt->throwMin);
		printf("ability = %0.f %\n", r * 100);
		showPopFishingGage(false);
				
		break;
	}
	return false;
}


/////////////////////////////////////////////////////////////////
//PopFishingfloat
/////////////////////////////////////////////////////////////////



iPopup* fishingFloat;
iImage* fbg;

void drawFishingfloatAfter(iPopup* me, float dt);
void createFishingfloat()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);	
	fishingFloat = pop;
	
	fbg = new iImage();
	iGraphics* g = iGraphics::instance();
	{
		iSize size = iSizeMake(200, 200);
		g->init(size);

		setRGBA(1, 0, 0, 1);
		g->drawRect(0, 0, size.width + 1, size.height + 1,0);

		setRGBA(0, 1, 1, 1);
		g->fillRect(0, 0, size.width, size.height, 0);

		Texture* tex = g->getTexture();
		fbg->addObject(tex);
		freeImage(tex);
	}
	pop->addObject(fbg);

	pop->openPosition = iPointMake(devSize.width/2, devSize.height/2);
	pop->closePosition = iPointMake(devSize.width/2, devSize.height /2);
	pop->methodDrawAfter = drawFishingfloatAfter;

}

void freeFishingfloat()
{
	delete fishingFloat;

}

void showFishingfloat(bool show)
{
	fishingFloat->show(show);

	if (show)
	{
		floatDt = 0.0f;
		bt->throwing = 3;
	}

}

void drawFishingfloatAfter(iPopup* me, float dt)
{
	if (me->stat != iPopupStatProc)
		return;

	floatDt += dt;

	iPoint fp = fbg->position + me->closePosition;
	if (floatDt < _floatDt)//2초있다가사라지게하고싶음
	{
		setRGBA(1, 1, 1, 1);
		Texture* tex = createImage("assets/Boat/fishingfloat.png");
		drawImage(tex, 150 + fp.x / 2 * _sin(180 * floatDt / _floatDt), fp.y / 2, HCENTER | VCENTER);
		freeImage(tex);
	}
	else if (floatDt > 4.0f)
	{
		bt->throwing = 4;
	}
		

	
}



void drawFishingfloat(float dt)
{
	fishingFloat->paint(dt);
}


bool keyFishingfloat(iKeyState stat, iPoint point)
{
	if (fishingFloat->bShow == false)
		return false;
	if (fishingFloat->stat != iPopupStatProc)
		return true;
	
	
	switch (stat)
	{
	case iKeyStateBegan:
		floatDt;
		biteDt = 0.0f;
		bt->throwing = 4;
		showFishingfloat(false);

		break;
	}
	return false;
}

