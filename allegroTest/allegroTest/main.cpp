#include <enet\enet.h> //Must be included before the allegro library to prevent conflicts
#include <allegro5/allegro5.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <random>
#include <cmath>
#include <string>

#include <vector>
#include "Ship.h"
#include "Planet.h"

using namespace std;

////////////////////////////////////
/////     GLOBAL VARIABLES     /////

//Allegro objects
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *backgroundSprite1 = NULL;
ALLEGRO_BITMAP *backgroundSprite2 = NULL;
ALLEGRO_BITMAP *backgroundSprite3 = NULL;
ALLEGRO_BITMAP *backgroundSprite4 = NULL;
ALLEGRO_BITMAP *backgroundSprite5 = NULL;
ALLEGRO_BITMAP *backgroundSprite6 = NULL;
ALLEGRO_BITMAP *backgroundSprite7 = NULL;
ALLEGRO_BITMAP *backgroundSprite8 = NULL;
ALLEGRO_BITMAP *backgroundSprite9 = NULL;
ALLEGRO_BITMAP *radarBuffer = NULL;
ALLEGRO_BITMAP *radarSprite = NULL;
ALLEGRO_BITMAP *radarDotSprite = NULL;
ALLEGRO_FONT *font;
ALLEGRO_BITMAP *dockingText = NULL;
ALLEGRO_BITMAP *upgradedText = NULL;
ALLEGRO_BITMAP *buffer = NULL;
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_DISPLAY_MODE disp_data;

//Screen vars
int gridX = 0;
int gridY = 0;
int bgX = 0;
int bgY = 0;
int numberGrids = 1000000; //Number of grids along one side of the square map actually...:)
int windowWidth, windowHeight, maxX, maxY, screenWidth, screenHeight;
int backgroundWidth = 1920;
int backgroundHeight = 1080;
int scaledWidth, scaledHeight, scaledOffsetX, scaledOffsetY;
bool leftPressed, rightPressed, upPressed, downPressed, firePressed;
int FPS = 60;

//Home screen vars
int homeScreenOption = 3;

//Planet vars
bool safe = true;
bool dispUpgradeText = false;

//Network vars
bool hostSet = false;
bool connected = false;
ENetHost * host; //This machine
ENetPeer * peer; //Remote machine
ENetEvent event;
int wait = 0;

//Misc vars
bool done; //Keeps game looping
bool paused = false;
bool gameOver = false;
int choice; //Choice of server/client/singleplayer
int screenMode = 0;
bool collided = true;
bool hyperDrive = false;
int startTime = 0;
float oldAngle, newAngle;
int distance;
int distanceTravelX = 0;
int distanceTravelY = 0;
int x2 = 3000;
int y2 = 3000;
int hyperSpeed = 50;
bool angleAligned, distanceAligned = false;
int newKey;
stringstream enterCoordinates;
string editText;
bool textEntered = false;
bool newPlayerConnected = false;
string ipAddress;


//Radar vars
int radarScreenWidth;
int radarScreenHeight;
float radarScale;




/////////////////////////////////////////
/////     Function Declarations     /////

//Program control functions declaration
void abort_game(const char*);
void init();
void shutdown();

//Game Functions declaration
void fire();
void triggerCollision();
void dock();
void upgrade_weapon();
void hyperdrive();
void press_key(ALLEGRO_EVENT);
void release_key(ALLEGRO_EVENT);
void update_logic();
void update_graphics();
void game_loop();

//Network Functions declaration
void setUpHost();


//////////////////////////////////
/////     Global Objects     /////

vector<Ship> players;
vector<Planet> planets;


/////////////////////////////////
/////     Main function     /////

int main(int argc, char* argv[])
{
    init();
    game_loop();
    shutdown();
}


//////////////////////////////////////////////
/////      Program control functions     /////

void abort_game(const char* message)
{
    printf("%s \n", message);
	_getch();
    exit(1);
}
 
