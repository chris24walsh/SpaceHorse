#ifndef MAP_H
#define MAP_H

#include "Header.h"

#include "Planet.h"

class Map
{
private:
	std::vector<Planet> planets;

public:
	Map(void);
	void makeSolarSystem();
	std::vector<Planet>& getPlanets();
	~Map(void);
};

#endif