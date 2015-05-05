//#include "Map.hpp"
//#include "Hole.hpp"
#include "Physics.hpp"

typedef enum {
	TRANSLATE,
	ROTATE_X,
	ROTATE_Y,
	ROTATE_Z,
	SCALE
} mode;

typedef enum {
	TOPVIEW,
	FREELOOK,
	THIRDPOV
} camera;

int const width = 800;
int const height = 600;
int    btn[3] = { 0 };		// Current button state
mode   cur_mode = TRANSLATE;		// Current mouse mode
float  translate[3] = { 0 };		// Current translation values
float  rotateVal[3] = { 0 };		// Current rotation values
float scaleVal[3] = { 1.0f, 1.0f, 1.0f };		//Current scale values
int    mouse_x, mouse_y;		// Current mouse position
camera cameraView = FREELOOK;
float BallDirAngle = 180.0f;
float GolfPower = 0.1f;
float const powerIncrements = 0.025f;
int numShots = 0;


GLuint vao[6];
GLuint ModelView, Projection;
glm::mat4 model_view;
GLuint shadertemp;

//Change in time after each clock tick
float deltaT = 0.0f;

//Total game time
float gameTime = 0.0f;
float prevgameTime = 0.0f;

//Run physics 60 frames per second
const float fixedupdatetime = 40.0f;//1.0f / 60.0f;

//Physics lag time; we need to see how many frames of physics simulations we need to go through to catch up
//This ensures that the game behaves the same on any hardware
float physicsLagTime = 0.0f;

//The golf ball (NOTE: This should be moved into Map later)
Ball GolfBall;
vector<Hole> HolesList;

//The holes, or levels the player plays through
//vector<Hole> Holes;
int currentHole = 0;
string parstr, numshots;
//The main update function
void Update()
{
	//Update gametime
	gameTime = glutGet(GLUT_ELAPSED_TIME);

	//Check how much time has passed between clock ticks, in milliseconds
	deltaT = (gameTime - prevgameTime);
	physicsLagTime += deltaT;

	//See how many physics simulations we need to go through
	//Ideally it should be 1, but this makes it so if there's lag anywhere, everything will still behave the same way
	while (physicsLagTime > fixedupdatetime)
	{
		//Perform a physics simulation
		MovePhysicsObject(GolfBall);
		WallCollision(GolfBall, HolesList[currentHole].Walls);

		int temp = FindCurrentTile(GolfBall, GolfBall.ObjectTile.TileID, HolesList[currentHole].Tiles);
		GolfBall.ObjectTile = HolesList[currentHole].Tiles[temp - 1];

		physicsLagTime -= fixedupdatetime;
	}

	//Set the previous game time to the current game time
	prevgameTime = gameTime;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);

	glutCreateWindow("minigolf");
	glewInit();
	initRendering(argv);

	//Render everything
	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeyboard);
	glutReshapeFunc(handleResize);
	glutMotionFunc(handle_motion);
	glutMouseFunc(handle_mouse);
	glutMouseWheelFunc(glutMouseWheel);

	glutCreateMenu(handle_menu);	// Setup GLUT popup menu
	glutAddMenuEntry("Translate", 0);
	glutAddMenuEntry("Rotate X", 1);
	glutAddMenuEntry("Rotate Y", 2);
	glutAddMenuEntry("Rotate Z", 3);
	glutAddMenuEntry("Scale", 4);
	glutAddMenuEntry("Quit", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//The update loop
	glutIdleFunc(Update);
	glutMainLoop();

	return 0;
}

void initRendering(char** argv)
{
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	ReadMap("course.db");

	HolesList = getHoles();
	InitBall();
}

void setLighting(GLuint program)
{
	//Initialize shader lighting parameters
	glm::vec4 light_position(0.0, 1.0, 0.0, 1.0);
	glm::vec4 light_ambient(0.2, 0.2, 0.2, 1.0);
	glm::vec4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	glm::vec4 light_specular(1.0, 1.0, 1.0, 1.0);
	glm::vec4 material_ambient(1.0, 1.0, 1.0, 1.0);
	glm::vec4 material_diffuse(1.0, 1.0, 1.0, 1.0);
	glm::vec4 material_specular(1.0, 1.0, 1.0, 1.0);
	float  material_shininess = 50.0;
	glm::vec4 ambient_product = light_ambient * material_ambient;
	glm::vec4 diffuse_product = light_diffuse * material_diffuse;
	glm::vec4 specular_product = light_specular * material_specular;
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, (GLfloat*)&ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, (GLfloat*)&diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, (GLfloat*)&specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, (GLfloat*)&light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	//End lighting
}

void BindShader(GLuint vao, GLuint program, vector<glm::vec3> Vertices, vector<glm::vec3> Normals, vector<int> Indices)
{
	GLuint buffer[3];
	glGenBuffers(3, buffer);
	GLuint vPosition, vNormal;

	//Vertex binding
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3), Vertices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(glm::vec3), Normals.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), Indices.data(), GL_DYNAMIC_DRAW);
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	//Set up vertex arrays
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}