void init()
{
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
 
	//Setup Display
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data); //Set resolution to max
	//windowWidth = backgroundWidth;
	//windowHeight = backgroundHeight;
	//windowWidth = 700;
	//windowHeight = 700;
	windowWidth = disp_data.width;
	windowHeight = disp_data.height;
	maxX = windowWidth*numberGrids;
	maxY = windowHeight*numberGrids;
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(windowWidth, windowHeight);
    if (!display)
        abort_game("Failed to create display");

	font = al_load_ttf_font("C:/Dev/allegro/Font/pirulen.ttf",25,0 );
	
	if (!font){
		abort_game("Could not load 'pirulen.ttf'.\n");
	}

	//Create player1
	Ship player1("c:/dev/allegro/images/shipSprite.png", "c:/dev/allegro/images/shipSprite1.png", "c:/dev/allegro/images/shipSprite2.png", "c:/dev/allegro/images/fireball.png");
	players.push_back(player1);

	//Calculate scaling factor
	float sx = float(windowWidth)/backgroundWidth;
	float sy = float(windowHeight)/backgroundHeight;
	float scale = sx<sy?sx:sy;
	scaledWidth = windowWidth * scale;
	scaledHeight = windowHeight * scale;
	scaledOffsetX = (windowWidth - scaledWidth) / 2;
	scaledOffsetY = (windowHeight - scaledHeight) / 2;

	//Load bitmap files
	//buffer = al_create_bitmap(windowWidth, windowHeight);
	//buffer = al_create_bitmap(maxX, maxY);
	backgroundSprite1 = al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png"); //Load background image
	backgroundSprite2 = al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png"); //Load background image
	backgroundSprite3 = al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png"); //Load background image
	backgroundSprite4 = al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png"); //Load background image
	backgroundSprite5 = al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png"); //Load background image
	backgroundSprite6 = al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png"); //Load background image
	backgroundSprite7 = al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png"); //Load background image
	backgroundSprite8 = al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png"); //Load background image
	backgroundSprite9 = al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png"); //Load background image
	
	for (int p=0;p<players.size();p++) {
		/*players.at(p).shipSprite = al_load_bitmap("c:/dev/allegro/images/shipSprite.png"); //Stationary ship sprite
		players.at(p).shipSprite1 = al_load_bitmap("c:/dev/allegro/images/shipSprite1.png"); //Moving ship sprite 1
		players.at(p).shipSprite2 = al_load_bitmap("c:/dev/allegro/images/shipSprite2.png"); // Moving ship sprite 2
		players.at(p).shipSpriteCurrent = players.at(p).shipSprite;
		for (int i=0; i<MAXFIREBALLS; i++) players.at(p).fireSprite[i]= al_load_bitmap("c:/dev/allegro/images/fireball.png");*/
	}

	//Set a known random nonce before seeding planets
	srand(6);
	Planet planet1("c:/dev/allegro/images/sun.png", 0);
	Planet planet2("c:/dev/allegro/images/mercury.png", 1);
	Planet planet3("c:/dev/allegro/images/venus.png", 2);
	Planet planet4("c:/dev/allegro/images/earth.png", 3);
	Planet planet5("c:/dev/allegro/images/mars.png", 4);
	Planet planet6("c:/dev/allegro/images/jupiter.png", 13);
	Planet planet7("c:/dev/allegro/images/saturn.png", 24);
	Planet planet8("c:/dev/allegro/images/neptune.png", 49);
	Planet planet9("c:/dev/allegro/images/uranus.png", 76);
	planets.push_back(planet1);
	planets.push_back(planet2);
	planets.push_back(planet3);
	planets.push_back(planet4);
	planets.push_back(planet5);
	planets.push_back(planet6);
	planets.push_back(planet7);
	planets.push_back(planet8);
	planets.push_back(planet9);
	cout << planets.at(3).x << " " << planets.at(3).y << endl;
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
	if(!backgroundSprite1) abort_game("Failed to load the background image");
	for (int p=0;p<players.size();p++) {
		if(!players.at(p).shipSprite) abort_game("Failed to load the shipSprite image");
		if(!players.at(p).shipSprite1) abort_game("Failed to load the shipSprite1 image");
		if(!players.at(p).shipSprite2) abort_game("Failed to load the shipSprite2 image");
		if(!players.at(p).fireSprite[0]) abort_game("Failed to load the fireball image");
	}
	if(!planets.at(0).planetSprite) abort_game("Failed to load the planets1 image");
	cout << planets.at(0).x << " " << planets.at(0).y << endl;
	//if(!planets[1]) abort_game("Failed to load the planets2 image");
	//if(!planets[2]) abort_game("Failed to load the planets3 image");
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
 
	//Docking station initialisation
	//Random seed the planetss throughout the map
	//float distance[planets.size()] = {0, 1, 2, 3, 4, 13, 24, 49, 76};
	/*srand(6); //srand(time(NULL));
	for (int i=0; i<planets.size(); i++) {
		float angle = float(6.25)*(rand()%100)/100;
		planets.at(i).x = planets.at(i).distance * 10 * planets.at(i).planetScale * cos(angle) + 500000;
		planets.at(i).y = planets.at(i).distance * 10 * planets.at(i).planetScale * sin(angle) + 500000;
	}*/

	//Initialise the network library
	if (enet_initialize () != 0)
    {
        abort_game("An error occurred while initializing ENet.\n");
    }

	
}
 
