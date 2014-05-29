#include "Map.hpp"

//Gravity acceleration value
static const float Gravity = .0001f;
static const float Epsilon = .00000001;

//A world object affected by physics; it is an abstract class
class PhysicsObject
{
public:
	ImportObj Model;			//The model to import
	glm::vec3 Location;			//Current vec3 position
	glm::vec3 Direction;		//The direction of the object
	glm::vec3 Velocity;			//The velocity of the object
	Tile ObjectTile;			//Current Tile
	Tile Origin;

	//Default constructor
	PhysicsObject()
	{
		Direction = Location = Velocity = glm::vec3(0, 0, 0);
		ObjectTile = NULL;
		Model = NULL;
	}

	//Gets the location of the object
	glm::vec3 GetLocation()
	{
		return Model.Coordinate;
	}

	//Adds force to the object in a direction with a magnitude
	void AddForce(glm::vec3 velocity)			//glm::vec3 direction, float magnitude)
	{
		Velocity += velocity;					//Direction * magnitude;
	}

	void NegativeForce()
	{
		Direction *= -1.0f;
	}

	void ReturnTo()
	{
		Model.Coordinate = Location;
		Velocity = glm::vec3(0, 0, 0);
		ObjectTile = Origin;
	}

	void ReturnTo(glm::vec3 location)
	{
		Model.Coordinate = location;
		Velocity = glm::vec3(0, 0, 0);
	}
};

class Ball : public PhysicsObject
{
public:
	//Constructors
	Ball() {}

	Ball(ImportObj &sphere, glm::vec3 location, Tile currentTile)
	{
		Model = sphere;
		Location = location;
		Model.Coordinate = Location;
		ObjectTile = currentTile;
		Origin = currentTile;
	}
};