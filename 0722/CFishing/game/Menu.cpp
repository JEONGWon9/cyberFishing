#include "Menu.h"

#include "Loading.h"
#include "SelectMap.h"

Texture* texBg;
Texture* texTitle;

void loadMenu()
{
	
	texBg = createImage("assets/menu/menuBg.jpg");
	texTitle = createImage("assets/menu/menuTitle.png");
	createPopPress();
	createPopMenu();
	createPopSetting();

	showPopPress(true);
	
}

void freeMenu()
{		 
	freeImage(texBg);
	freeImage(texTitle);
	freePopPress();
	freePopMenu();
	freePopSetting();


}

void drawMenu(float dt)
{
	// texBg : 1400 x 933
	float r0 = devSize.width / texBg->width;
	float r1 = devSize.height / texBg->height;
	float r = max(r0, r1);
	drawImage(texBg, devSize.width / 2, devSize.height / 2,
		0, 0, texBg->width, texBg->height, VCENTER | HCENTER,
		r, r, 2, 0, REVERSE_NONE);

	drawImage(texTitle, devSize.width / 2, 50,
		0, 0, texTitle->width, texTitle->height, TOP | HCENTER,
		0.6f, 0.6f, 2, 0, REVERSE_NONE);

	drawPopPress(dt);
	drawPopMenu(dt);
	drawPopSetting(dt);
	
}

bool touching = false;
iPoint prevPoint;
void moveTable(iPoint& off, iPoint mp, const iRect& rt, const iSize& size)
{
	off += mp;
	float minX = rt.origin.x + rt.size.width - size.width;
	float minY = rt.origin.y + rt.size.height - size.height;
	float maxX = rt.origin.x;
	float maxY = rt.origin.y;
	if (off.x < minX) off.x = minX;
	else if (off.x > maxX) off.x = maxX;
	if (off.y < minY) off.y = minY;
	else if (off.y > maxY) off.y = maxY;
}

void keyMenu(iKeyState stat, iPoint point)
{

	   
	if (keyPopPress(stat, point))
		return;
	if (keyPopMenu(stat, point))
		return;
	if (keyPopSetting(stat, point))
		return;
}

// -----------------------------------
// popPress
// -----------------------------------
iPopup* popPress;


void createPopPress()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);

	iImage* img = new iImage();
	Texture* tex = createImage("assets/menu/pressAnyKey.png");
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	iPoint p = iPointMake(devSize.width / 2,
		devSize.height * 0.67f );
	pop->openPosition = p;
	pop->closePosition = p;
	//pop->methodClose = closePopPress;
	popPress = pop;
}

void freePopPress()
{
	delete popPress;
}

void showPopPress(bool show)
{
	popPress->show(show);
}

void drawPopPress(float dt)
{
	popPress->paint(dt);
}

bool keyPopPress(iKeyState stat, iPoint point)
{
	if (popPress->bShow == false)
		return false;
	if (popPress->stat != iPopupStatProc)
		return true;

	if (stat == iKeyStateBegan)
	{
		showPopPress(false);
		showPopMenu(true);
	}
	return true;
}

//============================================
//PoopMenu
//============================================

iPopup* PopMenu;
iImage** imgMenuBtn;

void createPopMenu()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);//
	PopMenu = pop;

	imgMenuBtn = (iImage**)malloc(sizeof(iImage*) * 3);
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			Texture* tex = createImage("assets/menu/menuBtn%d%d.png", i, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(0, 120 * i);
		pop->addObject(img);
		imgMenuBtn[i] = img;//?
	}
	pop->openPosition = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePosition = iPointMake((devSize.width - 280) / 2,
		(devSize.height - 350) / 2);


}

void freePopMenu()
{
	delete PopMenu;//생성해줬으니까.
	free(imgMenuBtn);

}

void showPopMenu(bool show)
{
	PopMenu->show(show);
}

void drawPopMenu(float dt)
{
	for (int i = 0; i < 3; i++)
		imgMenuBtn[i]->setTexAtIndex(i == PopMenu->selected);
	PopMenu->paint(dt);

}

bool keyPopMenu(iKeyState stat, iPoint point)
{

	if (PopMenu->bShow == false)
		return false;
	if (PopMenu->stat != iPopupStatProc)
		return true;
	int i,j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
		i = PopMenu->selected;
		if (i == -1)break;
		//audioPlay(1);
		if (i == 0)
		{
			METHOD_LOADING m[1] = { loadSelectMap };
			setLoading(gs_selectmap, freeMenu,m[0]);
		}
		else if (i == 1)
		{
			showPopSetting(true);
		}
		else if (i == 2)
		{
			//end
			showPopPress(true);
			showPopMenu(false);
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(point, imgMenuBtn[i]->touchRect(PopMenu->closePosition)))
			{
				j = i;
				break;
			}
		}
		PopMenu->selected = j;
		break;

	case iKeyStateEnded:

		break;

	}

	return true;
}
//============================================
//PopSetting
//============================================


iPopup* popSettings;
iImage** imgSettingsBtn;

void createPopSetting()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	popSettings = pop;

	imgSettingsBtn = (iImage**)malloc(sizeof(iImage*) * 2);

	iImage* imgbg = new iImage();
	iGraphics* g = iGraphics::instance();
	{
		iSize size = iSizeMake(256, 256);
		g->init(size);

		setRGBA(0, 1, 0, 0.8f);
		g->fillRect(0, 0, size.width, size.height, 10);

		setStringSize(30);
		setStringRGBA(1, 1, 1, 1);
		setStringBorder(0);
		setRGBA(1, 1, 1, 1);
		g->drawString(size.width-200, size.height-50, VCENTER | HCENTER, "환경설정");

		Texture* tex = g->getTexture();
		imgbg->addObject(tex);
		freeImage(tex);
	}
	pop->addObject(imgbg);

	//btnx
	iImage* btnx = new iImage;
	for (int i = 0; i < 2; i++)
	{
		iSize size = iSizeMake(201, 77);
		g->init(size);
		igImage* ig = g->createIgImage("assset / menu / btnX % d.png", i);
		g->drawImage(ig, 0, 0, TOP | LEFT);
		g->freeIgImage(ig);

		Texture* tex = g->getTexture();
		btnx->addObject(tex);
		freeImage(tex);		

		imgSettingsBtn[i] = btnx;
		btnx->position = iPointMake(size.width - 150, size.height - 50);
	}
	pop->addObject(btnx);

	pop->openPosition = iPointMake((devSize.width - imgbg->tex->width) / 2, imgbg->tex->height);
	pop->closePosition = iPointMake((devSize.width - imgbg->tex->width) / 2, (devSize.height - imgbg->tex->height) / 2);


}

void freePopSetting()
{
	delete popSettings;
	free(imgSettingsBtn);
}

void showPopSetting(bool show)
{
	popSettings->show(show);
}

void drawPopSetting(float dt)
{
	popSettings->paint(dt);

}

bool keyPopSetting(iKeyState stat, iPoint point)
{

	if (popSettings->bShow == false)
		return false;
	if (popSettings->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
	case iKeyStateBegan:
		i = popSettings->selected;
		if (i == -1) break;
		if (i == 0);
		{
			showPopSetting(false);
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(point, imgSettingsBtn[i]->touchRect(popSettings->closePosition)))
			{
				j = i;
				break;
			}

		}
		popSettings->selected = j;
		break;

	case iKeyStateEnded:
		break;

	}
	return false;
}