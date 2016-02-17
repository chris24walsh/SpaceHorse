#ifndef PLANET_H
#define PLANET_H

#include <allegro5\allegro.h>

class Planet {
public:
	int x, y, height, width, color1, color2, color3, planetScale;
	float distance;
	bool canCollide, canDock;
	ALLEGRO_BITMAP *planetSprite;

	Planet(const char* filename);
	Planet(const char* filename, int distance);
};

#endif