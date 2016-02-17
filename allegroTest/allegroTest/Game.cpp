//#include <enet\enet.h> //Must be included before the allegro library to prevent conflicts
#include "Allegro_init.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <random>
#include <cmath>
#include <string>
#include <vector>

#include "Game.h"
#include "Ship.h"
#include "Planet.h"

using namespace std;

//Object Construtor
Game::Game() {
	FPS = 60;
	numberGrids = 1000000;
	backgroundWidth = 1920;
	backgroundHeight = 1080;
	homeScreenOption = 3;
	safe = true;
	dispUpgradeText = false;
	paused = false;
	gameOver = false;
	collided = true;
	hyperDrive = false;
	x2 = 3000;
	y2 = 3000;
	hyperSpeed = 50;
	angleAligned = distanceAligned = false;
	textEntered = false;
	screenMode = 0;
	leftPressed = rightPressed = upPressed = downPressed = firePressed = false;

	init_game();
};

//Object Destructor
Game::~Game() {
	shutdown_game();
}

void Game::init_game() {
	//Allegro initialisation
    if (!al_init())
        abort_game("Failed to initialize allegro");
    if (!al_install_keyboard())
        abort_game("Failed to install keyboard");
    timer = al_create_timer(1.0 / FPS);
    if (!timer)
        abort_game("Failed to create timer");

	//Initialise image addon
	if(!al_init_image_addon())
		abort_game("Failed to initialise al_init_image_addon");
	
	//Initialise the font addon
	al_init_font_addon(); 
	if (!al_init_ttf_addon())
		abort_game("Failed to initalise al_init_ttf_addon");

	//Initialise primitives addon
	if (!al_init_primitives_addon())
		abort_game("Failed to initialise al_init_primitives_addon");
 
	//Initialise Display
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data); //Set resolution to max
	windowWidth = disp_data.width;
	windowHeight = disp_data.height;
	maxX = windowWidth*numberGrids;
	maxY = windowHeight*numberGrids;
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(windowWidth, windowHeight);
    if (!display)
        abort_game("Failed to create display");

	//Initialise font
	font = al_load_ttf_font("C:/Dev/allegro/Font/pirulen.ttf",25,0 );
	if (!font){
		abort_game("Could not load 'pirulen.ttf'.\n");
	}

	//Create player1
	//Ship player1("c:/dev/allegro/images/shipSprite.png", "c:/dev/allegro/images/shipSprite1.png", "c:/dev/allegro/images/shipSprite2.png", "c:/dev/allegro/images/fireball.png");
	//ships.push_back(player1); //Add player1 to ships

	//Calculate scaling factor
	float sx = float(windowWidth)/backgroundWidth;
	float sy = float(windowHeight)/backgroundHeight;
	float scale = sx<sy?sx:sy;
	scaledWidth = windowWidth * scale;
	scaledHeight = windowHeight * scale;
	scaledOffsetX = (windowWidth - scaledWidth) / 2;
	scaledOffsetY = (windowHeight - scaledHeight) / 2;

	//Load bitmap files
	for (int i=0; i<9; i++) {
		backgroundSprites[i] = al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png");
	}

	//Initialise docking screen
	dockingText = al_load_bitmap("c:/dev/allegro/images/dockingText.png");
	upgradedText = al_load_bitmap("c:/dev/allegro/images/upgradedText.png");
	
	//Radar
	radarBuffer = al_create_bitmap(windowWidth*3, windowHeight*3);
	radarSprite = al_load_bitmap("c:/dev/allegro/images/radar1.png");
	radarDotSprite = al_load_bitmap("c:/dev/allegro/images/radarDot.png");
	radarScreenWidth = 1920;
	radarScreenHeight = 1080;
	radarScale = 0.005;

	//Check if bitmaps loaded properly
	if(!backgroundSprites[0]) abort_game("Failed to load the background image");
	for (int p=0;p<ships.size();p++) {
		if(!ships.at(p).shipSprite) abort_game("Failed to load the shipSprite image");
		if(!ships.at(p).shipSprite1) abort_game("Failed to load the shipSprite1 image");
		if(!ships.at(p).shipSprite2) abort_game("Failed to load the shipSprite2 image");
		if(!ships.at(p).fireSprite[0]) abort_game("Failed to load the fireball image");
	}
	//if(!planets.at(0).planetSprite) abort_game("Failed to load the planets1 image");
	if(!dockingText) abort_game("Failed to load the dockingText image");
	if(!upgradedText) abort_game("Failed to load the upgradedText image");
	if(!radarSprite) abort_game("Failed to load the radar image");
	if(!radarDotSprite) abort_game("Failed to load the radar dot image");
	
	//Initisatise the event queue
	event_queue = al_create_event_queue();
    if (!event_queue) abort_game("Failed to create event queue");
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
}

