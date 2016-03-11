#include "Map.h"


Map::Map(void)
{
}

Planet Map::getPlanet(int index) { return planets.at(index); }
int Map::getTotalPlanets() { return planets.size(); }

Map::~Map(void)
{
}