void setShaders()
{
	ImportObj Pointer = getPointer();
	GLuint program = LoadShaders("vshader5.glsl", "fshader5.glsl");
	shadertemp = program;
	glUseProgram(program);
	GLuint vPosition, vNormal;

	//Set lighting
	setLighting(program);

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");

	glGenVertexArrays(6, vao);

	BindShader(vao[0], program, HolesList[currentHole].TileBuffer.Vertices, HolesList[currentHole].TileBuffer.Normals, HolesList[currentHole].TileBuffer.Indices);
	BindShader(vao[1], program, HolesList[currentHole].Tee.Vertices, HolesList[currentHole].Tee.Normals, HolesList[currentHole].Tee.Indices);
	BindShader(vao[2], program, HolesList[currentHole].Cup.Vertices, HolesList[currentHole].Cup.Normals, HolesList[currentHole].Cup.Indices);
	BindShader(vao[3], program, GolfBall.Model.Vertices, GolfBall.Model.Normals, GolfBall.Model.Indices);
	BindShader(vao[4], program, HolesList[currentHole].Walls.Vertices, HolesList[currentHole].Walls.Normals, HolesList[currentHole].Walls.Indices);
	BindShader(vao[5], program, Pointer.Vertices, Pointer.Normals, Pointer.Indices);
}

void display()
{
	setShaders();
	//Clear screen
	glClearColor(0.678431, 0.847059, 0.901961, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::vec3 eye(0.0f, 5.0f, 5.0f);
	glm::vec3 at(0.0f, 0.0f, 0.0f);
	switch (cameraView)
	{
	case TOPVIEW:
		eye = glm::vec3(0.0f, 10.0f, 0.01f);
		at = glm::vec3(0.0f, 0.0f, 0.0f);
		break;
	case FREELOOK:
		eye = glm::vec3(0.0f, 5.0f, 5.0f);
		at = glm::vec3(0.0f, 0.0f, 0.0f);
		break;
	case THIRDPOV:
		eye = glm::vec3(GolfBall.Model.Coordinate.x + 0.0f, GolfBall.Model.Coordinate.y + 0.4f, GolfBall.Model.Coordinate.z + 0.75f);
		at = glm::vec3(GolfBall.Model.Coordinate.x + 0.0f, GolfBall.Model.Coordinate.y + 0.2f, GolfBall.Model.Coordinate.z);
		break;
	}
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	model_view = glm::lookAt(eye, at, up) *
		glm::scale(glm::vec3(scaleVal[0], scaleVal[1], scaleVal[2])) *			//Scale
		glm::translate(glm::vec3(translate[0], translate[1], translate[2])) *	//Translate 
		glm::rotate(rotateVal[0], glm::vec3(1, 0, 0)) *							//Rotate X
		glm::rotate(rotateVal[1], glm::vec3(0, 1, 0)) *							//Rotate Y
		glm::rotate(rotateVal[2], glm::vec3(0, 0, 1));							//Rotate Z
	glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(model_view));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(Projection, 1, GL_FALSE, glm::value_ptr(projection));
	//Draw
	RenderMap();
	DisplayMap(3, GolfBall.Model.Indices.size());
	drawGUIText("Hole Name: ", 0, 580);
	drawGUIText(&HolesList[currentHole].HoleName[0], 100, 580);
	drawGUIText("Par: ", 0, 560);
	drawGUIText(&parstr[0], 40, 560);
	drawGUIText("Shots: ", 0, 540);
	numshots = to_string(numShots);
	drawGUIText(&numshots[0], 60, 540);
	//Send it to the screen
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void drawGUIText(char* s, int x, int y)
{
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);
	void * font = GLUT_BITMAP_HELVETICA_18;
	glColor3f(0.0f, 0.0f, 0.0f);
	glutBitmapString(font, (const unsigned char *) s);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void handleKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 015:		//Return
		cout << "Pressed" << endl;
		if (currentHole >= HolesList.size() - 1)
			currentHole = 0;
		else
		{
			currentHole++;
			numShots = 0;
			InitBall();
		//	GolfBall
		}
		break;
	case 033:		//Escape
		exit(EXIT_SUCCESS);
		break;
	case 040:		//Space
		HitBall(GolfBall, BallDirAngle, GolfPower);
		numShots++;
		break;
	case 'a':
		if (BallDirAngle <= 0.0)
			BallDirAngle = 360.0f;
		BallDirAngle -= 8.0f;
		break;
	case 'd':
		if (BallDirAngle >= 360.0)
			BallDirAngle = 0.0f;
		BallDirAngle += 8.0f;
		break;
	case 'w':
		GolfPower += powerIncrements;
		if (GolfPower >= 0.1)
			GolfPower = 0.1f;
		break;
	case 's':
		GolfPower -= powerIncrements;
		if (GolfPower <= 0.025)
			GolfPower = 0.025;
		break;
	case 'r':
		GolfBall.ReturnTo();
		break;
	case 't':		//Top view
		cameraView = TOPVIEW;
		break;
	case 'f':		//Free look
		cameraView = FREELOOK;
		break;
	case 'g':		//Third Person View
		cameraView = THIRDPOV;
		break;
	case 'p':
		GolfBall.Model.Coordinate.x = 1.5;
		break;
	}
}