void Game::abort_game(const char* message)
{
    printf("%s \n", message);
	_getch();
    exit(1);
}

void Game::shutdown_game() {
	//Allegro shutdown
    if (timer) al_destroy_timer(timer);
    if (display) al_destroy_display(display);
	if (backgroundSprites[0]) al_destroy_bitmap(backgroundSprites[0]);
	for (int p=0;p<ships.size();p++) {
		if (ships.at(p).shipSprite) al_destroy_bitmap(ships.at(p).shipSprite);
		if (ships.at(p).shipSprite1) al_destroy_bitmap(ships.at(p).shipSprite1);
		if (ships.at(p).shipSprite2) al_destroy_bitmap(ships.at(p).shipSprite2);
		//if (ships.at(0).shipSpriteCurrent) al_destroy_bitmap(ships.at(0).shipSpriteCurrent);
		for (int i=0; i<MAXFIREBALLS; i++)  if (ships.at(p).fireSprite) al_destroy_bitmap(ships.at(p).fireSprite[i]);
	}
    if (event_queue) al_destroy_event_queue(event_queue);
}

//////////////////////////////////
/////     Game functions     /////

void Game::fire()
{
	firePressed = true;
}

void Game::dock()
{
	for (int i=0; i<planets.size(); i++) {
		if (ships.at(0).canDock) {
			if (screenMode == 2) screenMode = 1; //Undock, if already docked and docking activated
			else if (screenMode == 1) { //Else, if able to dock, do so, stop ship and set it to safe mode
				screenMode = 2;
				ships.at(0).speed = 0;
				safe = true;
			}
		}
	}
}

void Game::triggerCollision() {
	ships.at(0).health--;
}

void Game::upgrade_weapon()
{
	if (screenMode == 2) {
		dispUpgradeText = true;
		for (int i=0; i<MAXFIREBALLS; i++) ships.at(0).fireSprite[i] = al_load_bitmap("c:/dev/allegro/images/fireball2.png");
		ships.at(0).fireHeight = 40;
	}
}

void Game::hyperdrive()
{
	ships.at(0).speed = 0;
	//paused = true;
	hyperDrive = true;
	enterCoordinates.str("");
	editText = "";
	oldAngle = ships.at(0).angle;
	distanceTravelX = abs(abs(x2) - abs(ships.at(0).x));
	distanceTravelY = abs(abs(y2) - abs(ships.at(0).y));
}

void Game::save_game()
{
	ofstream file;
	file.open("save/save_file.dat", ofstream::trunc);

	file << ships.at(0).x << ships.at(0).y << ships.at(0).health;
	file.close();
}

void Game::load_game()
{
	ifstream file;
	file.open("save/save_file.dat", ofstream::in);

	
	file.close();
}

