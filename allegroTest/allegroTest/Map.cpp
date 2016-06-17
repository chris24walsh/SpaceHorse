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

	Planet sun("../../images/sun.png", "The Sun", 0, 0, 1, 0);
	Planet mercury("../../images/mercury.png", "Mercury", 1, 1, 0, canUpgrade[0]);
	Planet venus("../../images/venus.png", "Venus", 2, 1, 0, canUpgrade[1]);
	Planet earth("../../images/earth.png", "Earth", 3, 1, 0, canUpgrade[2]);
	Planet mars("../../images/mars.png", "Mars", 4, 1, 0, canUpgrade[3]);
	Planet jupiter("../../images/jupiter.png", "Jupiter", 13, 1, 0, canUpgrade[4]);
	Planet saturn("../../images/saturn.png", "Saturn", 24, 1, 0, canUpgrade[5]);
	Planet neptune("../../images/neptune.png", "Neptune", 49, 1, 0, canUpgrade[6]);
	Planet uranus("../../images/uranus.png", "Uranus", 76, 1, 0, canUpgrade[7]);
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
