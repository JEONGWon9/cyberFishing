#include "MapEditor.h"

WndCtrlSystem* wcsMapEditor;

// Left - state
HWND hCbState;

// Left - 0. offset
HWND* hEbOff;

// Left - 1. image
HWND hBtnOpenImage;
void btnOpenImageUpdate(HWND hwnd);
HWND* hEbOpenImage;

WndGL* wgTile;
void methodTileUpdate(float dt);

// Left - 2. collision
HWND hCbCollision;

// Right - world map
HWND* hEbMapNum;
HWND* hBtnMapOpen;
void btnMapOpenUpdate(HWND hwnd);

WndGL* wgMap;
void methodMapUpdate(float dt);

HWND* btnAllDelCanvas;

void methodallDel(HWND hwnd);


WndGL* selectTile;
void methodSelectTileUpdate(float dt);




int selectedTileIndex;
MapTile* mapTile;
//iPoint prevPoint;
iPoint offMap;
iPoint offTile;
bool moving;
bool del;
bool Pmove;

void loadMapEditor(HWND hwnd)
{
	int i;

	initWndCtrlSystem();

	wcsMapEditor = new WndCtrlSystem(hwnd);
	setWndCtrlStyem(wcsMapEditor);
	createWndGroup(2, 5, 250, 430, "MapTileSet", NULL, NULL);
    // to do....
	const char* strState[3] = {"이동", "이미지", "충돌"};
	hCbState = createWndComboBox(5, 25, 80, 160, strState, 3, NULL, NULL);

	hEbOff = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
	{
		createWndStatic(5 + 80 * i, 45, 10, 25, i == 0 ? "X" : "Y", NULL, NULL);
		hEbOff[i] = createWndEditBox(20 + 80*i, 60, 60, 25, "0", WndEditBoxStyle_int, NULL, NULL);
	}

	// open, numX, numY
	hBtnOpenImage = createWndButton(5, 95, 100, 25, "Open Image", NULL, btnOpenImageUpdate);
	hEbOpenImage = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hEbOpenImage[i] = createWndEditBox(110 + 40*i, 95, 35, 25, "5", WndEditBoxStyle_int, NULL, NULL);

	wgTile = createOpenGL(5, 125, 240, 200, methodTileUpdate, 240, 200);

	const char* strCollision[3] = {"모두이동불가", "공중이동불가", "모두이동가능"};
	hCbCollision = createWndComboBox(5, 335, 130, 120, strCollision, 3, NULL, NULL);
	


	selectTile = createOpenGL(5, 365, 50, 50, methodSelectTileUpdate, 300, 300);
	createWndStatic(60, 380, 80, 20, "SelectTile", NULL, NULL);

	//
	// Right
	//
	createWndGroup(280, 2, 640, 670, "MapCanvas", NULL, NULL);
	hEbMapNum = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hEbMapNum[i] = createWndEditBox(300 + 60 * i, 30, 35, 25, "50", WndEditBoxStyle_int, NULL, ebMapNumUpdate);
	createWndStatic(340,30, 10, 25, "x", NULL, NULL);

	const char* strMap[2] = {"Open Map", "Save Map"};
	hBtnMapOpen = (HWND*)malloc(sizeof(HWND) * 2);
	for (i = 0; i < 2; i++)
		hBtnMapOpen[i] = createWndButton(400 + 105 * i, 30, 100, 25, strMap[i], NULL, btnMapOpenUpdate);

	wgMap = createOpenGL(300, 65, 600, 600, methodMapUpdate, 600, 600);
	

	const char* strpaint[2] = { "DelAll","Bucket" };
	
	btnAllDelCanvas = (HWND*)malloc(sizeof(HWND) * 2);
	for(i = 0; i<2; i++)
		btnAllDelCanvas[i] = createWndButton(620+100*i, 30, 100, 25, strpaint[i], NULL, methodallDel);

	selectedTileIndex = -1;
	int numX = getWndInt(hEbMapNum[0]);
	int numY = getWndInt(hEbMapNum[1]);
	mapTile = (MapTile*)malloc(sizeof(MapTile) * numX * numY);
	for (int j = 0; j < numY; j++)
		for (i = 0; i < numX; i++)
			mapTile[numX * j + i].imgIndex = -1;
	offMap = iPointZero;
	offTile = iPointZero;
	moving = false;
	del = false;
}