void shutdown()
{
	//Allegro shutdown
    if (timer) al_destroy_timer(timer);
    if (display) al_destroy_display(display);
	if (backgroundSprite1)	al_destroy_bitmap(backgroundSprite1);
	for (int p=0;p<players.size();p++) {
		if (players.at(p).shipSprite) al_destroy_bitmap(players.at(p).shipSprite);
		if (players.at(p).shipSprite1) al_destroy_bitmap(players.at(p).shipSprite1);
		if (players.at(p).shipSprite2) al_destroy_bitmap(players.at(p).shipSprite2);
		//if (players.at(0).shipSpriteCurrent) al_destroy_bitmap(players.at(0).shipSpriteCurrent);
		for (int i=0; i<MAXFIREBALLS; i++)  if (players.at(p).fireSprite) al_destroy_bitmap(players.at(p).fireSprite[i]);
	}
    if (event_queue) al_destroy_event_queue(event_queue);

	//Networking shutdown
	enet_deinitialize();
	enet_host_destroy(host);
}


//////////////////////////////////
/////     Game functions     /////

void fire()
{
	firePressed = true;
}

void dock()
{
	for (int i=0; i<planets.size(); i++) {
		if (planets.at(i).canDock) {
			if (screenMode == 2) screenMode = 1; //Undock, if already docked and docking activated
			else if (screenMode == 1) { //Else, if able to dock, do so, stop ship and set it to safe mode
				screenMode = 2;
				players.at(0).speed = 0;
				safe = true;
			}
		}
	}
}

void triggerCollision() {
	players.at(0).health--;
}

void upgrade_weapon()
{
	if (screenMode == 2) {
		dispUpgradeText = true;
		for (int i=0; i<MAXFIREBALLS; i++) players.at(0).fireSprite[i] = al_load_bitmap("c:/dev/allegro/images/fireball2.png");
		players.at(0).fireHeight = 40;
	}
}

void hyperdrive()
{
	players.at(0).speed = 0;
	paused = true;
	hyperDrive = true;
	enterCoordinates.str("");
	editText = "";
	oldAngle = players.at(0).angle;
	distanceTravelX = abs(abs(x2) - abs(players.at(0).x));
	distanceTravelY = abs(abs(y2) - abs(players.at(0).y));
}

void press_key(ALLEGRO_EVENT e)
{
	switch (screenMode) {
	case 0: //Home mode
		{
			if (e.keyboard.keycode == ALLEGRO_KEY_UP) {
				homeScreenOption--;
				if (homeScreenOption < 1) homeScreenOption = 4;
			}
			if (e.keyboard.keycode == ALLEGRO_KEY_DOWN) {
				homeScreenOption++;
				if (homeScreenOption > 4) homeScreenOption = 1;
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
					setUpHost();
					screenMode = 1;
				}
			}
			break;
		}

	case 1: //Space mode
		{
			if (!hyperDrive) {
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
				else if (e.keyboard.keycode == ALLEGRO_KEY_ENTER) {
					textEntered = true;
					x2 = atoi(strtok((char*)editText.c_str(), " "));
					y2 = atoi(strtok(NULL, " "));
					newAngle = atan2( (y2 - players.at(0).y),  (x2 - players.at(0).x) );
					if (newAngle<0) newAngle += 3.14*2;
					distanceTravelX = abs(abs(x2) - abs(players.at(0).x));
					distanceTravelY = abs(abs(y2) - abs(players.at(0).y));
					if (x2==players.at(0).x && y2==players.at(0).y) hyperDrive = paused = false;
				}
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

	case 3: //Enter server IP
		{
			if (e.keyboard.keycode >= 27 && e.keyboard.keycode <= 36) { //Number
				int newKey = e.keyboard.keycode;
				newKey -= 27;
				stringstream enterIp;
				enterIp << newKey;
				ipAddress += enterIp.str();
			}
			if (e.keyboard.keycode == 73) {
				stringstream enterIp;
				enterIp << ".";
				ipAddress += enterIp.str();
			}
			else if (e.keyboard.keycode == ALLEGRO_KEY_ENTER) {
				cout << ipAddress;
				setUpHost();
				screenMode = 1;
			}
			else cout << e.keyboard.keycode << endl;
		}
	}
}

void release_key(ALLEGRO_EVENT e)
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
				players.at(0).fireCycle = 10;
			}
			break;
		}
	}
}

