#include "Planet.h"
#include <math.h>

Planet::Planet(const char* filename) {
	x = rand()%1000;
	y = rand()%1000;
	planetSprite = al_load_bitmap(filename);
	height = al_get_bitmap_height(planetSprite);// rand()%200 + 50;
	width = al_get_bitmap_width(planetSprite);// rand()%200 + 50;
	planetScale = 4;
}

Planet::Planet(const char* filename, int d) {
	planetScale = 4;
	distance = d;
	float angle = float(6.25)*(rand()%100)/100;
	x = distance * 1000 * planetScale * cos(angle) + 500000;
	y = distance * 1000 * planetScale * sin(angle) + 500000;
	planetSprite = al_load_bitmap(filename);
	height = al_get_bitmap_height(planetSprite);// rand()%200 + 50;
	width = al_get_bitmap_width(planetSprite);// rand()%200 + 50;
}