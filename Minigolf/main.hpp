#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/intersect.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#define _USE_MATH_DEFINES
#define PI 3.141592653589793238463f
#define MU -0.003f
using namespace std;

void initRendering(char** argv);
void setShaders();
void Update();
void display();
void drawGUIText(char* s, int x, int y);
void handleKeyboard(unsigned char key, int x, int y);
void handleResize(int w, int h);
void handle_menu(int ID);
void handle_motion(int x, int y);
void handle_mouse(int b, int s, int x, int y);
void glutMouseWheel(int wheel, int direction, int x, int y);
GLuint* getVAO();
GLuint getShader();
glm::vec3 getNewCoor();
GLuint getModelView();
glm::mat4 getmodel_view();
glm::vec2 getPointerBar();