void update_logic()
{
	if (!paused) {
		//Rotate clockwise or anti-clockwise
		if (leftPressed) {
			players.at(0).angle-=0.05;
		}
		if (rightPressed) {
			players.at(0).angle+=0.05;
		}
		if (players.at(0).angle > 6.25) players.at(0).angle = 0;
		if (players.at(0).angle < 0) players.at(0).angle = 6.25;

		//Cycle between different rocket sprites, to give effect of rocket blasting
		for (int p=0;p<players.size();p++) {
			if (players.at(p).speed>0) {
				if (players.at(p).flipflop<5) players.at(p).shipSpriteCurrent = players.at(p).shipSprite1;
				else if (players.at(p).flipflop>=5) players.at(p).shipSpriteCurrent = players.at(p).shipSprite2;
				players.at(p).flipflop++;
				if (players.at(p).flipflop==10) players.at(p).flipflop=0;
			}
			else players.at(p).shipSpriteCurrent = players.at(p).shipSprite;
		}

		//Increase or decrease players.at(0).speed
		if (upPressed) {
			if (players.at(0).speed<players.at(0).maxSpeed) players.at(0).speed += 1;
		}
		else if (downPressed) {
			if (players.at(0).speed>0) players.at(0).speed -= 1;
		}

		//Resolve translations for x and y axis
		players.at(0).speedX = players.at(0).speed * cos(players.at(0).angle);
		players.at(0).speedY = players.at(0).speed * sin(players.at(0).angle);

		//Apply translation to ship
		//Restrict map in X axis (can't travel below 0, above maxX)
		if ((players.at(0).x<players.at(0).width/2 && players.at(0).speedX<0) || (players.at(0).x>maxX-(players.at(0).width/2) && players.at(0).speedX>0)) ; //Can't travel
		else players.at(0).x += players.at(0).speedX;
	
		//Restrict map in Y axis (can't travel below 0, above maxY)
		if ((players.at(0).y<players.at(0).height/2 && players.at(0).speedY<0) || (players.at(0).y>maxY-(players.at(0).height/2) && players.at(0).speedY>0)) ; //Can't travel
		else players.at(0).y += players.at(0).speedY;

		//Set which grid the ship is currently in
		gridX = int(float(players.at(0).x)/windowWidth)*windowWidth;
		gridY = int(float(players.at(0).y)/windowHeight)*windowHeight;

		//Set background coordinates
		bgX = players.at(0).x - windowWidth/2;
		bgY = players.at(0).y - windowHeight/2;
	
		//Set fireball position
		for (int p=0;p<players.size();p++) {
			for (int i=0; i<MAXFIREBALLS; i++) players.at(p).fireX[i] += players.at(p).fireSpeed * cos(players.at(p).fireAngle[i]);
			for (int i=0; i<MAXFIREBALLS; i++) players.at(p).fireY[i] += players.at(p).fireSpeed * sin(players.at(p).fireAngle[i]);
		}

		//Set fire cycle
		if (firePressed) {
			if (players.at(0).fireCycle >= 10) {
				int x = players.at(0).fireballNumber++; //Cycled through available fireballs (having a high upper threshold of fireballs prevents onscreen fireballs being recycled)
				players.at(0).fireX[x] = players.at(0).x;
				players.at(0).fireY[x] = players.at(0).y;
				players.at(0).fireAngle[x] = players.at(0).angle;
				if (players.at(0).fireballNumber>=MAXFIREBALLS){
					players.at(0).fireballNumber = 0;
				}
				players.at(0).fireCycle = 0;
			}
			players.at(0).fireCycle++;
		}
	
		//Check if near docking station, and if so allow docking
		int proxX, proxY;
		for (int i=0; i<planets.size(); i++) {
			proxX = abs(players.at(0).x - planets.at(i).x);
			proxY = abs(players.at(0).y - planets.at(i).y);
			if (planets.at(i).canDock)
				if (proxX<planets.at(i).width/2 && proxY<planets.at(i).height/2) {
					players.at(0).canDock = true;
					break;
				}
			else players.at(0).canDock = false;
		}
	
		if (hostSet) {
			//Apply remote ship's new coordinates to player2's current local coordinates
			enet_uint8 *d = NULL;
			while (enet_host_service(host, &event, wait)) { //Poll to enets data buffer
			
				switch(event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					printf("Connection received from %x\n", event.peer->address.host);
					connected = true;
					peer = event.peer;
					newPlayerConnected = true;
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					wait = 0;
					d = event.packet->data;
					players.at(1).x = atoi(strtok((char*)d,"|"));
					players.at(1).y = atoi(strtok(NULL,"|"));
					players.at(1).angle = atof(strtok(NULL,"|"));
					players.at(1).speed = atoi(strtok(NULL,"|"));
					for (int i=0;i<MAXFIREBALLS;i++) {
						players.at(1).fireX[i] = atoi(strtok(NULL,"|"));
						players.at(1).fireY[i] = atoi(strtok(NULL,"|"));
						players.at(1).fireAngle[i] = atof(strtok(NULL,"|"));
					}

					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					printf("%s disconnected.\n", event.peer->data);
					break;
				}
			}

			if (newPlayerConnected) {
				Ship player2("c:/dev/allegro/images/shipSprite.png", "c:/dev/allegro/images/shipSprite1.png", "c:/dev/allegro/images/shipSprite2.png", "c:/dev/allegro/images/fireball.png");
				players.push_back(player2);
				newPlayerConnected = false;
			}

			//Check if connected or not before sending network data
			if (connected) {
				//Send player1's data to remote
				stringstream ss;
				ss << players.at(0).x << "|" << players.at(0).y << "|" << players.at(0).angle << "|" << players.at(0).speed;
				for (int i=0;i<MAXFIREBALLS;i++)
					ss << "|" << players.at(0).fireX[i] << "|" << players.at(0).fireY[i] << "|" << players.at(0).fireAngle[i];
				string data = ss.str();
				ENetPacket *packet = enet_packet_create(data.c_str(), strlen(data.c_str())+1, 0);
				enet_peer_send(peer, 0, packet);
			}
		}
		//Collision detection
		if (players.size() > 1) {
			for (int i=0; i<MAXFIREBALLS; i++ ) { //Checking all enemy fireballs
				if (abs(players.at(0).x - players.at(1).fireX[i]) < abs(players.at(0).width/2 - players.at(1).fireWidth/2))
					if (abs(players.at(0).y - players.at(1).fireY[i]) < abs(players.at(0).height/2 - players.at(1).fireHeight/2))
						triggerCollision();
			}
		}

		//Check for gameover
		if (players.at(0).health <= 0) {
			gameOver = true;
			paused = true;
		}
	}

	if (hyperDrive && textEntered) {
		if (!angleAligned) { //Get angle right first
			//Decide if better to rotate clockwise or anticlockwise
			if (newAngle > players.at(0).angle) {
				if (newAngle - players.at(0).angle < 3.14) players.at(0).angle += 0.05;
				else players.at(0).angle -= 0.05;
			}
			else {
				if (players.at(0).angle - newAngle > 3.14) players.at(0).angle += 0.05;
				else players.at(0).angle -= 0.05;
			}
			if (players.at(0).angle > 6.25) players.at(0).angle = 0; //Keep angle under 6.28
			if (players.at(0).angle < 0) players.at(0).angle = 6.25; //Keep angle above 0
			if (abs(abs(players.at(0).angle) - abs(newAngle)) < 0.05) { //Check for nearness then align angle exactly
				players.at(0).angle = newAngle;
				angleAligned = true;
			}
		}
		if (angleAligned) { //Then check distance
			players.at(0).x += players.at(0).maxSpeed * hyperSpeed * cos(players.at(0).angle);
			players.at(0).y += players.at(0).maxSpeed * hyperSpeed * sin(players.at(0).angle);
			distanceTravelX -= players.at(0).maxSpeed * hyperSpeed * cos(players.at(0).angle);
			distanceTravelY -= players.at(0).maxSpeed * hyperSpeed * sin(players.at(0).angle);
			//if ( abs(abs(players.at(0).x) - abs(x2)) < 1000 && abs(abs(players.at(0).y) - abs(y2)) < 1000) { //Check for nearness, then align distance exactly
			if (distanceTravelX <= 1000 || distanceTravelY <= 1000) {
				players.at(0).x = x2;
				players.at(0).y = y2;
				paused = hyperDrive = angleAligned = textEntered = false; //Reset flags
			}
		}
		//Set which grid the ship is currently in
		gridX = int(float(players.at(0).x)/windowWidth)*windowWidth;
		gridY = int(float(players.at(0).y)/windowHeight)*windowHeight;

		//Set background coordinates
		bgX = players.at(0).x - windowWidth/2;
		bgY = players.at(0).y - windowHeight/2;
	}
}