void ebMapNumUpdate(HWND hwnd)
{
	free(mapTile);
	free(btnAllDelCanvas);

	int numX = getWndInt(hEbMapNum[0]);
	int numY = getWndInt(hEbMapNum[1]);
	mapTile = (MapTile*)malloc(sizeof(MapTile) * numX * numY);
}


void freeMapEditor()
{
    // to do.....
	delete wcsMapEditor;
	free(mapTile);
}

void dragMapEditor(WPARAM wParam, LPARAM lParam)
{
	wcsMapEditor->dropFiles(wParam, lParam);
}

LRESULT colorMapEditor(WPARAM wParam, LPARAM lParam)
{
	return wcsMapEditor->color(wParam, lParam);
}

void updateMapEditor(WPARAM wParam, LPARAM lParam)
{
	wcsMapEditor->update(wParam, lParam);
}
void scrollMapEditor(WPARAM wParam, LPARAM lParam)
{
	wcsMapEditor->scroll(wParam, lParam);
}

void drawMapEditor(float dt)
{
	// opengl
}


Texture** texs = NULL;
char* strImagePath = NULL;
void btnOpenImageUpdate(HWND hwnd)
{
	const char* path = openFileDlg(true, TEXT("Image Files(*.png, *.jpg)\0*.png;*.jpg\0"));
	if (path)
	{
		int len = strlen(path);
		strImagePath = (char*)calloc(sizeof(char), 1 + len);
		strcpy(strImagePath, path);
	}
}

void methodTileUpdate(float dt)
{
	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//setRGBA(0, 0, 0, 1);
	//fillEllipse(30, 30, 50, 20, 0);

	if (strImagePath)
	{
		int numX = getWndInt(hEbOpenImage[0]);
		int numY = getWndInt(hEbOpenImage[1]);
		if (texs)
		{
			int i, j = numX * numY;
			for (i = 0; i < j; i++)
			{
				freeImage(texs[i]);			
			}
			
			free(texs);
		}
		texs = createDivideImage(numX, numY, strImagePath);

		free(strImagePath);
		strImagePath = NULL;
	}

	
	if (texs == NULL)
		return;

	int w = texs[0]->width;
	int h = texs[1]->height;
	int numX = getWndInt(hEbOpenImage[0]);
	int numY = getWndInt(hEbOpenImage[1]);

	setRGBA(1, 1, 1, 1);
	for (int j = 0; j < numY; j++)
	{
		for (int i = 0; i < numX; i++)
			drawImage(texs[numX * j + i], w * i, h * j, TOP | LEFT);
	}
	
	int num = numX * numY;
	for (int i = 0; i < num; i++)
	{
		glLineWidth(2);
		drawRect(w * (i % numX), h * (i / numX), w, h,0);
	}

	if (selectedTileIndex != -1)//선택.
	{
		setRGBA(1, 0, 0, 1);
		glLineWidth(2);
		drawRect(w * (selectedTileIndex % numX), h * (selectedTileIndex / numX), w, h, 0);
		setRGBA(1, 1, 1, 1);
	}
}


void methodMapUpdate(float dt)
{
	// world map

	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	static Texture* t = createImage("assets/ex.png");
//	drawImage(t, 0, 0, TOP | LEFT);
//	setRGBA(1, 0, 1, 1);
//	drawCircle(55, 55, 20, 3);
//	setRGBA(1, 1, 1, 1);

	
	if (texs == NULL)
		return;

	int w = texs[0]->width;
	int h = texs[1]->height;
	int numX = getWndInt(hEbMapNum[0]);
	int numY = getWndInt(hEbMapNum[1]);

	int x = offMap.x;
	int y = offMap.y;

	for (int j = 0; j < numY; j++)
	{
		for (int i = 0; i < numX; i++)
		{
			MapTile* mt = &mapTile[numX * j + i];
			if (mt->imgIndex != -1)
			{
				setRGBA(1,1,1,1);
				drawImage(texs[mt->imgIndex], x + w * i, y + h * j, TOP | LEFT);
			}

			if (mt->collision == 0)
				setRGBA(1, 0, 0, 0.5f);
			else if (mt->collision == 1)
				setRGBA(1, 1, 0, 0.5f);
			else
				continue;
			fillRect(x + w * i, y + h * j, w, h, 0);
		}
	}


	setRGBA(1, 1, 1, 1);
	int num = numX * numY;
	for (int i = 0; i < num; i++)
	{
		glLineWidth(2);
		drawRect(x + w * (i % numX), y + h * (i / numX), w, h, 0);
	}




}


