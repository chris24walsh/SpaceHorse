#ifndef MAP_H
#define MAP_H

#include "Header.h"

#include "Planet.h"
#include "Space_junk.h"

class Map
{
private:
	std::vector<Planet> m_planets;
	std::vector<SpaceJunk> m_spaceJunks;

public:
	Map(void);
	void makeSolarSystem();
	std::vector<Planet>& getPlanets();
	void generateSpaceJunk();
	std::vector<SpaceJunk>& getSpaceJunk();
	~Map(void);
};

#endif