void update_graphics()
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
        //if (!hyperDrive) al_clear_to_color(al_map_rgb(0, 0, 0));
		al_clear_to_color(al_map_rgb(0, 0, 0));
		
		//Remember that all sprites drawn to the display must be drawn relative to the the current background coordinates (the camera), bgX and bgY
		
		//Draw all 9 of the background sprites, tiled around, and including, the current grid
		al_draw_bitmap(backgroundSprite1,gridX-bgX,gridY-bgY,0);
		al_draw_bitmap(backgroundSprite2,gridX-bgX,gridY-windowHeight-bgY,0);
		al_draw_bitmap(backgroundSprite3,gridX-bgX,gridY+windowHeight-bgY,0);
		al_draw_bitmap(backgroundSprite4,gridX-windowWidth-bgX,gridY-bgY,0);
		al_draw_bitmap(backgroundSprite5,gridX-windowWidth-bgX,gridY-windowHeight-bgY,0);
		al_draw_bitmap(backgroundSprite6,gridX-windowWidth-bgX,gridY+windowHeight-bgY,0);
		al_draw_bitmap(backgroundSprite7,gridX+windowWidth-bgX,gridY-bgY,0);
		al_draw_bitmap(backgroundSprite8,gridX+windowWidth-bgX,gridY-windowHeight-bgY,0);
		al_draw_bitmap(backgroundSprite9,gridX+windowWidth-bgX,gridY+windowHeight-bgY,0);
		
		//Draw planets
		for (int i=0; i<planets.size(); i++)
			al_draw_scaled_rotated_bitmap(planets.at(i).planetSprite, planets.at(i).width/2, planets.at(i).height/2, planets.at(i).x-bgX, planets.at(i).y-bgY, planets.at(i).planetScale, planets.at(i).planetScale, 0, 0); //Draw planets only if their coordinates exist within current screen
		//Draw sprites for each Ship object
		for (int p=players.size()-1;p>=0;p--) {
			//Draw fireballs
			for (int i=0; i<MAXFIREBALLS; i++)
				al_draw_tinted_rotated_bitmap(players.at(p).fireSprite[i], al_map_rgb(255,255,255), players.at(p).fireWidth/2, players.at(p).fireHeight/2, players.at(p).fireX[i]-bgX, players.at(p).fireY[i]-bgY, players.at(p).fireAngle[i], 0);
			//Draw ships
			al_draw_rotated_bitmap(players.at(p).shipSpriteCurrent, players.at(p).width/2, players.at(p).height/2, players.at(p).x-bgX, players.at(p).y-bgY, players.at(p).angle, 0); 
		}

		//Draw bordering rectangle
		al_draw_rectangle(0-bgX, 0-bgY, maxX-bgX, maxY-bgY, al_map_rgb(255, 255, 255), 10);
		
		//Draw stats to screen
		stringstream s1, s2;
		s1 << "Health: " << players.at(0).health;
		s2 << "Coordinates: " << players.at(0).x << " , " << players.at(0).y;
		string str1 = s1.str();
		string str2 = s2.str();
		al_draw_text(font, al_map_rgb(255,255,255), windowWidth*0.05, windowHeight*0.1, 0, str1.c_str());
		al_draw_text(font, al_map_rgb(255,255,255), windowWidth*0.05, windowHeight*0.9, 0, str2.c_str());

		//Draw radar
		al_draw_scaled_rotated_bitmap(radarSprite, radarScreenWidth/2, radarScreenHeight/2, windowWidth*0.85, windowHeight*0.15, 0.2, 0.2, 0, 0);
		//Planets
		for (int i=0; i<planets.size(); i++) {
			float rX = windowWidth*0.85 - (players.at(0).x - planets.at(i).x) * radarScale;
			float rY = windowHeight*0.15 - (players.at(0).y - planets.at(i).y) * radarScale;
			if ((rX > windowWidth*0.85 - windowWidth*0.1) && (rX < windowWidth*0.85 + windowWidth*0.1) && (rY > windowHeight*0.15 - windowHeight*0.1) && (rY < windowHeight*0.15 + windowHeight*0.1))
				al_draw_rotated_bitmap(radarDotSprite, 2.5, 2.5, rX, rY, 0, 0); //Draw planets only if their coordinates exist within current screen
		}
		//Ships
		for (int p=players.size()-1;p>=0;p--) {
			float rX = windowWidth*0.85 - (players.at(0).x - players.at(p).x) * radarScale;
			float rY = windowHeight*0.15 - (players.at(0).y - players.at(p).y) * radarScale;
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
		al_draw_rotated_bitmap(players.at(0).shipSprite, players.at(0).width/2, players.at(0).height/2, 250, 250, 0, 0);
		al_draw_rotated_bitmap(players.at(0).fireSprite[0], players.at(0).fireHeight/2, players.at(0).fireWidth/2, 250, 250, 0, 0);
		al_draw_bitmap(dockingText, 0, 0, 0);
		if (dispUpgradeText) al_draw_bitmap(upgradedText, 0, windowHeight/2, 0);
		
		break;
		}

	//Join server mode
	case 3:
		{
		al_clear_to_color(al_map_rgb(30,30,30));
		al_draw_text(font, al_map_rgb(255,255,255), windowWidth*0.5, windowHeight*0.4, ALLEGRO_ALIGN_CENTRE, "ENTER SERVER IP:");
		al_draw_text(font, al_map_rgb(255,255,255), windowWidth*0.5, windowHeight*0.5, ALLEGRO_ALIGN_CENTRE, ipAddress.c_str());
		
		break;
		}
	}
}