void methodallDel(HWND hwnd)
{
	int i;
	for (i = 0; i < 2; i++)
	{
		if (btnAllDelCanvas[i] == hwnd)
			break;
	}
	
	
	int numX = getWndInt(hEbMapNum[0]);
	int numY = getWndInt(hEbMapNum[1]);
	mapTile = (MapTile*)malloc(sizeof(MapTile) * numX * numY);
	if (i == 0)
	{
		for (int j = 0; j < numY; j++)
			for (int i = 0; i < numX; i++)
				mapTile[numX * j + i].imgIndex = -1;
	}
	else
	{
		for (int j = 0; j < numY; j++)
			for (int i = 0; i < numX; i++)
				mapTile[numX * j + i].imgIndex = selectedTileIndex;
	}

}



void methodSelectTileUpdate(float dt)
{
	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (texs == NULL)
		return;

	int w = texs[0]->width;
	int h = texs[1]->height;
	int numX = getWndInt(hEbOpenImage[0]);
	int numY = getWndInt(hEbOpenImage[1]);



}

void keyMapEditor(iKeyState stat, iPoint point)
{
	if (texs == NULL)
		return;

	int w = texs[0]->width;
	int h = texs[1]->height;
	int cb = indexWndComboBox(hCbState);
	//if (getKeyStat(keyboard_ctrl))
	//{
	//	printf("on\n");
	//	Pmove = true;
	//}

	if (checkOpenGLPosition(wcsMapEditor->hwndParent, wgTile->gl->hWnd, point))
	{
		//printf("tile(%f,%f)\n", point.x, point.y);

		int numX = getWndInt(hEbOpenImage[0]);
		int numY = getWndInt(hEbOpenImage[1]);

		if (stat == iKeyStateBegan)
		{
			int x = point.x - offTile.x; x /= w;
			int y = point.y - offTile.y; y /= h;
			selectedTileIndex = numX * y + x;
		}
	////	uint32 keyStat = getKeyStat();
	//	if (getKeyDown() & keyboard_ctrl)// move
	//	{
	//		
	//		if (stat == iKeyStateBegan)
	//		{
	//			moving = true;
	//			prevPoint = point;
	//		}
	//		else if (stat == iKeyStateMoved)
	//		{
	//			if (moving)
	//			{
	//				offTile += (point - prevPoint);
	//				prevPoint = point;
	//			}
	//		}
	//		else if (stat == iKeyStateEnded)
	//		{
	//			moving = false;
	//		}
	//	}
	}
	else if (checkOpenGLPosition(wcsMapEditor->hwndParent, wgMap->gl->hWnd, point))
	{
		//printf("Map(%f,%f)\n", point.x, point.y);
		int numX = getWndInt(hEbMapNum[0]);
		int numY = getWndInt(hEbMapNum[1]);
			
		if (cb==0)// move
		{
			if (stat == iKeyStateBegan)
			{
				moving = true;
				prevPoint = point;
			}
			else if (stat == iKeyStateMoved)
			{
				if (moving)
				{
					offMap += (point - prevPoint);
					prevPoint = point;
				}
			}
			else if (stat == iKeyStateEnded)
			{
				moving = false;
			}
		}
		else if (cb == 1)// image
		{
			if (stat == iKeyStateRBegan)
			{
				del = true;
				int x = point.x - offMap.x; x /= w;
				int y = point.y - offMap.y; y /= h;
				MapTile* mt = &mapTile[numX * y + x];
			//	if (mt->imgIndex = selectedTileIndex == NULL)
			//		return;
			
				mt->imgIndex= -1;
			}
			if (stat == iKeyStateBegan)
			{
				moving = true;

				int x = point.x - offMap.x; x /= w;
				int y = point.y - offMap.y; y /= h;
				MapTile* mt = &mapTile[numX * y + x];
				mt->imgIndex = selectedTileIndex;
			}
			else if (stat == iKeyStateMoved)
			{
				if (moving)
				{
					int x = point.x - offMap.x; x /= w;
					int y = point.y - offMap.y; y /= h;
					MapTile* mt = &mapTile[numX * y + x];
					mt->imgIndex = selectedTileIndex;
					
				}
				else if (del)
				{
					int x = point.x - offMap.x; x /= w;
					int y = point.y - offMap.y; y /= h;
					MapTile* mt = &mapTile[numX * y + x];
					if (mt->imgIndex = selectedTileIndex == NULL)
						return;
					mt->imgIndex = -1;

				}
			}
			else if (stat == iKeyStateEnded||stat == iKeyStateREnded)
			{
				moving = false;
				del = false;
			}
		}
		else if (cb == 2)// collision
		{
			int collision = indexWndComboBox(hCbCollision);
			if (stat == iKeyStateBegan)
			{
				moving = true;

				int x = point.x - offMap.x; x /= w;
				int y = point.y - offMap.y; y /= h;
				MapTile* mt = &mapTile[numX * y + x];
				mt->collision = collision;
			
			}
			else if (stat == iKeyStateMoved)
			{
				if (moving)
				{
					int x = point.x - offMap.x; x /= w;
					int y = point.y - offMap.y; y /= h;
					MapTile* mt = &mapTile[numX * y + x];
					mt->collision = collision;
				
				}
			}
			else if (stat == iKeyStateEnded)
			{
				moving = false;
			}
		}
	}
	
}


