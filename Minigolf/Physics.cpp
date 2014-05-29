#include "Physics.hpp"

void HitBall(PhysicsObject &obj, float directionAngle, float power)
{
	float temp = directionAngle * PI / 180;
	obj.Direction = glm::vec3(sin(temp), 0, cos(temp));
	obj.AddForce(glm::vec3(power, 0, power));
}

void MovePhysicsObject(PhysicsObject &obj)
{	obj.Model.Coordinate.y = FindYPos(obj);
	//cout << obj.Model.Coordinate.x << " " << obj.Model.Coordinate.y << " " << obj.Model.Coordinate.z << " | ";
	obj.Model.Coordinate +=  obj.Velocity * obj.Direction;
	//cout << obj.Model.Coordinate.x << " " << obj.Model.Coordinate.y << " " << obj.Model.Coordinate.z << endl;
	//obj.AddForce(-MU * obj.ObjectTile.Normal);
}

void DeceleratePhysicsObject(PhysicsObject &obj)
{
	float rate = 0.05;
	if (obj.Velocity.x > 0.0)
	{
		obj.Velocity.x -= obj.Velocity.x * rate;
	}
	else
	{
		obj.Velocity.x = 0.0f;
	}
	if (obj.Velocity.y > 0.0)
	{
		obj.Velocity.y -= obj.Velocity.y * rate;
	}
	else
	{
		obj.Velocity.y = 0.0f;
	}
	if (obj.Velocity.z > 0.0)
	{
		obj.Velocity.z -= obj.Velocity.z * rate;
	}
	else
	{
		obj.Velocity.z = 0.0f;
	}
}

int FindCurrentTile(PhysicsObject &obj, int originTile, vector<Tile> tiles)
{
	float sumTheta = 0.0f;
	float temp;
	for (int i = 0; i < tiles[originTile - 1].Neighbors.size(); i++)
	{
		temp = tiles[originTile - 1].Neighbors[i];
		sumTheta = 0.0f;
		if (temp != 0)
		{
			for (int j = 0; j < tiles[temp - 1].Vertices.size(); j++)
			{
				glm::vec3 A(tiles[temp - 1].Vertices[j] - obj.Model.Coordinate);
				A.y = 0.0f;
				glm::vec3 B(0, 0, 0);
				if (j == tiles[temp - 1].Vertices.size() - 1)
				{
					B = glm::vec3(tiles[temp - 1].Vertices[0] - obj.Model.Coordinate);
					B.y = 0.0f;
				}
				else
				{
					B = glm::vec3(tiles[temp - 1].Vertices[j + 1] - obj.Model.Coordinate);
					B.y = 0.0f;
				}
				float theta = acos(glm::dot(A, B) / (glm::length(A) * glm::length(B)));
				theta = theta * 180 / PI;
				sumTheta += theta;
			}
			//cout << "neighbor" << temp << " | " << sumTheta << endl;
		}
		
		if (sumTheta == 360.0f)
		{
 			return temp;
		}
	}
	return originTile;
}

float FindYPos(PhysicsObject &obj)
{
	float y = (glm::dot(obj.ObjectTile.Normal, obj.ObjectTile.Vertices[0]) - 
		obj.ObjectTile.Normal.x * obj.Model.Coordinate.x -
		obj.ObjectTile.Normal.z * obj.Model.Coordinate.z) / obj.ObjectTile.Normal.y;
	return y;
}

bool WallCollision(PhysicsObject &obj)
{
	ImportObj Walls = getWallsBuffer();
	glm::vec3 pos(0, 0, 0), temp(0, 0, 0);

	for (int i = 0; i < Walls.Indices.size(); i += 3)
	{
		if (glm::intersectRayTriangle(obj.Model.Coordinate, obj.Direction,
			Walls.Vertices[Walls.Indices[i]], Walls.Vertices[Walls.Indices[i + 1]], Walls.Vertices[Walls.Indices[i + 2]], pos))
		{
			/*cout << i << " " << obj.Direction.x << " " << obj.Direction.y << " " << obj.Direction.z << " | " 
				<< dist << endl;*/
			//cout << obj.Direction.x << " " << obj.Direction.y << " " << obj.Direction.z << " | " << 
			//obj.Model.Coordinate.x << " " << obj.Model.Coordinate.y << " " << obj.Model.Coordinate.z << " " <<
			cout << i << " " << glm::length(pos) - pos.y << " | " << pos.x << " " << pos.y << " " << pos.z << endl;
			//if (dist >= 1.88f && dist < 2.0f)
			if (glm::length(pos) - pos.y < 0.09f)
			{
				obj.NegativeForce();
			}
		}
	}
	return false;
}

	//if (glm::intersectRayTriangle(obj.Model.Coordinate, obj.Direction,
	//	Walls.Vertices[Walls.Indices[0]], Walls.Vertices[Walls.Indices[1]], Walls.Vertices[Walls.Indices[2]], pos))
	//{
	//	dist = glm::distance(obj.Model.Coordinate, pos);
	//	cout << "1 " << "dist " << dist << endl;
	//	if (dist >= 2.0f)
	//	{
	//		cout << "1" << endl;
	//		return true;
	//	}
	//}
	//if (glm::intersectRayTriangle(obj.Model.Coordinate, obj.Direction,
	//	Walls.Vertices[Walls.Indices[3]], Walls.Vertices[Walls.Indices[4]], Walls.Vertices[Walls.Indices[5]], pos))
	//{
	//	dist = glm::distance(obj.Model.Coordinate, pos);
	//	cout << "2 " << "dist " << dist << endl;
	//	if (dist >= 2.0f)
	//	{
	//		cout << "2" << endl;
	//		return true;
	//	}
	//}
	//if (glm::intersectRayTriangle(obj.Model.Coordinate, obj.Direction,
	//	Walls.Vertices[Walls.Indices[6]], Walls.Vertices[Walls.Indices[7]], Walls.Vertices[Walls.Indices[8]], pos))
	//{
	//	dist = glm::distance(obj.Model.Coordinate, pos);
	//	cout << "3 " << "dist " << dist << endl;
	//	if (dist >= 2.0f)
	//	{
	//		cout << "3" << endl;
	//		return true;
	//	}
	//}