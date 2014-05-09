#include "Physics.hpp"

//A world object affected by physics; it is an abstract class
class PhysicsObject
{
private:
	void ReloadPosition()
	{
		vector<glm::vec3> newvertices;
		vector<int> newelements;
		vector<glm::vec3> newnormals;

		Model.Indices.clear();
		load_obj("BallSmall.obj", newvertices, newelements, Model.Coordinate);

		Model.Vertices.swap(newvertices);
		Model.Indices.swap(newelements);
		Model.Normals.swap(newnormals);

		Model.CalculateNormals();
	}

public:
	//The model to import
	ImportObj Model;

	//The location of the object; this should be the center
	//If we imported a model, we will use that model's coordinates instead
	glm::vec3 Location;

	//The direction of the object
	glm::vec3 Direction;

	//The velocity of the object
	glm::vec3 Velocity;

	//The tile the object is on; if a tile isn't present where it's about to go, then that is a wall
	Tile ObjectTile;

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
		/*if (Model != NULL)*/ return Model.Coordinate;
		//else return Location;
	}

	//Adds force to the object in a direction with a magnitude
	void AddForce(glm::vec3 direction, float magnitude)
	{
		Direction = direction;
		Velocity = direction * magnitude;
	}

	//Move the object
	virtual void Move()
	{
		/*if (Model != NULL)*/ Model.Coordinate += Velocity;
		//else Location += Velocity;

		ReloadPosition();
	}

	virtual void Move(glm::vec3 newvelocity)
	{
		Model.Coordinate += newvelocity;
		ReloadPosition();
	}

	//Checks for collisions; pure virtual because each object can have a different shape, thus needing to raycast from different points
	virtual void CheckCollisions() = 0;
};