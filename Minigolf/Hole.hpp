/*#pragma once
#include "main.hpp"
#include "Map.hpp"
#include "MapReader.hpp"
#include "PhysicsObject.hpp"

//A hole, or level in our minigolf stage
class Hole
{
public:
	//The tiles in the map
	vector<Tile> Tiles;

	//The walls in the hole
	ImportObj Walls;

	//The tee for this hole
	ImportObj Tee;

	//The cup for this hole
	ImportObj Cup;

	//The golf ball
	Ball Golfball;

	//Constructor
	Hole(string filename)
	{
		LoadHole(filename);
		
		//Load the ball on the Tee
		LoadGolfBall();
	}

	//Places the Golf Ball on the Tee
	void LoadGolfBall()
	{
		ImportObj temp;
		load_obj("BallSmall.obj", temp.Vertices, temp.Indices, glm::vec3(0, 0.08, 0));
		temp.CalculateNormals();

		Golfball = Ball(temp, Tee.Coordinate, Tiles[Tee.TileID - 1]);
	}

	void LoadHole(string filename)
	{
		ReadFile(filename, Tiles, Tee, Cup);
	}

	//Updates the hole
	void Update()
	{
		GolfBall.Move();
	}

	//Initializes rendering of the hole (binds buffers, etc.)
	void InitHoleRender()
	{

	}

	//Renders the hole
	void RenderHole()
	{
		RenderTiles();
		RenderTee();
		RenderCup();
	}

	//Renders all the tiles
	void RenderTiles()
	{

	}

	//Renders the Tee
	void RenderTee()
	{

	}

	//Renders the Cup
	void RenderCup()
	{

	}
};*/