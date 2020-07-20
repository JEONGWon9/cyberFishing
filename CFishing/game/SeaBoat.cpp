#include "SeaBoat.h"

#include "Menu.h"
#include "Loading.h"
#include "SelectMap.h"
#include "Fishing.h"


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
float heightDt;
float bfloatAniDt;

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

	if (bt->throwing < 2)// 0 or 1
	{
		if (keyStat & keyboard_left) v.x = -1;
		else if (keyStat & keyboard_right) v.x = 1;
		if (keyStat & keyboard_up) v.y = -1;
		else if (keyStat & keyboard_down) v.y = 1;
		//printf("%d \n", bt->behave);
	}

	if (bt->throwing == 0)
	{
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
			bt->setBehave(v);
		}

		if (getKeyDown() & keyboard_space)
		{
			showPopFishingGage(true);
			bt->throwing = 1;
		}
	}
	else if (bt->throwing == 1)
	{
		if (v != iPointZero)
		{
			showPopFishingGage(false);
			bt->throwing = 0;
		}
	}

	//boat//
	bt->paint(dt,offMt);


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
	throwResult =10;// init
	throwMin = 10;
	throwMax = 60;

	texFloat = createImage("assets/Boat/fishing_float.png");
}

Boat::~Boat()
{
	for (int i = 0; i < BoatBehave_num; i++)
		delete imgs[i];
	free(imgs);

	freeImage(texFloat);
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

void Boat::paint(float dt, iPoint off)
{
	img->paint(dt, position + off);
		
	if (throwing == 0)
	{
		// 찾기
	}
	else if (throwing == 1)
	{
		// 던질준비
	}
	else if (throwing == 2)
	{
		// 던짐
		//printf("throwResult = %f\n", throwResult);
		iPoint sp = position + off;
		iPoint ep = position + throwV * throwResult + off;
		
		throwAniDt += dt;
		
		if (throwAniDt > _throwAniDt)
		{
			throwAniDt = _throwAniDt;
			showFishingfloat(true);
			throwing = 3;
		}
		float r = throwAniDt / _throwAniDt;
		iPoint cp = sp * (1.0f - r) + ep * r;

		setRGBA(1, 1, 1, 1);
		drawLine(sp, cp);
		drawImage(texFloat, cp.x, cp.y, HCENTER | VCENTER);
	}
	else if(throwing == 3)
	{
		// 낚시준비
		iPoint sp = position + off;
		iPoint cp = position + throwV * throwResult + off;

		setRGBA(1, 1, 1, 1);
		drawLine(sp, cp);
		drawImage(texFloat, cp.x, cp.y, HCENTER | VCENTER);

		biteDt += dt;
		if (biteDt > _biteDt)
		{	// 시간 오버 - 취소처리
			biteDt = _biteDt;
			showFishingfloat(false);

			throwing = 0;
		}
		
	}
	else if (throwing == 4)
	{
	
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


void drawPopFishingGageAfter(iPopup* me, iPoint p, float dt);
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
	}
}

void drawPopFishingGageAfter(iPopup* me,iPoint p, float dt)
{
	if (me->stat != iPopupStatProc)
		return;

	gageDt += dt;
	if (gageDt > _gageDt)
		gageDt -= _gageDt;
	p = gagebarRect->position + me->closePosition;

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
		
		showPopFishingGage(false);
		bt->throwing = 2;

		iPoint v = iPointZero;
		switch (bt->behave) {
		case BoatBehave_right: v.x = 1; break;
		case BoatBehave_left: v.x = -1; break;
		case BoatBehave_up: v.y = -1; break;
		case BoatBehave_down: v.y = 1; break;

		case BoatBehave_right_up: v.x = 1; v.y = -1; break;
		case BoatBehave_left_up: v.x = -1; v.y = -1; break;
		case BoatBehave_right_down: v.x = 1; v.y = 1; break;
		case BoatBehave_left_down: v.x = -1; v.y = 1; break;
		}
		bt->throwV = v / iPointLength(v);

		float r = fabsf(gageDt - _gageDt);
		printf("ability = %0.000f %\n", r * 100);
		bt->throwResult = bt->throwMin * (1.0f - r) * bt->throwMax * r;
		bt->throwAniDt = 0.0f;
		heightDt = 0.0f;
		break;
	}
		
}


/////////////////////////////////////////////////////////////////
//PopFishingfloat
/////////////////////////////////////////////////////////////////



iPopup* fishingFloat;
iImage* fbg;

Texture* texBFloat;
float _bfloatAniDt, bfloatAniDegree;

void drawFishingfloatAfter(iPopup* me, iPoint p, float dt);
void createFishingfloat()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);	
	fishingFloat = pop;
	
	fbg = new iImage();
	iGraphics* g = iGraphics::instance();
	{
		iSize size = iSizeMake(200, 200);
		g->init(size);

	
		setRGBA(0, 1, 1, 1);
		g->fillRect(0, 0, size.width, size.height, 0);

		Texture* tex = g->getTexture();
		fbg->addObject(tex);
		freeImage(tex);
	}
	pop->addObject(fbg);

	pop->openPosition = iPointMake(devSize.width/2 +200, devSize.height/2-200);
	pop->closePosition = iPointMake(devSize.width/2+200 , devSize.height /2-200);
	pop->methodDrawAfter = drawFishingfloatAfter;
	
	texBFloat = createImage("assets/Boat/fishingfloat.png");
	bfloatAniDt = 0.0f;
	_bfloatAniDt = 0.5f; // 물렸을때0.2f;
	bfloatAniDegree = 8.0f;// 16.f;
}

void freeFishingfloat()
{
	delete fishingFloat;

	freeImage(texBFloat);
}

void showFishingfloat(bool show)
{
	fishingFloat->show(show);

	//if (show)
	//{
	//	//floatDt = 0.0f;
	//	bt->throwing = 3;
	//}

}
#define floatAniDt 4.5f
void drawFishingfloatAfter(iPopup* me,iPoint p, float dt)
{
	if (me->stat != iPopupStatProc)
		return;



	setRGBA(1, 1, 1, 1);
	p = fbg->position + iPointMake(fbg->tex->width/2, fbg->tex->height/2);

	bfloatAniDt += dt;
	
	if (bfloatAniDt > floatAniDt)//물렸을때
	{
		_bfloatAniDt = 0.2f;
		bfloatAniDegree = 16.f;
		//#Bug  
	}
	float degree = bfloatAniDegree * _sin(360 * bfloatAniDt / _bfloatAniDt);
	if (degree < 0)
		degree += 360;
	
	drawImage(texBFloat, p.x, p.y, 0, 0, texBFloat->width, texBFloat->height,
		VCENTER | HCENTER, 1.0f, 1.0f, 2, degree, REVERSE_NONE);
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

		if (bfloatAniDt < floatAniDt)
		{
			showFishingfloat(false);
		
			bfloatAniDt = 0.0f;
			biteDt = 0.0f;
			floatDt = 0.0f;
			bt->throwing = 0;
		}
		else if (bfloatAniDt > floatAniDt)
		{
			METHOD_LOADING fishing[1] = { loadFishing };
			setLoading(gs_fishing, freeSeaBoat, fishing[0]);

		}
		break;

	}
	return false;
}

