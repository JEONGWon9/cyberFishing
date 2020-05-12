#pragma once

#include "iStd.h"

void loadMenu();
void freeMenu();
void drawMenu(float dt);
void keyMenu(iKeyState stat, iPoint point);

// pressAnyKey

// Menu (게임시작 / 옵션 / 게임종료)

// Slot

// Option
// - Credits / Howto

// Gamover
// - Really

// -----------------------------------
// popPress
// -----------------------------------
void createPopPress();
void freePopPress();
void showPopPress(bool show);
void drawPopPress(float dt);
bool keyPopPress(iKeyState stat, iPoint point);

//============================================
//PoopMenu
//============================================

void createPopMenu();
void freePopMenu();
void showPopMenu(bool show);
void drawPopMenu(float dt);
bool keyPopMenu(iKeyState stat, iPoint point);

//============================================
//PopSetting
//============================================

void createPopSetting();
void freePopSetting();
void showPopSetting(bool show);
void drawPopSetting(float dt);
bool keyPopSetting(iKeyState stat, iPoint point);
    