#include "Allegro_init.h"
#include "Ship.h"

Ship::Ship(const char* filename1, const char* filename2, const char* filename3, const char* filename4) {
//Ship::Ship() {
	
	srand(time(NULL));
	//x = rand()%1000;
	//y = rand()%1000;
	id = rand()%1000;
	x = 489000;
	y = 503800;
	shipSprite = al_load_bitmap(filename1);
	shipSprite1 = al_load_bitmap(filename2);
	shipSprite2 = al_load_bitmap(filename3);
	height = al_get_bitmap_height(shipSprite);// rand()%200 + 50;
	width = al_get_bitmap_width(shipSprite);// rand()%200 + 50;
	//height = width = 100;
	speed = speedX = speedY = 0;
	maxSpeed = 10;
	angle = 0;
	health = 6;
	flipflop = 0;
	fireWidth = 30;
	fireHeight = 20;
	fireSpeed = maxSpeed + 10;
	fireballNumber = 0;
	for (int i=0;i<MAXFIREBALLS;i++) {
		fireSprite[i]= al_load_bitmap(filename4);
		fireX[i] = 2000;
		fireY[i] = 2000;
		fireAngle[i] = 0;
	}
	fireCycle = 100;
};