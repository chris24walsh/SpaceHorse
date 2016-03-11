#ifndef MAP_H
#define MAP_H

#include "Game.h"
#include "Planet.h"

class Map
{
private:
	std::vector<Planet> planets;

public:
	Map(void);
	Planet getPlanet(int index);
	int getTotalPlanets();
	~Map(void);
};

#endif