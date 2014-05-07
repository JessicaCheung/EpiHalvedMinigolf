#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <GL/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

void handleResize(int w, int h);
void handle_menu(int ID);
void handle_motion(int x, int y);
void handle_mouse(int b, int s, int x, int y);
void initRendering(char** argv);
void display();