void handleResize(int w, int h)
{

	//Convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	GLfloat aspect = GLfloat(w) / h;
	glm::mat4 projection = glm::perspective(45.0f, aspect, 0.1f, 60.0f);
	glUniformMatrix4fv(Projection, 1, GL_FALSE, glm::value_ptr(projection));
}

void handle_menu(int ID)

//  This routine handles popup menu selections
//
//  ID:  Menu entry ID
{
	switch (ID) {
	case 0:				// XY translation
		cur_mode = TRANSLATE;
		break;
	case 1:				// X rotation
		cur_mode = ROTATE_X;
		break;
	case 2:				// Y rotation
		cur_mode = ROTATE_Y;
		break;
	case 3:				// Z rotation
		cur_mode = ROTATE_Z;
		break;
	case 4:				//xyz scale
		cur_mode = SCALE;
		break;
	case 5:				// Quit
		exit(0);
	}
}					// End routine handle_menu


void handle_motion(int x, int y)

//  This routine acts as a callback for GLUT mouse motion events
//
//  x, y:  Cursor position
{
	float	 x_ratchet;			// X ratchet value
	float	 y_ratchet;			// Y ratchet value


	if (!btn[0]) {			// Left button not depressed?
		return;
	}

	x_ratchet = glutGet(GLUT_WINDOW_WIDTH) / 10.0;
	y_ratchet = glutGet(GLUT_WINDOW_HEIGHT) / 10.0;

	//  Windows XP has y = 0 at top, GL has y = 0 at bottom, so reverse y

	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	switch (cur_mode) {
	case TRANSLATE:			// XY translation
		translate[0] += (float)(x - mouse_x) / x_ratchet;
		translate[1] += (float)(y - mouse_y) / y_ratchet;
		break;
	case ROTATE_X:			// X rotation
		x_ratchet /= 10.0;
		rotateVal[0] += (float)(x - mouse_x) / x_ratchet;
		break;
	case ROTATE_Y:			// Y rotation
		x_ratchet /= 10.0;
		rotateVal[1] += (float)(x - mouse_x) / x_ratchet;
		break;
	case ROTATE_Z:			// Z rotation
		x_ratchet /= 10.0;
		rotateVal[2] += (float)(x - mouse_x) / x_ratchet;
		break;
	}

	mouse_x = x;				// Update cursor position
	mouse_y = y;
}					// End routine handle_motion


void handle_mouse(int b, int s, int x, int y)

//  This routine acts as a callback for GLUT mouse events
//
//  b:     Mouse button (left, middle, or right)
//  s:     State (button down or button up)
//  x, y:  Cursor position
{
	if (s == GLUT_DOWN) {		// Store button state if mouse down
		btn[b] = 1;
	}
	else {
		btn[b] = 0;
	}
	mouse_x = x;
	mouse_y = glutGet(GLUT_WINDOW_HEIGHT) - y;
}					// End routine handle_mouse

void glutMouseWheel(int wheel, int direction, int x, int y)
{
	if (direction != 0)
	{
		if (direction < 0 && cur_mode == SCALE)		//Down
		{
			scaleVal[0] -= 0.1f;
			scaleVal[1] -= 0.1f;
			scaleVal[2] -= 0.1f;
		}
		else if (cur_mode == SCALE)
		{
			scaleVal[0] += 0.1f;
			scaleVal[1] += 0.1f;
			scaleVal[2] += 0.1f;
		}
	}
}

void InitBall()
{
	int TeeTileID = HolesList[currentHole].Tee.TileID;
	ImportObj temp;
	ImportObj Tee = getTeeBuffer();
	load_obj("BallSmall.obj", temp.Vertices, temp.Indices, glm::vec3(0, 0.08, 0));
	temp.CalculateNormals();
	GolfBall = Ball(temp, HolesList[currentHole].Tee.Coordinate, HolesList[currentHole].Tiles[TeeTileID - 1]);
	//_itoa(HolesList[currentHole].Par, parstr, 10);
	parstr = to_string( HolesList[currentHole].Par);
}

GLuint* getVAO()
{
	return vao;
}

GLuint getShader()
{
	return shadertemp;
}

glm::vec3 getNewCoor()
{
	return GolfBall.Model.Coordinate;
}

GLuint getModelView()
{
	return ModelView;
}

glm::mat4 getmodel_view()
{
	return model_view;
}

glm::vec2 getPointerBar()
{
	return glm::vec2(BallDirAngle, GolfPower);
}

int getCurrentHole()
{
	return currentHole;
}