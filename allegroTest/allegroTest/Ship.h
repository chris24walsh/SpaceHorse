#ifndef SHIP_H
#define SHIP_H
#define MAXFIREBALLS 15

#include <allegro5/allegro5.h>
#include <string>

class Ship {
public:
	int id, x, y, height, width, speed, speedX, speedY, maxSpeed, armour, flipflop, fireballNumber, 
		fireX[MAXFIREBALLS], fireY[MAXFIREBALLS], fireHeight, fireWidth, fireSpeed, fireCycle, 
		shields, energy, energyCounter, maxEnergy, fuel, maxFuel, credits, minerals, luxuries, cargoUsed, maxCargo;
	float angle, fireAngle[MAXFIREBALLS];
	ALLEGRO_BITMAP *shipSprite, *shipSprite1, *shipSprite2, *shipSpriteCurrent;
	ALLEGRO_BITMAP *fireSprite[MAXFIREBALLS];
	bool canDock;

	Ship();
	Ship(const char*, const char*, const char*, const char*);
};

#endif