void Game::press_key(ALLEGRO_EVENT e)
{
	switch (screenMode) {
	case 0: //Home mode 1
		{
			if (e.keyboard.keycode == ALLEGRO_KEY_UP) {
				homeScreenOption--;
				if (homeScreenOption < 3) homeScreenOption = 4;
			}
			if (e.keyboard.keycode == ALLEGRO_KEY_DOWN) {
				homeScreenOption++;
				if (homeScreenOption > 4) homeScreenOption = 3;
			}
			if (e.keyboard.keycode == ALLEGRO_KEY_ENTER) {
				if (homeScreenOption==4) { //Quit
					done = true;
					break;
				}
				if (homeScreenOption==2) { //Join server
					screenMode = 3;
					break;
				}
				else { //Host on localhost or play single player
					//setUpHost();
					screenMode = 1;
				}
			}
			break;
		}

	case 1: //Space mode
		{
			if (!hyperDrive && !gameOver) {
				if (e.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					done = true;
				}
				if (e.keyboard.keycode == ALLEGRO_KEY_LEFT) {
					leftPressed = true;
				}
				if (e.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
					rightPressed = true;
				}
				if (e.keyboard.keycode == ALLEGRO_KEY_UP) {
					upPressed = true;
				}
				if (e.keyboard.keycode == ALLEGRO_KEY_DOWN) {
					downPressed = true;
				}
				if (e.keyboard.keycode == ALLEGRO_KEY_F) {
					fire();
				}
				if (e.keyboard.keycode == ALLEGRO_KEY_D) {
					dock();
				}
				if (e.keyboard.keycode == ALLEGRO_KEY_P) {
					if (paused && !gameOver) paused = false;
					else paused = true;
				}
				if (e.keyboard.keycode == ALLEGRO_KEY_H) {
					hyperdrive();
				}
				if (e.keyboard.keycode == ALLEGRO_KEY_F1) {
					save_game();
				}
			}
			if (hyperDrive) {
				if (e.keyboard.keycode >= 27 && e.keyboard.keycode <= 36) {
					newKey = e.keyboard.keycode;
					newKey -= 27;
					enterCoordinates << newKey;
					editText = enterCoordinates.str();
				}
				if (e.keyboard.keycode == 75) {
					enterCoordinates << " ";
					editText = enterCoordinates.str();
				}
				if (e.keyboard.keycode == 63) {
					if (editText.length() > 0)
						editText.pop_back();
				}
				else if (e.keyboard.keycode == ALLEGRO_KEY_ENTER) {
					if (!editText.empty()) {
						textEntered = true;
						x2 = atoi(strtok((char*)editText.c_str(), " "));
						y2 = atoi(strtok(NULL, " "));
						newAngle = atan2( (y2 - ships.at(0).y),  (x2 - ships.at(0).x) );
						if (newAngle<0) newAngle += 3.14*2;
						distanceTravelX = abs(abs(x2) - abs(ships.at(0).x));
						distanceTravelY = abs(abs(y2) - abs(ships.at(0).y));
						if (x2==ships.at(0).x && y2==ships.at(0).y) hyperDrive = paused = false;
					}
					else hyperDrive = paused = false;
				}
			}
			if (gameOver) {
				if (e.keyboard.keycode == ALLEGRO_KEY_ENTER || e.keyboard.keycode == ALLEGRO_KEY_ESCAPE) done = true;
			}
			break;
		}
		
	case 2: //Docking mode
		{
			if (e.keyboard.keycode == ALLEGRO_KEY_W) {
				upgrade_weapon();
			}
			if (e.keyboard.keycode == ALLEGRO_KEY_D) {
				dock();
			}
			break;
		}
	}
}

void Game::release_key(ALLEGRO_EVENT e)
{
	switch (screenMode) {
	case 1: //Space mode
		{
			if (e.keyboard.keycode == ALLEGRO_KEY_LEFT) {
				leftPressed = false;
			}
			if (e.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
				rightPressed = false;
			}
			if (e.keyboard.keycode == ALLEGRO_KEY_UP) {
				upPressed = false;
			}
			if (e.keyboard.keycode == ALLEGRO_KEY_DOWN) {
				downPressed = false;
			}
			if (e.keyboard.keycode == ALLEGRO_KEY_F) {
				firePressed = false;
				ships.at(0).fireCycle = 10;
			}
			break;
		}
	}
}

