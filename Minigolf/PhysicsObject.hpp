#include "Physics.hpp"
#include "MathHelper.hpp"

//A world object affected by physics; it is an abstract class
class PhysicsObject
{
public:
	bool RayPlaneCollision(glm::vec3 rayposition, glm::vec3 raydirection, const Tile &tile, double &collisiondistance)
	{
		//Get the dot product between the tile's normal and the ray's direction
		double dotproduct = glm::dot(raydirection, tile.Normal);

		double colldist;

		//Check if it's parallel; a value of 0 indicates no collision
		if (dotproduct < 0 && dotproduct >= 0)
			return false;

		glm::vec3 subtractvector = tile.Location - rayposition;

		//Find the distance to the collision point
		colldist = glm::dot(tile.Normal, subtractvector) / dotproduct;

		//Check if the collision is behind the start
		if (colldist <= 0)
			return false;

		collisiondistance = colldist;
		return true;
	}

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

	/*Ray-Plane Intersection (raycasting)
	Plane:
	Ax + By + Cz + D = 0
	Xn : (A, B, C)

	For any point X on the plane,
	Xn * X = d

	Ray: Pr = Rs + (t * Rdir)

	At the intersection point,

	t = Xn dot (Pp - Rdir) / (Xn dot Rdir)

	(Xn dot Rdir) would be 0 if the ray and plane are parallel

	The lower t value is your first collision, and thus the only one you care about
	*/

	/*Sphere-Sphere collision
	t = *-(A * B) - sqrt((A*B)^2 - B^2(A^2 - (rP + rQ)^2)) / (B^2)

	A = P1 - Q1
	B = (P2 - P1) - (Q2 - Q1)

	If t is in [0..1] then collide

	OR:

	d^2 = A^2 - (((A*B)^2) / B^2)

	if distance large enough, no collision: d^2 > (rP + rQ)^2
	*/

	//Gets the tile the object is on based on its position
	Tile GetTile()
	{
		vector<Tile> tiles = getTiles();

		for (int i = 0; i < tiles.size(); i++)
		{
			//if (tiles[i])
		}

		//If there was no tile, stop and return null
		Velocity = glm::vec3(0, 0, 0);
		return NULL;
	}

	//Adds force to the object in a direction with a magnitude
	void AddForce(glm::vec3 direction, float magnitude)
	{
		Direction = direction;
		Velocity = direction * magnitude;
	}

	void MoveTile(Tile tile)
	{
		//Velocity is (.05, .05, -.05)
		//Normal is (.5, 1, .5)
		//Velocity should be (.025, .05, -.025)
		Direction = tile.Normal;
		//Model.Coordinate += (Velocity * tile.Normal);
	}

	//Move the object
	virtual void Move(vector<Tile> tiles)
	{
		for (int i = 0; i < tiles.size(); i++)
		{
			double collisiondistance = -1.0;
			double collisiondistancedown = -1.0;

			if ((RayPlaneCollision(Model.Coordinate, glm::vec3(Direction.x, -.5f, -.5f), tiles[i], collisiondistance) == true && collisiondistance >= .1 && collisiondistance <= .2))// || (RayPlaneCollision(Model.Coordinate, glm::vec3(0, 1, 0), tiles[i], collisiondistancedown) == true && collisiondistancedown >= 0))
			{
				MoveTile(tiles[i]);
				break;
			}
		}

		Model.Coordinate += (Velocity * Direction);

		/*if (Model != NULL)*/ //Model.Coordinate += (Velocity * Direction);
		//if (Direction.y < 1) Model.Coordinate.y += (.05 * Direction.y);
		//Model.Coordinate.x += Velocity.x * Direction.x;
		//Model.Coordinate.z += Velocity.z * Direction.z;
		//else Location += Velocity;

		//ReloadPosition();
	}

	//Move the object, passing a velocity in
	virtual void Move(glm::vec3 newvelocity)
	{
		Model.Coordinate.x += newvelocity.x;
		Model.Coordinate.y += newvelocity.y;
		Model.Coordinate.z += newvelocity.z;
		//ReloadPosition();
	}

	//Checks for collisions; pure virtual because each object can have a different shape, thus needing to raycast from different points
	virtual void CheckCollisions() = 0;
};