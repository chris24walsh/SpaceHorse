#ifndef GAME_H
#define GAME_H

#include "Allegro_init.h"
#include <vector>
#include "Ship.h"
#include "Planet.h"
#include <sstream>
//#include "Radar.h"

class Game {
public:
	//Members
	std::vector<Ship> ships;
	std::vector<Planet> planets;
	//Radar radar;

	//Allegro objects
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_BITMAP *backgroundSprites[9];
	ALLEGRO_BITMAP *radarBuffer;
	ALLEGRO_BITMAP *radarSprite;
	ALLEGRO_BITMAP *radarDotSprite;
	ALLEGRO_FONT *font;
	ALLEGRO_BITMAP *dockingText;
	ALLEGRO_BITMAP *upgradedText;
	ALLEGRO_BITMAP *buffer;
	ALLEGRO_DISPLAY *display;
	ALLEGRO_DISPLAY_MODE disp_data;

	//Screen vars
	int gridX;
	int gridY;
	int bgX;
	int bgY;
	int numberGrids;// = 1000000; //Number of grids along one side of the square map actually...:)
	int windowWidth, windowHeight, maxX, maxY, screenWidth, screenHeight;
	int backgroundWidth;// = 1920;
	int backgroundHeight;// = 1080;
	int scaledWidth, scaledHeight, scaledOffsetX, scaledOffsetY;
	bool leftPressed, rightPressed, upPressed, downPressed, firePressed;
	int FPS;// = 60
	//Home screen vars
	int homeScreenOption;// = 3; //Choice is single player by default
	//Planet vars
	bool safe;// = true;
	bool dispUpgradeText;// = false;
	//Misc vars
	bool done; //Keeps game looping
	bool paused;// = false;
	bool gameOver;// = false;
	int choice; //Choice of server/client/singleplayer
	int screenMode;
	bool collided;// = true;
	bool hyperDrive;// = false;
	int startTime;
	float oldAngle, newAngle;
	int distance;
	int distanceTravelX;
	int distanceTravelY;
	int x2;// = 3000;
	int y2;// = 3000;
	int hyperSpeed;// = 50;
	bool angleAligned, distanceAligned;// = false;
	int newKey;
	std::stringstream enterCoordinates;
	std::string editText;
	bool textEntered;// = false;
	bool newPlayerConnected;// = false;
	std::string ipAddress;
	//Radar vars
	int radarScreenWidth;
	int radarScreenHeight;
	float radarScale;

	//Methods
	Game();
	~Game();
	void init_game();
	void shutdown_game();
	void abort_game(const char*);
	void update_logic();
	void update_graphics();
	void game_loop();
	void fire();
	void triggerCollision();
	void dock();
	void save_game();
	void load_game();
	void upgrade_weapon();
	void hyperdrive();
	void press_key(ALLEGRO_EVENT);
	void release_key(ALLEGRO_EVENT);
};

#endif