#include "PhysicsObject.hpp"

//The ball we use to roll into the hole
class Ball : public PhysicsObject
{
public:

	//Constructors
	Ball()
	{

	}
	
	Ball(ImportObj &sphere, glm::vec3 location)
	{
		Model = sphere;
		Location = location;
		Model.Coordinate = Location;
	}

	//Check the edges of the sphere
	void CheckCollisions() override
	{

	}
};