#pragma once

#include "iStd.h"

void loadSelectMap();
void freeSelectMap();
void drawSelectMap(float dt);
void keySelectMap(iKeyState stat, iPoint point);

// -----------------------------------
// selectpoint
// -----------------------------------

void createPopPoint();
void freePopPoint();
void showPopPoint(bool show);
void drawPopPoint(float dt);
bool keyPopPoint(iKeyState stat, iPoint point);
    