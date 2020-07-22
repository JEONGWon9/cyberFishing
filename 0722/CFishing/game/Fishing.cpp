#include "Fishing.h"
#include "Menu.h"
#include "Loading.h"
#include "SeaBoat.h"
#include "Game.h"

Texture* fishingBg;
Texture* fishingrod;

Maptile_F* fishingmap;
iPoint offFt;
float devfishing_width, devfishing_height;

iPoint fishP;

Rod* rod;
Fish* fish;


uint8 fishingtile[fishingtileNumX*fishingtileNumY] =
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,// 1 == sky
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,// 0 == ocean
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,//2 == catch 포인트
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
	fishingrod = createImage("assets/fish/fishingrod.png");


	rod = new Rod();

	rod->rodDegree = 0;
	rod->roding = false;
	rod->atk = 1;

	fish = new Fish();

	fish->position = iPointMake(devSize.width/2,devSize.height/2);
	fish->size = iSizeMake(50, 10);
	fish->speed = 100;

}
void freeFishing()
{
	delete rod;
	delete fish;

	free(fishingmap);
	freeImage(fishingBg);
	freeImage(fishingrod);



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
		r,0.7,0, 0, REVERSE_NONE);


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
		case 2: setRGBA(1, 0, 0, 1); break;
		}
		
		float x = offFt.x +devfishing_width * (i % fishingtileNumX);
		float y = offFt.y +devfishing_height * (i / fishingtileNumX);
		fillRect(x, y,devfishing_width,devfishing_height);
		setRGBA(0, 0, 0, 1);
		setLineWidth(2);
		drawRect(x, y, devfishing_width, devfishing_height);

	}

	///////////////////////////////////////////////////////////
	///물고기
	///////////////////////////////////////////////////////////
	iPoint fmove = iPointMake(fish->position.x , fish->position.y);
	setRGBA(1, 1, 1, 1);

	drawImage(fish->fishbubble->tex,fmove.x,fmove.y, VCENTER | HCENTER);

	fish->position.y--;

	

	//fish->paint(dt, fish->position);
	fish->fishDt += dt;
	if (fish->fishDt > _fishDt)
		fish->fishDt -= _fishDt;



	///////////////////////////////////////////////////////////
	///낚시대
	///////////////////////////////////////////////////////////



	iPoint hand = iPointMake(devSize.width / 2, devSize.height);
	iPoint c = hand - iPointMake(0, fishingrod->height / 2);
	c = iPointRotate(c, hand, rod->rodDegree);
	setRGBA(1, 1, 1, 1);
	drawImage(fishingrod, c.x, c.y,
		0, 0, fishingrod->width, fishingrod->height, VCENTER | HCENTER, 1.0f, 1.0f,
		2, rod->rodDegree, REVERSE_NONE);

	iPoint rodhead = hand - iPointMake(5, fishingrod->height);
	rodhead = iPointRotate(rodhead, hand, rod->rodDegree);
	

	setLineWidth(2);
	
	drawLine(rodhead,fmove);
	


}

void keyFishing(iKeyState stat, iPoint point)
{
	uint32 keyStat = getKeyStat();
#if 0
	if (stat == iKeyStateBegan)
	{
		rod->roding = true;
		uint32 keyStat = getKeyStat();
		
	
	}
	
	else if (stat == iKeyStateMoved)
	{
		if (rod->roding)
		{		
			fish->position.y += 2.0;

			rod->rodDegree = 45 * (devSize.width/2 - point.x) / 200;
			if (rod->rodDegree > 45)
			{
				rod->rodDegree = 45;
				fish->position.x -= 2.0;
			}
			else if (rod->rodDegree < -45)
			{
				rod->rodDegree = -45;
				fish->position.x += 2.0;
			}

		}
	}
	else if (stat == iKeyStateEnded)
	{

		rod->rodDegree = 0;
		rod->roding = false;
	}
#else
	switch (stat)
	{
	case iKeyStateBegan:

		rod->roding = true;
		
		break;

	case iKeyStateMoved:
		if (rod->roding)
		{
			fish->position.y += 2.0;

			rod->rodDegree = 45 * (devSize.width / 2 - point.x) / 200;
			if (rod->rodDegree > 45)
			{
				rod->rodDegree = 45;
				fish->position.x -= 2.0;
			}
			else if (rod->rodDegree < -45)
			{
				rod->rodDegree = -45;
				fish->position.x += 2.0;
			}

		}
		break;

	case iKeyStateEnded:
		rod->rodDegree = 0;
		rod->roding = false;

		break;

	}

	
#endif


}

