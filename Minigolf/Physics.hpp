#include "PhysicsObject.hpp"

void HitBall(PhysicsObject &obj, float directionAngle, float power);
void MovePhysicsObject(PhysicsObject &obj);
void DeceleratePhysicsObject(PhysicsObject & obj);
int FindCurrentTile(PhysicsObject &obj, int originTile, vector<Tile> tiles);
float FindYPos(PhysicsObject &obj);
bool WallCollision(PhysicsObject &obj);