void btnMapOpenUpdate(HWND hwnd)
{
	//bool open = (hBtnMapOpen[0] == hwnd);
	const char* path = openFileDlg(true, TEXT("Map Files(*.map)\0*.map\0"));
	int i;
	for (i = 0; i < 2; i++)
	{
		if (hBtnMapOpen[i] == hwnd)
			break;
	}
	if (i == 0)
	{
		if (path)
		{
			//loadDataMapEditor(path);
		}
	}
	else
	{
		const char* path = openFileDlg(false, TEXT("Map Files(*.map)\0*.map\0"));
		if (path)
		{
			char _path[256];
			if (strcmp(&path[strlen(path) - 4], ".map") == 0)
				strcpy(_path, path);
			else
				sprintf(_path, "%s.map", path);
			//saveDataMapEditor(path);
		}
	}


}

////iArray* arrayMap;
////MapTile* inputMap;
////void freeMap(void* parm)
////{
////	MapTile* m = (MapTile*)parm;
////	free(m->collision);
////	free(m->imgIndex);
////	free(m->path);
////	free(m);
////}
//
//void loadDataMapEditor(const char* path)
//{
//	//printf("path = %s\n", path);
//	FILE* pf = fopen(path, "rb");
//	loadDataMapEditor(pf);
//	fclose(pf);
//}
////void freeMap(void* parm)
////{
////	MapTile* m = (MapTile*)parm;
////	free(&m->collision);
////	free(&m->imgIndex);
////
////}
//void loadDataMapEditor(FILE* pf)
//{
//	int numX = getWndInt(hEbMapNum[0]);
//	int numY = getWndInt(hEbMapNum[1]);
//	int i, j = numX * numY;
//	mapTile = (MapTile*)malloc(sizeof(MapTile) * numX * numY);
//	for (int j = 0; j < numY; j++)
//		for (int i = 0; i < numX; i++)
//			mapTile[numX * j + i].imgIndex = -1;
//	
//	int tilenumX = getWndInt(hEbOpenImage[0]);
//	int tilenumY = getWndInt(hEbOpenImage[1]);
//
//	arrayMapTile->removeAll();
//	
//	for (i = 0; i < j; i++)
//	{
//		MapTile* map = (MapTile*)malloc(sizeof(MapTile));
//
//		//int len;
//		//fread(&len, sizeof(int), 1, pf);
//		fread(&map->collision, sizeof(int), 1, pf);
//		fread(&map->imgIndex, sizeof(int), 1, pf);
//
//	}
//
//}
//void saveDataMapEditor(const char* path)
//{
//	//printf("path = %s\n", path);
//	FILE* pf = fopen(path, "wb");
//	saveDataMapEditor(pf);
//	fclose(pf);
//
//}
//
//void saveDataMapEditor(FILE* pf)
//{
//	int mapnumX = getWndInt(hEbMapNum[0]);
//	int mapnumY = getWndInt(hEbMapNum[1]);
//	int tilenumX = getWndInt(hEbOpenImage[0]);
//	int tilenumY = getWndInt(hEbOpenImage[1]);
//
//	int i, j = mapnumX * mapnumY;
//
//	
//	for (i = 0; i < j; i++)
//	{
//		MapTile* map = (MapTile*)arrayMapTile->objectAtIndex(i);		
//		
//		fwrite(&map->collision, sizeof(int), 1, pf);
//		fwrite(&map->imgIndex, sizeof(int), 1, pf);
//
//
//	}
//
//
//}
