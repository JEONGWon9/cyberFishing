#include "Game.h"

#include "Loading.h"
#include "Intro.h"
#include "Menu.h"
#include "Ending.h"
#include "SelectMap.h"
#include "SeaBoat.h"
#include "Fishing.h"

void loadGame()
{
#if 0
	loadIntro();
	gameState = gs_intro;
#elif 1
	loadMenu();
	gameState = gs_menu;
#elif 0
	loadSeaBoat();
	gameState = gs_seaboat;
#elif 0
	loadFishing();
	gameState = gs_fishing;
#endif

	AudioInfo ai[4] = {
		{ "assets/snd/0.wav", false, 0.6f },
		{ "assets/snd/1.wav", false, 0.6f },
		{ "assets/snd/2.wav", true, 1.0f },
		{ "assets/snd/3.wav", true, 1.0f },
	};
	loadAudio(ai, 4);

	audioPlay(2);

}

void freeGame()
{
	switch (gameState) {
	case gs_intro:		freeIntro(); break;
	case gs_menu:		freeMenu(); break;
	case gs_ending:		freeEnding(); break;
	case gs_selectmap:  freeSelectMap(); break;
	case gs_seaboat:	freeSeaBoat(); break;
	case gs_fishing:	freeFishing(); break;
	
	}
	freeAudio();

	freeSeaBoat();
}

void drawGame(float dt)
{
	switch (gameState) {
	case gs_intro:		drawIntro(dt); break;
	case gs_menu:		drawMenu(dt); break;	
	case gs_ending:		drawEnding(dt); break;
	case gs_selectmap:  drawSelectMap(dt); break;
	case gs_seaboat:	drawSeaBoat(dt); break;
	case gs_fishing:	drawFishing(dt); break;
	}

	drawLoading(dt);
}

void keyGame(iKeyState stat, iPoint point)
{
	if (keyLoading(stat, point))
		return;

	switch (gameState) {
	case gs_intro:		keyIntro(stat, point); break;
	case gs_menu:		keyMenu(stat, point); break;	
	case gs_ending:		keyEnding(stat, point); break;
	case gs_selectmap:  keySelectMap(stat, point); break;
	case gs_seaboat:	keySeaBoat(stat, point); break;	
	case gs_fishing:	keyFishing(stat, point); break;
	}
}
