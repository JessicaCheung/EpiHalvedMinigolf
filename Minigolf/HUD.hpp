#include "main.hpp"

//Total score the player has on all the holes combined
int CurrentScore = 0;

//The remaining distance to the hole; change it to a float if necessary
int DistanceToHole = 0;

//Par value assigned to each hole. Basically the number of strokes that a golfer should require to complete a hole
//If you complete the hole in 1 less stroke (-1) than the Par requires, you get a Birdie. 2 under (-2) is an Eagle, 3 (-3) under is an Albatross, and 4 (-4)
//under is a Condor. Anything above is a Bogey (+1) or a variation (double-bogey is +2, triple-bogey is +3, and so on). The lower your score, the better
int Par = 0;

//Current stroke; increase this each time you hit the ball
int Stroke = 0;

const float MaxPower = 100.0f;
const float MinPower = 0.0f;
float CurPower = 0.0f;

//The rate of increasing power; it goes up and down, starting at up until it reaches the top of the bar, then starts decreasing until it hits the bottom, after which it goes up again and continues the cycle
float CurRate = 1.0f;

//The direction the ball will be shot in; defaults to straight forward
glm::vec3 ShootDirection = glm::vec3(0, 0, 1);

//Renders a string; no clue how well it actually works
void RenderString(float x, float y, void *font, const unsigned char* text, glm::vec3 color);

//Renders the power bar according to the current power in relation to the max power
void RenderPowerBar();

//Changes the scale of the power bar according to the current rate and depending on if the mouse is held or not
void ScalePowerBar();