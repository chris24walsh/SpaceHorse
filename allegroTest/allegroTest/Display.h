#ifndef DISPLAY_H
#define DISPLAY_H

#include "Allegro_init.h"
#include "Game.h"
#include "Menu_display.h"
#include "Space_display.h"
#include "Dock_display.h"

class Display
{
private:
	Menu_display menu;
	Space_display space;
	Dock_display dock;
	ALLEGRO_DISPLAY_MODE disp_data;
	ALLEGRO_DISPLAY *display;
	ALLEGRO_BITMAP *shipSprite;
	ALLEGRO_BITMAP *shipSprite1;
	ALLEGRO_BITMAP *shipSprite2;
	ALLEGRO_BITMAP *shipSpriteCurrent;
	ALLEGRO_BITMAP *fireSprite;
	ALLEGRO_BITMAP *backgroundSprite;
	ALLEGRO_BITMAP *planet1;
	ALLEGRO_BITMAP *planet2;
	ALLEGRO_BITMAP *planet3;
	ALLEGRO_BITMAP *planet4;
	ALLEGRO_BITMAP *planet5;
	ALLEGRO_BITMAP *planet6;
	ALLEGRO_BITMAP *planet7;
	ALLEGRO_BITMAP *planet8;
	ALLEGRO_BITMAP *planet9;
	ALLEGRO_BITMAP *dockingText;
	ALLEGRO_BITMAP *upgradedText;
	ALLEGRO_BITMAP *radarSprite;
	ALLEGRO_BITMAP *radarDotSprite;
	ALLEGRO_BITMAP *radarBuffer;
	int numberGrids;
	int windowWidth;
	int windowHeight;
	int maxX; 
	int maxY; 
	int screenWidth; 
	int screenHeight;
	int gridX;
	int gridY;
	int bgX;
	int bgY;
	int radarScreenWidth;
	int radarScreenHeight;
	float radarScale;
	std::string editText;


public:
	Display(void);
	void update(Logic &logic, std::vector<Player> &players, Map &map);
	ALLEGRO_DISPLAY* getDisplay();
	Menu_display& getMenu();
	Space_display& getSpace();
	Dock_display& getDock();
	void displayFail(std::string failMessage);
	~Display(void);
};

#endif