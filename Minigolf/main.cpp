#include "Map.hpp"

typedef enum {
	TRANSLATE,
	ROTATE_X,
	ROTATE_Y,
	ROTATE_Z,
	SCALE
} mode;

int    btn[3] = { 0 };		// Current button state
mode   cur_mode = TRANSLATE;		// Current mouse mode
float  translate[3] = { 0 };		// Current translation values
float  rotateVal[3] = { 0 };		// Current rotation values
float scaleVal[3] = { 1.0f, 1.0f, 1.0f };		//Current scale values
int    mouse_x, mouse_y;		// Current mouse position


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);

	glutCreateWindow("minigolf");
	initRendering(argv);

	glutDisplayFunc(display);
	//glutKeyboardFunc(handleKey
	glutReshapeFunc(handleResize);
	glutMotionFunc(handle_motion);
	glutMouseFunc(handle_mouse);

	glutCreateMenu(handle_menu);	// Setup GLUT popup menu
	glutAddMenuEntry("Translate", 0);
	glutAddMenuEntry("Rotate X", 1);
	glutAddMenuEntry("Rotate Y", 2);
	glutAddMenuEntry("Rotate Z", 3);
	glutAddMenuEntry("Scale", 4);
	glutAddMenuEntry("Quit", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}

void initRendering(char** argv)
{
	/*GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };*/
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	/*glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
	GLfloat global_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	ReadMap("hole.02.db");
	//ReadMap("testcourse3.db");
}

void display()
{
	//Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(0.0, 2.0, 3.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(translate[0], translate[1], translate[2]);
	glRotatef(rotateVal[0], 1, 0, 0);
	glRotatef(rotateVal[1], 0, 1, 0);
	glRotatef(rotateVal[2], 0, 0, 1);
	glScalef(scaleVal[0], scaleVal[1], scaleVal[2]);

	//Draw
	RenderMap();

	//Send it to the screen
	glutSwapBuffers();
}

void handleResize(int w, int h)
{

	//Convert from coordinates to pixel values
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //Set camera to perspective

	//Reset camera
	glLoadIdentity();

	//camera angle, width-to-height ratio, and near z and far z clipping coordinates
	gluPerspective(85.0, (double)w / (double)h, .10, 200.0);
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

	glutPostRedisplay();
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
		switch (b)
		{
		case GLUT_LEFT_BUTTON:
			if (cur_mode == SCALE)
			{
				scaleVal[0] -= 0.1f;
				scaleVal[1] -= 0.1f;
				scaleVal[2] -= 0.1f;

			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if (cur_mode == SCALE)
			{
				scaleVal[0] += 0.1f;
				scaleVal[1] += 0.1f;
				scaleVal[2] += 0.1f;
			}
		}


	}
	else {
		btn[b] = 0;
	}

	mouse_x = x;
	mouse_y = glutGet(GLUT_WINDOW_HEIGHT) - y;
}					// End routine handle_mouse
