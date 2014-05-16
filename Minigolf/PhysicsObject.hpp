#include "Physics.hpp"
#include "MathHelper.hpp"

//Gravity acceleration value
static const float Gravity = .0001f;
static const float Epsilon = .00000001;

//A world object affected by physics; it is an abstract class
class PhysicsObject
{
public:
	//Essentially create a line between 2 points and see if the ball passed it
	bool CheckPassEdge(glm::vec3 vertex1, glm::vec3 vertex2)
	{
		//Check X, Y, and Z values
		bool xcheck = (Model.Coordinate.x >= vertex1.x && Model.Coordinate.x <= vertex2.x);
		bool ycheck = (Model.Coordinate.y >= vertex1.y && Model.Coordinate.y <= vertex2.y);
		bool zcheck = (Model.Coordinate.z >= vertex1.z && Model.Coordinate.z <= vertex2.z);

		return (xcheck == true && ycheck == true && zcheck == true);
	}

	//Use only X and Z for 3D projecting into 1D
	//Base algorithm obtained from here: http://stackoverflow.com/a/16391873
	//It has been modified to work in 3D space
	bool IsPointInPolygon(glm::vec3 point, vector<glm::vec3> polygon)
	{
		float minX = polygon[0].x;
		float maxX = polygon[0].x;
		float minZ = polygon[0].z;
		float maxZ = polygon[0].z;
		for (int i = 1; i < polygon.size(); i++)
		{
			glm::vec3 point = polygon[i];
			minX = min(point.x, minX);
			maxX = max(point.x, maxX);
			minZ = min(point.z, minZ);
			maxZ = max(point.z, maxZ);
		}

		if (point.x < minX || point.x > maxX || point.z < minZ || point.z > maxZ)
			return false;

		bool inside = false;
		for (int i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
		{
			if ((polygon[i].z > point.z) != (polygon[j].z > point.z) && point.x < (polygon[j].x - polygon[i].x) * (point.z - polygon[i].z) / (polygon[j].z - polygon[i].z) + polygon[i].x)
				inside = !inside;
		}

		return inside;
	}

	bool RayPlaneCollision(glm::vec3 rayposition, glm::vec3 raydirection, /*const Tile &tile,*/glm::vec3 normal, double &collisiondistance)
	{
		//double raything = glm::dot((rayposition - tile.Location), tile.Normal);
		//double raydirnormal = glm::dot(raydirection, tile.Normal);
		//
		//double d = -(raything / raydirnormal);

		//cout << endl << raydirection.x << "," << raydirection.y << "," << raydirection.z << endl;
		//cout << endl << rayposition.x << "," << rayposition.y << "," << rayposition.z << endl;

		cout << endl << normal.x << "," << normal.y << "," << normal.z << endl;

		//Check if they're parallel
		double parallel = glm::dot(normal, raydirection);
		
		if (parallel >= Epsilon || parallel < (-Epsilon))
		{
			//Distance to intersection
			collisiondistance = glm::dot(normal, (rayposition - raydirection)) / parallel;
			if (collisiondistance >= 0)
			{
				//Find intersection point
				glm::vec3 intersectionpoint = rayposition + (raydirection * static_cast<float>(collisiondistance));
				cout << endl << intersectionpoint.x << "," << intersectionpoint.y << "," << intersectionpoint.z << endl;

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

		//for (int i = 0; i < tiles.size(); i++)
		//{
		//	//if (tiles[i])
		//}

		//If there was no tile, stop and return null
		Velocity = glm::vec3(0, 0, 0);
		return NULL;
	}

	//Adds force to the object in a direction with a magnitude
	void AddForce(glm::vec3 velocity)//glm::vec3 direction, float magnitude)
	{
		Direction = glm::vec3(0, 0, -1);
		Velocity = velocity;//Direction * magnitude;
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
	}

	vector<Tile> FindSamePlane()
	{
		vector<Tile> curtiles = getTiles();
		vector<Tile> planetiles = getTiles();

		for (int i = 0; i < curtiles.size(); i++)
		{
			if (IsPointInPolygon(Model.Coordinate, curtiles[i].Vertices) == true)
				planetiles.push_back(curtiles[i]);
		}

		return planetiles;
	}

	//Move the object
	virtual void Move(vector<Tile> tiles)
	{
		/*(Radius + position <= wall plane) is a collision*/

		//NOTE: We're checking collisions from the center of the ball, which is wrong; we should be checking from the end of the ball (radius + position)

		//Check all the edges
		vector<double> collisiondistances;

		//Go through all neighbors and see which one it hits
		for (int i = 0; i < ObjectTile.Neighbors.size(); i++)
		{
			collisiondistances.push_back(-1.0);
			if (ObjectTile.Neighbors[i] > 0)
			{
				Tile tile = getTiles()[ObjectTile.Neighbors[i] - 1];

				//Raycast forwards to a neighbor
				if (RayPlaneCollision(Model.Coordinate, Direction, tile.Normal, collisiondistances[i]) == true)
				{
					if (collisiondistances[i] <= .01) MoveTile(tile);
					cout << '\n' << ObjectTile.TileID << endl;

					//Model.Coordinate += glm::vec3(Velocity.x, 0, Velocity.z);
					//cout << '\n' << collisiondistances[i] << endl;

					//Be affected by gravity
					//if (collisiondistances[i] >= 0.1) Velocity.y -= Gravity;
					//else Velocity.y = 0.0f;

					//cout << '\n' << i << " " << tiles[i].TileID << endl;// << " " << collisiondistances[i] << endl;
					//cout << '\n' << collisiondistances[i] << endl;
					//Model.Coordinate += Velocity;
				}
			}
		}

		/*What the raycast currently does:
			In hole.01.db, Tile 1 has a Z between .5 and 2.5
			Tile 4 is a neighbor, when moving up, I encounter it
		*/

		//Be affected by gravity
		if (Model.Coordinate.y > ObjectTile.Location.y)
		{
			cout << endl << ObjectTile.TileID << "  " << Model.Coordinate.x <<  "," << Model.Coordinate.y << "," << Model.Coordinate.z << endl;
			ObjectTile.PrintVertices();

			Velocity.y -= Gravity;
			Model.Coordinate.y += Velocity.y;
			if (Model.Coordinate.y <= ObjectTile.Location.y)
			{
				Model.Coordinate.y = ObjectTile.Location.y;
				Velocity.y = 0.0f;
			}
		}

		

		//Sort collision distances in ascending order
		//sort(collisiondistances.begin(), collisiondistances.end());
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