Fish::Fish()
{
//생성
	iImage* img = new iImage();
	iGraphics* g = iGraphics::instance();
	{
		iSize size = iSizeMake(50, 10);
		g->init(size);
	
		setRGBA(1, 0, 0, 1);
		g->fillRect(0, 0, size.width, size.height);
	
		Texture* tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	
	fishbubble = img;

	fishDt = 0.0f;

}

Fish::~Fish()
{
	free(fishbubble);
}

void Fish::move(iPoint mp)
{
	if (mp.x < 0)
	{
		// left
		int tlx = position.x - size.width / 2; tlx /=  fishingtileW;
		int tly = position.y - size.height / 2; tly /= fishingtileH;
		int bly = position.y + size.height / 2; bly /= fishingtileH;
		int min = 0;
		for (int x = tlx - 1; x > -1; x--)
		{
			bool col = false;
			for (int y = tly; y < bly + 1; y++)
			{
				if (fishingmap[fishingtileNumX * y + x].attr == 1)
				{
					col = true;
					min = fishingtileW * (x + 1);
					break;
				}
				if (fishingmap[fishingtileNumX * y + x].attr == 2)
				{
					

					
					break;
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
		int TRX = position.x + size.width / 2; TRX /= fishingtileW;
		int TRY = position.y - size.height / 2; TRY /= fishingtileH;
		int BRY = position.y + size.height / 2; BRY /= fishingtileH;
		int min = fishingtileW * fishingtileNumX;
		for (int x = TRX + 1; x < fishingtileNumX; x++)
		{
			bool col = false;
			for (int y = TRY; y < BRY + 1; y++)
			{
				if (fishingmap[fishingtileNumX * y + x].attr == 1)
				{
					col = true;
					min = SeaTileWidth * x - 1;
					break;
				}
				if (fishingmap[fishingtileNumX * y + x].attr == 2)
				{

					break;
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
		int TLY = position.y - size.height / 2; TLY /= fishingtileH;
		int TLX = position.x - size.width / 2; TLX /= fishingtileW;
		int TRX = position.x + size.width / 2; TRX /= fishingtileW;
		int min = 0;
		for (int y = TLY - 1; y > -1; y--)
		{
			bool col = false;
			for (int x = TLX; x < TRX + 1; x++)
			{
				if (fishingmap[fishingtileNumX * y + x].attr == 1)
				{
					col = true;
					min = fishingtileH * (y + 1);
					break;
				}
				else if (fishingmap[fishingtileNumX * y + x].attr == 2)
				{
					
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
		int TLY = position.y + size.height / 2; TLY /= fishingtileH;
		int TLX = position.x - size.width / 2; TLX /=  fishingtileW;
		int TRX = position.x + size.width / 2; TRX /=  fishingtileW;
		int min = fishingtileH * fishingtileNumY;
		for (int y = TLY + 1; y < fishingtileNumY; y++)
		{
			bool col = false;
			for (int x = TLX; x < TRX + 1; x++)
			{
				if (fishingmap[fishingtileNumX * y + x].attr == 1)
				{
					col = true;
					min = fishingtileH * y - 1;
					break;
				}
				else if (fishingmap[fishingtileNumX * y + x].attr == 2)
				{
					
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


void Fish::paint(float dt, iPoint off)
{
	
	
	fishbubble->paint(dt, off);

}


void createPopTension()
{

}

void freePopTension()
{

}

void showPopTension(bool show)
{

}

void drawPopTension(float dt)
{

}

bool keyPopTension(iKeyState stat, iPoint point)
{
	return false;
}
