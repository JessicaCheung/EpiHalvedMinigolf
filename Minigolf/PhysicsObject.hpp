#include "Physics.hpp"
#include "MathHelper.hpp"

//A world object affected by physics; it is an abstract class
class PhysicsObject
{
public:
	bool RayPlaneCollision(glm::vec3 rayposition, glm::vec3 raydirection, const Tile &tile, double &collisiondistance)
	{
		//double raything = glm::dot((rayposition - tile.Location), tile.Normal);
		//double raydirnormal = glm::dot(raydirection, tile.Normal);
		//
		//double d = -(raything / raydirnormal);

		//Check if they're parallel
		double parallel = glm::dot(tile.Normal, raydirection);
		
		if (parallel != 0.0)
		{
			//Distance to intersection
			collisiondistance = glm::dot(tile.Normal, (rayposition - raydirection)) / parallel;
			if (collisiondistance >= 0)
			{
				//Find intersection point
				glm::vec3 intersectionpoint = rayposition + (raydirection * static_cast<float>(collisiondistance));

				return true;
			}
		}

		return false;
		//
		//return false;
		//double denominator = glm::dot(tile.Normal, raydirection);
		//
		//if (denominator > 1e-6)
		//{
		//	glm::vec3 planepositionrayorigin = tile.Location - rayposition;
		//	double d = glm::dot(planepositionrayorigin, tile.Normal) / denominator;
		//
		//	//Found an intersection if >= 0
		//	return (d >= 0);
		//}
		//
		//return false;

		//Get the dot product between the tile's normal and the ray's direction
		//double dotproduct = glm::dot(raydirection, tile.Normal);
		//
		//double colldist;
		//
		////Check if it's parallel; a value of 0 indicates no collision
		//if (dotproduct == 0)//dotproduct < 0 && dotproduct >= 0)
		//	return false;
		//
		////glm::vec3 subtractvector = tile.Location - rayposition;
		//
		////Find the distance to the collision point
		//colldist = glm::dot(tile.Normal, rayposition) / dotproduct;
		//
		////Check if the collision is behind the start
		//if (colldist < 0)
		//	return false;
		//
		//collisiondistance = colldist;
		//return true;
	}

	//The model to import
	ImportObj Model;

	//The location of the object; this should be the center
	//If we imported a model, we will use that model's coordinates instead
	glm::vec3 Location;

	//The direction of the object
	glm::vec3 Direction;

	//The velocity of the object
	float Velocity;

	//The tile the object is on; if a tile isn't present where it's about to go, then that is a wall
	Tile ObjectTile;

	//Default constructor
	PhysicsObject()
	{
		Direction = Location = glm::vec3(0, 0, 0);
		Velocity = 0.0f;
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

		//for (int i = 0; i < tiles.size(); i++)
		//{
		//	//if (tiles[i])
		//}

		//If there was no tile, stop and return null
		Velocity = 0.0f;
		return NULL;
	}

	//Adds force to the object in a direction with a magnitude
	void AddForce(glm::vec3 direction, float magnitude)
	{
		Direction = direction;
		Velocity = magnitude;
	}

	void TeleportTo(glm::vec3 location)
	{
		Model.Coordinate = location;
	}

	void MoveTile(Tile tile)
	{
		//Velocity is (.05, .05, -.05)
		//Normal is (.5, 1, .5)
		//Velocity should be (.025, .05, -.025)
		
		ObjectTile = tile;
		cout << '\n' << tile.TileID << endl;
		//Direction = tile.Normal;
		

		Model.Coordinate += (Velocity * Direction);
	}

	//Move the object
	virtual void Move(vector<Tile> tiles)
	{
		vector<double> collisiondistances;
		for (int i = 0; i < tiles.size(); i++)
		{
			collisiondistances.push_back(-1.0);

			if (RayPlaneCollision(Model.Coordinate, glm::vec3(0, -1, 0), tiles[i], collisiondistances[i]) == true)
			{
				cout << '\n' << i << " " << tiles[i].TileID << endl;// << " " << collisiondistances[i] << endl;
				cout << '\n' << collisiondistances[i] << endl;
				Model.Coordinate += (Velocity * Direction);
			}
		}

		//Check lowest collision distance
		//int tileindex = LowestCollDist(collisiondistances);
		//
		//if (tileindex >= 0)
		//	MoveTile(tiles[tileindex]);
	}

	int LowestCollDist(vector<double> collisiondistances)
	{
		double lowestnum = INFINITY;
		int index = -1;

		for (int i = 0; i < collisiondistances.size(); i++)
		{
			if (collisiondistances[i] < lowestnum && collisiondistances[i] >= 0)
			{
				lowestnum = collisiondistances[i];
				index = i;
			}
		}

		return index;
	}

	//Move the object, passing a velocity in
	virtual void Move(glm::vec3 newvelocity)
	{
		Model.Coordinate.x += newvelocity.x;
		Model.Coordinate.y += newvelocity.y;
		Model.Coordinate.z += newvelocity.z;
	}

	//Checks for collisions; pure virtual because each object can have a different shape, thus needing to raycast from different points
	virtual void CheckCollisions() = 0;
};