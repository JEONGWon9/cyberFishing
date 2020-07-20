#pragma once

#include "iStd.h"

#include "iWindow.h"

void loadMapEditor(HWND hwnd);
void freeMapEditor();
void dragMapEditor(WPARAM wParam, LPARAM lParam);
LRESULT colorMapEditor(WPARAM wParam, LPARAM lParam);
void updateMapEditor(WPARAM wParam, LPARAM lParam);
void drawMapEditor(float dt);
void keyMapEditor(iKeyState stat, iPoint point);
void scrollMapEditor(WPARAM wParam, LPARAM lParam);

extern int selectedTileIndex;

struct MapTile
{
	int imgIndex;
	int collision;

};

extern MapTile* mapTile;
void ebMapNumUpdate(HWND hwnd);
extern iPoint prevPoint;
extern iPoint offMap;
extern iPoint offTile;
extern bool moving;
extern bool del;
/*
1. 배열별 넘버링


*/

//void freeMap(int* parm);


//
//void loadDataMapEditor(FILE* pf);
//void loadDataMapEditor(const char* path);
//void saveDataMapEditor(FILE* pf);
//void saveDataMapEditor(const char* path);
//

