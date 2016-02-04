#ifndef SHIP_H
#define SHIP_H
#define MAXFIREBALLS 15

#include <allegro5/allegro5.h>
#include <string>

class Ship {
public:
	int id, x, y, height, width, speed, speedX, speedY, maxSpeed, armour, flipflop, fireballNumber, 
		fireX[MAXFIREBALLS], fireY[MAXFIREBALLS], fireHeight, fireWidth, fireSpeed, fireCycle, shields, energy, fuel, credits;
	float angle, fireAngle[MAXFIREBALLS];
	ALLEGRO_BITMAP *shipSprite, *shipSprite1, *shipSprite2, *shipSpriteCurrent;
	ALLEGRO_BITMAP *fireSprite[MAXFIREBALLS];
	bool canDock;

	Ship();
	Ship(const char*, const char*, const char*, const char*);
};

#endif