void Game::update_logic()
{
	if (!paused) {
		//Rotate clockwise or anti-clockwise
		if (leftPressed) {
			ships.at(0).angle-=0.05;
		}
		if (rightPressed) {
			ships.at(0).angle+=0.05;
		}
		if (ships.at(0).angle > 6.25) ships.at(0).angle = 0;
		if (ships.at(0).angle < 0) ships.at(0).angle = 6.25;

		//Cycle between different rocket sprites, to give effect of rocket blasting
		for (int p=0;p<ships.size();p++) {
			if (ships.at(p).speed>0) {
				if (ships.at(p).flipflop<5) ships.at(p).shipSpriteCurrent = ships.at(p).shipSprite1;
				else if (ships.at(p).flipflop>=5) ships.at(p).shipSpriteCurrent = ships.at(p).shipSprite2;
				ships.at(p).flipflop++;
				if (ships.at(p).flipflop==10) ships.at(p).flipflop=0;
			}
			else ships.at(p).shipSpriteCurrent = ships.at(p).shipSprite;
		}

		//Increase or decrease ships.at(0).speed
		if (upPressed) {
			if (ships.at(0).speed<ships.at(0).maxSpeed) ships.at(0).speed += 1;
		}
		else if (downPressed) {
			if (ships.at(0).speed>0) ships.at(0).speed -= 1;
		}

		//Resolve translations for x and y axis
		ships.at(0).speedX = ships.at(0).speed * cos(ships.at(0).angle);
		ships.at(0).speedY = ships.at(0).speed * sin(ships.at(0).angle);

		//Apply translation to ship
		//Restrict map in X axis (can't travel below 0, above maxX)
		if ((ships.at(0).x<ships.at(0).width/2 && ships.at(0).speedX<0) || (ships.at(0).x>maxX-(ships.at(0).width/2) && ships.at(0).speedX>0)) ; //Can't travel
		else ships.at(0).x += ships.at(0).speedX;
	
		//Restrict map in Y axis (can't travel below 0, above maxY)
		if ((ships.at(0).y<ships.at(0).height/2 && ships.at(0).speedY<0) || (ships.at(0).y>maxY-(ships.at(0).height/2) && ships.at(0).speedY>0)) ; //Can't travel
		else ships.at(0).y += ships.at(0).speedY;

		//Set which grid the ship is currently in
		gridX = int(float(ships.at(0).x)/windowWidth)*windowWidth;
		gridY = int(float(ships.at(0).y)/windowHeight)*windowHeight;

		//Set background coordinates
		bgX = ships.at(0).x - windowWidth/2;
		bgY = ships.at(0).y - windowHeight/2;
	
		//Set fireball position
		for (int p=0;p<ships.size();p++) {
			for (int i=0; i<MAXFIREBALLS; i++) ships.at(p).fireX[i] += ships.at(p).fireSpeed * cos(ships.at(p).fireAngle[i]);
			for (int i=0; i<MAXFIREBALLS; i++) ships.at(p).fireY[i] += ships.at(p).fireSpeed * sin(ships.at(p).fireAngle[i]);
		}

		//Set fire cycle
		if (firePressed) {
			if (ships.at(0).fireCycle >= 10) {
				int x = ships.at(0).fireballNumber++; //Cycled through available fireballs (having a high upper threshold of fireballs prevents onscreen fireballs being recycled)
				ships.at(0).fireX[x] = ships.at(0).x;
				ships.at(0).fireY[x] = ships.at(0).y;
				ships.at(0).fireAngle[x] = ships.at(0).angle;
				if (ships.at(0).fireballNumber>=MAXFIREBALLS){
					ships.at(0).fireballNumber = 0;
				}
				ships.at(0).fireCycle = 0;
			}
			ships.at(0).fireCycle++;
		}
	
		//Check if near docking station, and if so allow docking
		int proxX, proxY;
		for (int i=0; i<planets.size(); i++) {
			proxX = abs(ships.at(0).x - planets.at(i).x);
			proxY = abs(ships.at(0).y - planets.at(i).y);
			if (planets.at(i).canDock)
				if (proxX<planets.at(i).width/2 && proxY<planets.at(i).height/2) {
					ships.at(0).canDock = true;
					break;
				}
			else ships.at(0).canDock = false;
		}

		//Check for gameover
		if (ships.at(0).health <= 0) {
			gameOver = true;
			ships.at(0).speed = 0;
			//paused = true;
		}
	}

	if (hyperDrive && textEntered) {
		if (!angleAligned) { //Get angle right first
			//Decide if better to rotate clockwise or anticlockwise
			if (newAngle > ships.at(0).angle) {
				if (newAngle - ships.at(0).angle < 3.14) ships.at(0).angle += 0.05;
				else ships.at(0).angle -= 0.05;
			}
			else {
				if (ships.at(0).angle - newAngle > 3.14) ships.at(0).angle += 0.05;
				else ships.at(0).angle -= 0.05;
			}
			if (ships.at(0).angle > 6.25) ships.at(0).angle = 0; //Keep angle under 6.28
			if (ships.at(0).angle < 0) ships.at(0).angle = 6.25; //Keep angle above 0
			if (abs(abs(ships.at(0).angle) - abs(newAngle)) < 0.05) { //Check for nearness then align angle exactly
				ships.at(0).angle = newAngle;
				angleAligned = true;
			}
		}
		if (angleAligned) { //Then check distance
			ships.at(0).x += ships.at(0).maxSpeed * hyperSpeed * cos(ships.at(0).angle);
			ships.at(0).y += ships.at(0).maxSpeed * hyperSpeed * sin(ships.at(0).angle);
			distanceTravelX -= ships.at(0).maxSpeed * hyperSpeed * cos(ships.at(0).angle);
			distanceTravelY -= ships.at(0).maxSpeed * hyperSpeed * sin(ships.at(0).angle);
			//if ( abs(abs(ships.at(0).x) - abs(x2)) < 1000 && abs(abs(ships.at(0).y) - abs(y2)) < 1000) { //Check for nearness, then align distance exactly
			if (distanceTravelX <= 1000 || distanceTravelY <= 1000) {
				ships.at(0).x = x2;
				ships.at(0).y = y2;
				paused = hyperDrive = angleAligned = textEntered = false; //Reset flags
			}
		}
		//Set which grid the ship is currently in
		gridX = int(float(ships.at(0).x)/windowWidth)*windowWidth;
		gridY = int(float(ships.at(0).y)/windowHeight)*windowHeight;

		//Set background coordinates
		bgX = ships.at(0).x - windowWidth/2;
		bgY = ships.at(0).y - windowHeight/2;
	}
}

