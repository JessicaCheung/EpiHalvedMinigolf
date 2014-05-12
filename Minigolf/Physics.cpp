#include "Physics.hpp"

//This returns the tile or wall that the ray may collide with
//bool RayPlaneCollision(glm::vec3 rayposition, glm::vec3 raydirection, const Tile &tile, double &collisiondistance)
//{
//	//Get the dot product between the tile's normal and the ray's direction
//	double dotproduct = glm::dot(raydirection, tile.Normal);
//
//	double colldist;
//
//	//Check if it's parallel; a value of 0 indicates no collision
//	if (dotproduct < 0 && dotproduct >= 0)
//		return false;
//
//	glm::vec3 subtractvector = tile.Location - rayposition;
//
//	//Find the distance to the collision point
//	colldist = glm::dot(tile.Normal, subtractvector) / dotproduct;
//
//	//Check if the collision is behind the start
//	if (colldist <= 0)
//		return false;
//
//	collisiondistance = colldist;
//	return true;
//}

//glm::vec3 GetMinPoints(vector<glm::vec3> vertices)
//{
//	int x, y, z;
//	x = y = z = INFINITY;
//
//	for (int i = 0; i < vertices.size(); i++)
//	{
//		if (vertices[i].x < x) x = vertices[i].x;
//		if (vertices[i].y < y) y = vertices[i].y;
//		if (vertices[i].z < z) z = vertices[i].z;
//	}
//
//	return glm::vec3(x, y, z);
//}
//
//glm::vec3 GetMaxPoints(vector<glm::vec3> vertices)
//{
//	int x, y, z;
//	x = y = z = -INFINITY;
//
//	for (int i = 0; i < vertices.size(); i++)
//	{
//		if (vertices[i].x > x) x = vertices[i].x;
//		if (vertices[i].y > y) y = vertices[i].y;
//		if (vertices[i].z > z) z = vertices[i].z;
//	}
//
//	return glm::vec3(x, y, z);
//}