//#pragma once
#ifndef HOLE_H
#define HOLE_H
#include "Map.hpp"

//A hole, or level in our minigolf stage
class Hole
{
public:
	vector<Tile> Tiles;		//The tiles in the map
	MapObject TileBuffer;	//Buffer 
	ImportObj Walls;		//The walls in the hole
	ImportObj Tee;			//The tee for this hole
	ImportObj Cup;			//The cup for this hole
	int Par;
	string HoleName;

	//Constructor
	Hole()
	{
	}

	Hole(vector<Tile> tiles, MapObject tilebuffer, ImportObj walls, ImportObj tee, ImportObj cup, int par, string name)
	{
		Tiles = tiles;
		TileBuffer = tilebuffer;
		Walls = walls;
		Tee = tee;
		Cup = cup;
		Par = par;
		HoleName = name;
	}

	void SetTiles(vector<Tile> tiles) { Tiles = tiles; }
	void SetTileBuffer(MapObject buffer) { TileBuffer = buffer; }
	void SetWalls(ImportObj walls) { Walls = walls; }
	void SetTee(ImportObj tee) { Tee = tee; }
	void SetCup(ImportObj cup) { Cup = cup; }
	void SetPar(int par) { Par = par; }
	void SetName(string name) { HoleName = name; }

};

void LoadHole(vector<Tile> tiles, MapObject tilebuffer, ImportObj walls, ImportObj tee, ImportObj cup, int par, string name);

vector<Hole> getHoles();

#endif