void Game::update_graphics()
{
	switch (screenMode) {

	//Home screen mode
	case 0:
		{
		al_clear_to_color(al_map_rgb(30,30,30));
		int c1R, c1G, c1B, c2R, c2G, c2B, c3R, c3G, c3B, c4R, c4G, c4B;
		c1R = c1G = c1B = c2R = c2G = c2B = c3R = c3G = c3B = c4R = c4G = c4B = 256;
		if (homeScreenOption == 1) c1R = c1G = c1B = 190;
		if (homeScreenOption == 2) c2R = c2G = c2B = 190;
		if (homeScreenOption == 3) c3R = c3G = c3B = 190;
		if (homeScreenOption == 4) c4R = c4G = c4B = 190;
		al_draw_text(font, al_map_rgb(c1R,c1G,c1B), windowWidth*0.5, windowHeight*0.2, ALLEGRO_ALIGN_CENTRE, "HOST MULTIPLAYER GAME");
		al_draw_text(font, al_map_rgb(c2R,c2G,c2B), windowWidth*0.5, windowHeight*0.4, ALLEGRO_ALIGN_CENTRE, "JOIN MULTIPLAYER GAME");
		al_draw_text(font, al_map_rgb(c3R,c3G,c3B), windowWidth*0.5, windowHeight*0.6, ALLEGRO_ALIGN_CENTRE, "SINGLE PLAYER");
		al_draw_text(font, al_map_rgb(c4R,c4G,c4B), windowWidth*0.5, windowHeight*0.8, ALLEGRO_ALIGN_CENTRE, "QUIT");

		
		break;
		}

	//Space mode
	case 1:
		{
		//Clear display first
		al_clear_to_color(al_map_rgb(0, 0, 0));
		
		//Remember that all sprites drawn to the display must be drawn relative to the the current background coordinates (the camera), bgX and bgY - so subtract their x and y coordinates from bgX and bgY
		
		//Draw all 9 of the background sprites, tiled around, and including, the current grid
		int backX = gridX-windowWidth; //X coordinate of top left corner tile of the 9 tiles
		int backY = gridY-windowHeight; //Y coordinate of top left corner tile of the 9 tiles
		for (int x=0; x<3; x++)
			for (int y=0; y<3; y++)
				al_draw_bitmap(backgroundSprites[x+y],backX + windowWidth*x - bgX, backY + windowHeight*y - bgY,0);
		
		//Draw planets
		for (int i=0; i<planets.size(); i++)
			al_draw_scaled_rotated_bitmap(planets.at(i).planetSprite, planets.at(i).width/2, planets.at(i).height/2, planets.at(i).x-bgX, planets.at(i).y-bgY, planets.at(i).planetScale, planets.at(i).planetScale, 0, 0); //Draw planets only if their coordinates exist within current screen
		//Draw sprites for each Ship object
		for (int p=ships.size()-1;p>=0;p--) {
			//Draw fireballs
			for (int i=0; i<MAXFIREBALLS; i++)
				al_draw_tinted_rotated_bitmap(ships.at(p).fireSprite[i], al_map_rgb(255,255,255), ships.at(p).fireWidth/2, ships.at(p).fireHeight/2, ships.at(p).fireX[i]-bgX, ships.at(p).fireY[i]-bgY, ships.at(p).fireAngle[i], 0);
			//Draw ships
			al_draw_rotated_bitmap(ships.at(p).shipSpriteCurrent, ships.at(p).width/2, ships.at(p).height/2, ships.at(p).x-bgX, ships.at(p).y-bgY, ships.at(p).angle, 0); 
		}

		//Draw bordering rectangle //Not necessary with current gigantic map, but useful to debug a small bounded area
		//al_draw_rectangle(0-bgX, 0-bgY, maxX-bgX, maxY-bgY, al_map_rgb(255, 255, 255), 10);
		
		//Draw stats to screen
		stringstream s1, s2;
		s1 << "Health: " << ships.at(0).health;
		s2 << "Coordinates: " << ships.at(0).x << " , " << ships.at(0).y;
		string str1 = s1.str();
		string str2 = s2.str();
		al_draw_text(font, al_map_rgb(255,255,255), windowWidth*0.05, windowHeight*0.1, 0, str1.c_str());
		al_draw_text(font, al_map_rgb(255,255,255), windowWidth*0.05, windowHeight*0.9, 0, str2.c_str());

		//Draw radar
		al_draw_scaled_rotated_bitmap(radarSprite, radarScreenWidth/2, radarScreenHeight/2, windowWidth*0.85, windowHeight*0.15, 0.2, 0.2, 0, 0);
		//Planets on radar
		for (int i=0; i<planets.size(); i++) {
			float rX = windowWidth*0.85 - (ships.at(0).x - planets.at(i).x) * radarScale;
			float rY = windowHeight*0.15 - (ships.at(0).y - planets.at(i).y) * radarScale;
			if ((rX > windowWidth*0.85 - windowWidth*0.1) && (rX < windowWidth*0.85 + windowWidth*0.1) && (rY > windowHeight*0.15 - windowHeight*0.1) && (rY < windowHeight*0.15 + windowHeight*0.1))
				al_draw_rotated_bitmap(radarDotSprite, 2.5, 2.5, rX, rY, 0, 0); //Draw planets only if their coordinates exist within current screen
		}
		//Ships on radar
		for (int p=ships.size()-1;p>=0;p--) {
			float rX = windowWidth*0.85 - (ships.at(0).x - ships.at(p).x) * radarScale;
			float rY = windowHeight*0.15 - (ships.at(0).y - ships.at(p).y) * radarScale;
			if ((rX > windowWidth*0.85 - windowWidth*0.1) && (rX < windowWidth*0.85 + windowWidth*0.1) && (rY > windowHeight*0.15 - windowHeight*0.1) && (rY < windowHeight*0.15 + windowHeight*0.1))
				al_draw_rotated_bitmap(radarDotSprite, 2.5, 2.5, rX, rY, 0, 0);
		}

		//Game over
		if (gameOver) al_draw_text(font, al_map_rgb(250, 0, 20), windowWidth*0.5, windowHeight*0.5, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
		
		//Hyper Drive input
		if (hyperDrive){
			al_draw_text(font, al_map_rgb(255, 255, 255), windowWidth*0.5, windowHeight*0.35, ALLEGRO_ALIGN_CENTRE, "Enter coordinates: ");
			al_draw_text(font, al_map_rgb(255, 255, 255), windowWidth*0.5, windowHeight*0.4, ALLEGRO_ALIGN_CENTRE, editText.c_str());
		}

		break;
		}

	//Docking mode
	case 2:
		{
		al_clear_to_color(al_map_rgb(25,0,25));
		al_draw_rotated_bitmap(ships.at(0).shipSprite, ships.at(0).width/2, ships.at(0).height/2, 250, 250, 0, 0);
		al_draw_rotated_bitmap(ships.at(0).fireSprite[0], ships.at(0).fireHeight/2, ships.at(0).fireWidth/2, 250, 250, 0, 0);
		al_draw_bitmap(dockingText, 0, 0, 0);
		if (dispUpgradeText) al_draw_bitmap(upgradedText, 0, windowHeight/2, 0);
		
		break;
		}

	//Join server mode
	case 3:
		{
		al_clear_to_color(al_map_rgb(30,30,30));
		al_draw_text(font, al_map_rgb(255,255,255), windowWidth*0.5, windowHeight*0.4, ALLEGRO_ALIGN_CENTRE, "ENTER SERVER IP:");
		if (ipAddress.empty()) al_draw_text(font, al_map_rgb(50,50,50), windowWidth*0.5, windowHeight*0.5, ALLEGRO_ALIGN_CENTRE, "localhost");
		else al_draw_text(font, al_map_rgb(255,255,255), windowWidth*0.5, windowHeight*0.5, ALLEGRO_ALIGN_CENTRE, ipAddress.c_str());
		
		break;
		}
	}
}

void Game::game_loop()
{
    bool redraw = true;
    al_start_timer(timer);
 
	done = false;
    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
 
        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
            update_logic();
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            press_key(event);
        }
		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			release_key(event);
		}

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            update_graphics(); //Redraw background and sprites
			al_flip_display();
        }
    }
}