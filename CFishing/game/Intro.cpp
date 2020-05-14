#if 0
#include "Intro.h"

#include "Loading.h"
#include "Menu.h"

int score, sScore, eScore;
float scoreDt;
#define _scoreDt 0.5f

float dmgDt;
#define _dmgDt 1.2f
Image* imgDmg;


void freeM(void* data)
{
	char* s = (char*)data;
	printf("freeM = %s\n", s);
	free(data);
}

Image** imgs;
void loadIntro()
{
	printf("loadIntro()\n");

	score = 0;
	sScore = 0;
	eScore = 0;
	scoreDt = _scoreDt;

	dmgDt = _dmgDt;

	iSize size = iSizeMake(256, 40);
	imgDmg = new Bitmap(size.width, size.height);
	Graphics* g2 = Graphics::FromImage(imgDmg);

	//Graphics* getG();
	//void setG(Graphics * g);
	extern Graphics* g;
	Graphics* gBackup = g;// getG();
	g = g2;//setG(g2);


	Texture* tex = createImage("assets/ex.png");
	drawImage(tex, 0, 0, TOP | LEFT);
	delete tex;

	setStringSize(35);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	drawString(size.width/2, size.height/2,
		VCENTER | HCENTER, "Dmg 1234");

	g = gBackup;//setG(gBackup);

	tex = createImage("assets/Player.bmp");// 64x32
	imgs = (Image**)malloc(sizeof(Image*) * 2);
	for (int i = 0; i < 2; i++)
	{
		Image* ii = new Bitmap(32, 32);
		Graphics* gg = Graphics::FromImage(ii);

		Graphics* gBackup = g;// getG();
		g = gg;//setG(g2);
		drawImage(tex, -32 * i, 0, TOP | LEFT);
		g = gBackup;//setG(gBackup);

		delete gg;
		imgs[i] = ii;
	}
}

void freeIntro()
{
	printf("freeIntro()\n");
}

void drawIntro(float dt)
{
	setRGBA(1, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	printf("============ start\n");
	static float delta = 0.0f;
	delta += dt;
	if (delta > 1.0f)
		delta -= 1.0f;
	float x = 50 + 100 * _sin(360 * delta);
	float y = 100 * _sin(180 * delta);
	setClip(x, y, 80, 150);
	setRGBA(0, 0, 1, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);

	const char* s = "안녕,busan,게임,korea,아카데미";
	int lineNum;
	char** line = iString::getStringLine(s, lineNum, ',');

	for (int i = 0; i < lineNum; i++)
		drawString(10, 10 + 40 * i, TOP | LEFT, line[i]);

	iString::freeStringLine(line, lineNum);

	setClip(0, 0, devSize.width, devSize.height);
	printf("============ end\n");

	if (scoreDt < _scoreDt)//(score != eScore)
	{
		scoreDt += dt;
		if (scoreDt > _scoreDt)
			scoreDt = _scoreDt;
		// 0.0     =>  _scoreDt(0.5)
		// sScore  =>  eScore
		score = sScore + (eScore - sScore) * scoreDt / _scoreDt;
	}
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	drawString(devSize.width / 2, devSize.height / 2, VCENTER | HCENTER,
		"score %d", score);

	if (dmgDt < _dmgDt)
	{
		float scale, alpha, degree;
		iPoint position;

		// 0.2f : 3배 크기에서 1 크기로 줄어든다.(scale=3 => 1
		if (dmgDt < 0.2f)
		{
			scale = 8.0f - 7.0f * dmgDt / 0.2f;
			alpha = dmgDt / 0.2f;
			position = iPointMake(devSize.width / 2, devSize.height / 2);
			degree = 360 * dmgDt / 0.2f;
		}
		// 0.2f : 1배 크기(1.0, 1.0)
		else if( dmgDt < 0.4f )
		{
			scale = 1.0f;
			alpha = 1.0f;
			position = iPointMake(devSize.width / 2, devSize.height / 2);
			degree = 0.0;
		}
		// 0.8f : 위로 사라진다.(scale=1.0, alpha= 1.0 => 0.0)
		else// if (dmgDt < _dmgDt)
		{
			scale = 1.0f;
			float r = (dmgDt - 0.4f) / 0.8f;
			alpha = 1.0f - r;
			position = iPointMake(devSize.width / 2,
								devSize.height / 2 - 100*r);
			degree = 0.0;
		}
		setRGBA(1, 1, 1, alpha);
		drawImage(tex, position.x, position.y,
			0, 0, imgDmg->GetWidth(), imgDmg->GetHeight(),
			VCENTER | HCENTER, scale, scale, 2, degree);
		dmgDt += 0.01;// dt;
	}

	static float aniDt = 0.0f;
	aniDt += dt;
	if (aniDt > 1.0f)
		aniDt -= 1.0f;

	setRGBA(1, 1, 1, 1);
	Image* ii = imgs[aniDt < 0.5f ? 0 : 1];
	drawImage(ii, devSize.width / 2, devSize.height / 2,
		0, 0, ii->GetWidth(), ii->GetHeight(),
		VCENTER | HCENTER, 5, 5, 2, 0, REVERSE_NONE);

}

void keyIntro(iKeyState stat, iPoint point)
{
	if (stat == iKeyStateBegan)
	{
		//setLoading(gs_menu, freeIntro, loadMenu);
	//	score += 100;
		if (score < eScore)
			score = eScore;
		sScore = score;
		eScore = score + 100;
		scoreDt = 0.0f;

		dmgDt = 0.0f;
	}
}

#else

#include "Intro.h"

#include "Loading.h"
#include "Menu.h"

void loadIntro()
{
	printf("loadIntro()\n");
}

void freeIntro()
{
	printf("freeIntro()\n");
}

void drawIntro(float dt)
{
}

void keyIntro(iKeyState stat, iPoint point)
{
	switch (stat) {

	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
		break;

	case iKeyStateEnded:
		break;
	}
}

#endif