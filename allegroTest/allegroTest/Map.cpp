#include "Map.h"


Map::Map(void)
	:planets(0)
{
}

void Map::makeSolarSystem()
{
	int canUpgrade[8] = {0,0,0,0,0,0,0,0};
	int i=0;
	while(!canUpgrade[i])
	{
		canUpgrade[i] = rand()%8;
		if(canUpgrade[i]==1) { break; }
		else { canUpgrade[i] = 0; }
		++i;
		if(i==8) { i=0; }
	}

	Planet sun("c:/dev/allegro/images/sun.png", 0, 0, 1, 0);
	Planet mercury("c:/dev/allegro/images/mercury.png", 1, 1, 0, canUpgrade[0]);
	Planet venus("c:/dev/allegro/images/venus.png", 2, 1, 0, canUpgrade[1]);
	Planet earth("c:/dev/allegro/images/earth.png", 3, 1, 0, canUpgrade[2]);
	Planet mars("c:/dev/allegro/images/mars.png", 4, 1, 0, canUpgrade[3]);
	Planet jupiter("c:/dev/allegro/images/jupiter.png", 13, 1, 0, canUpgrade[4]);
	Planet saturn("c:/dev/allegro/images/saturn.png", 24, 1, 0, canUpgrade[5]);
	Planet neptune("c:/dev/allegro/images/neptune.png", 49, 1, 0, canUpgrade[6]);
	Planet uranus("c:/dev/allegro/images/uranus.png", 76, 1, 0, canUpgrade[7]);
	planets.reserve(9); //this is the number of celestial bodies, but if it's wrong, push_back will still resize if necessary
	planets.push_back(sun);
	planets.push_back(mercury);
	planets.push_back(venus);
	planets.push_back(earth);
	planets.push_back(mars);
	planets.push_back(jupiter);
	planets.push_back(saturn);
	planets.push_back(neptune);
	planets.push_back(uranus);
}

std::vector<Planet>& Map::getPlanets() { return planets; }

Map::~Map(void)
{
}
