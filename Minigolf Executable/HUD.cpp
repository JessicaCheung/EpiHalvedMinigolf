#include "HUD.hpp"

//Code taken from http://stackoverflow.com/a/538707 but slightly modified for correctness
void RenderString(float x, float y, void *font, const unsigned char* text, glm::vec3 color)
{
	glColor3f(color.r, color.g, color.b);
	glRasterPos2f(x, y);

	glutBitmapString(font, text);
}

void RenderPowerBar()
{
	//Drawing code not here, but you'd want to scale it like this:
	float scale = (CurPower / MaxPower);

	//Draw code here
}

void ScalePowerBar()
{
	/*if (mouse left button down)		
	  {


	  }
	  else CurRate = 1.0f;
	*/

	//Increase the current power
	CurPower += CurRate;

	if (CurRate > 0 && CurPower >= MaxPower)
	{
		CurPower = MaxPower;
		CurRate = -CurRate;
	}
	else if (CurRate < 0 && CurPower <= MinPower)
	{
		CurPower = MinPower;
		CurRate = -CurRate;
	}
}