#include "Ball.hpp"

class Ray
{
public:
	glm::vec3 RayOrigin;
	glm::vec3 RayDirection;

	float RayDistance;

	Ray(glm::vec3 origin, glm::vec3 direction, float distance)
	{
		RayOrigin = origin;
		RayDirection = direction;
		RayDistance = distance;
	}

	//Shoot a ray from a point in a certain direction for a certain distance; the default value of 0 indicates an infinite distance
	glm::vec3 Raycast()
	{
		return RayOrigin + (RayDistance * RayDirection);
	}
};