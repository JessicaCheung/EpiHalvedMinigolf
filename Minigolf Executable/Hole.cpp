#include "Hole.hpp"

vector<Hole> Holes;

void LoadHole(vector<Tile> tiles, MapObject tilebuffer, ImportObj walls, ImportObj tee, ImportObj cup, int par, string name)
{
	Hole tempHole = Hole(tiles, tilebuffer, walls, tee, cup, par, name);
	Holes.push_back(tempHole);
	tempHole = Hole();
}

vector<Hole> getHoles()
{
	return Holes;
}