void game_loop()
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


///////////////////////////////////////////////////////
////////////////  NETWORK FUNCTIONS  //////////////////


void setUpHost() {
	//Create server (or client as below, depending on homeScreenOption);
	if (homeScreenOption==1) {
		ENetAddress address;
		//Bind server to localhost
		address.host = ENET_HOST_ANY;
		// Bind server to port 1234
		address.port = 1234;
		host = enet_host_create (& address /* the address to bind the server host to */, 
									 32      /* allow up to 32 clients and/or outgoing connections */,
									  2      /* allow up to 2 channels to be used, 0 and 1 */,
									  0      /* assume any amount of incoming bandwidth */,
									  0      /* assume any amount of outgoing bandwidth */);
		if (host == NULL) {
			abort_game("An error occurred while trying to create an ENet server host.\n");
		}

		hostSet = true;
	}

	//Create client
	else if (homeScreenOption==2) {
		host = enet_host_create (NULL /* create a client host */,
					1 /* only allow 1 outgoing connection */,
					2 /* allow up 2 channels to be used, 0 and 1 */,
					0	/* 57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
					0	/* 14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);
		if (host == NULL) {
			abort_game("An error occurred while trying to create an ENet client host.\n");
		}

		//Begin connection to server machine
		ENetAddress address;
		/* Connect to server:1234. */
		//enet_address_set_host (& address, "109.125.19.33");
		enet_address_set_host (& address, ipAddress.c_str());
		//enet_address_set_host (& address, "localhost");
		address.port = 1234;
		
		/* Initiate the connection, allocating the two channels 0 and 1. */
		peer = enet_host_connect (host, & address, 2, 2500); //Bind successful connection to peer
		if (peer == NULL)
		{
		   abort_game("No server available at this address.\n");
		}
		/*else {
			cout << "Connected." << endl;
			connected = true;
		}*/

		hostSet = true;
	}
	else ; //Single player mode (just don't setup server or client!)
}
