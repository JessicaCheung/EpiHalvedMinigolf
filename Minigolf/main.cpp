#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <gl/glut.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

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



//The tee/cup
class TeeCup
{
public:
	//The tile ID of the tee/cup
	int TileID;

	//Coordinates of the tee/cup
	glm::vec3 Coordinates;

	//Vertices
	vector<glm::vec3> Vertices;

	//Normals
	vector<glm::vec3> Normals;

	TeeCup()
	{
		TileID = -1;
	}

	TeeCup(int tileID)
	{
		TileID = tileID;
	}

	void ChangeCoordinates(glm::vec3 coordinates)
	{
		Coordinates = coordinates;
	}

	void AddNormal(glm::vec3 normal)
	{
		Normals.push_back(normal);
	}

	void AddVertex(glm::vec3 vertex)
	{
		Vertices.push_back(vertex);
		AddNormal(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	void CreateCup(float radius, int slices, float height)
	{
		float degInc = 360 / slices;
		float j = 0.0f;
		AddVertex(glm::vec3(Coordinates.x, Coordinates.y + height, Coordinates.z));
		for (int i = 0; i < slices; i++)
		{
			AddVertex(glm::vec3(Coordinates.x + radius * cos(j), Coordinates.y + height,
				Coordinates.z + radius * sin(j)));
			j += degInc;
		}

		//Normals
		for (int i = 0; i < Vertices.size() - 3; i++)
		{
				glm::vec3 a = Vertices[i];
				glm::vec3 b = Vertices[i + 1];
				glm::vec3 c = Vertices[i + 2];

				glm::vec3 u = a - b;
				glm::vec3 v = a - c;

				glm::vec3 norm = glm::normalize(glm::cross(u, v));		//Face normal

				//Add previous vertex normal to face normal for new vertex normal
				Normals[i] = glm::normalize(Normals[i] + norm);
				Normals[i + 1] = glm::normalize(Normals[i + 1] + norm);
				Normals[i + 2] = glm::normalize(Normals[i + 2] + norm);
			}
		}

	void PrintCoordinates()
	{
		cout << "Coordinates:" << endl;
		for (int i = 0; i < Coordinates.length(); i++)
			cout << Coordinates[i] << ", ";
		cout << endl;
	}

	void PrintVertices()
	{
		for (int i = 0; i < Vertices.size(); i++)
		{
			cout << "Vertex: " << i << endl;
			cout << Vertices[i].x << ", " << Vertices[i].y << ", " << Vertices[i].z << endl;
		}
	}

	void PrintNormals()
	{
		for (int i = 0; i < Normals.size(); i++)
		{
			cout << "Normals: " << i << endl;
			cout << Normals[i].x << ", " << Normals[i].y << ", " << Normals[i].z << endl;
		}
	}
};

//A tile
class Tile
{
public:
	//The tile's ID
	int TileID;

	//The tile's neighbors
	vector<int> Neighbors;

	//The tile's vertices
	vector<glm::vec3> Vertices;

	//The tile's normals
	vector<glm::vec3> Normals;

	Tile()
	{
		TileID = -1;
	}

	Tile(int tileID)
	{
		TileID = tileID;
	}

	void AddNeighbor(int neighbor)
	{
		Neighbors.push_back(neighbor);
	}

	void AddNormal(glm::vec3 normal)
	{
		Normals.push_back(normal);
	}

	void AddVertex(glm::vec3 vertex)
	{
		Vertices.push_back(vertex);
		AddNormal(glm::vec3(0.0f, 0.0f, 0.0f));
	}


	void PrintVertices()
	{
		for (int i = 0; i < Vertices.size(); i++)
		{
			cout << "Vertex: " << i << endl;
			cout << Vertices[i].x << ", " << Vertices[i].y << ", " << Vertices[i].z << endl;
		}
	}

	void PrintNormals()
	{
		for (int i = 0; i < Normals.size(); i++)
		{
			cout << "Normals: " << i << endl;
			cout << Normals[i].x << ", " << Normals[i].y << ", " << Normals[i].z << endl;
		}
	}

	void PrintNeighbors()
	{
		for (int i = 0; i < Neighbors.size(); i++)
		{
			cout << "Neighbor: " << i << endl;
			cout << Neighbors[i] << endl;
		}
	}
};

//Our tiles
vector<Tile> Tiles;

//The tee
TeeCup Tee;

//The cup
TeeCup Cup;

//Splits a string based on a specified character
//This function was obtained from here: http://stackoverflow.com/a/53878
vector<string> SplitString(const char *str, char c = ' ')
{
    vector<string> result;

    do
    {
		//Start at the first character in our current sequence; after each loop, this will be the first character after the delimiter (Ex. "h" the first time then "c" after one loop in the string "hello chris")
        const char *begin = str;

		//If the current character is not the delimiter (a space character in our case) and the pointer isn't null, move onto the next character
        while(*str != c && *str)
            str++;

		//We encountered the delimiter or a null character, so construct a new string based on the characters we had before this condition and add that to our vector
        result.push_back(string(begin, str));

	//Make sure the next character isn't a null character, which would end the string
    } while (*str++ != 0);

    return result;
}

//Parses the tee/cup
void ParseTeeCup(vector<string> lines, TeeCup& teecup)
{
	//Get the tile ID
	int tileID = stoi(lines[1]);

	glm::vec3 teecupcoordinates;

	//Read in the coordinates for the tee/cup
	for (int i = 0; i < teecup.Coordinates.length(); i++)
	{
		istringstream(lines[i + 2]) >> teecupcoordinates[i];
	}

	//Change the coordinates accordingly
	teecup.ChangeCoordinates(teecupcoordinates);
}

//Parses a tile
void ParseTile(vector<string> lines)
{
	//Get the tile ID and number of edges
	int tileID = stoi(lines[1]);
	int numedges = stoi(lines[2]);
	
	int vertexpoints = 3;
	
	//Make a new tile with its unique ID
	Tile newTile(tileID);
	
	//Go through the edges and store the vertices
	for (int i = 0; i < (numedges * vertexpoints); i += vertexpoints)
	{
		int curposition = i + vertexpoints;
	
		//Create a new vector3
		glm::vec3 vertex;
		
		//Load in all the following values 
		for (int j = 0; j < vertexpoints; j++)
		{
			istringstream(lines[curposition + j]) >> vertex[j];
		}
	
		newTile.AddVertex(vertex);
	}
	
	//Get the current position in the line for easy access; at this point it starts reading in the neighbors
	int neighborposition = vertexpoints + (numedges * vertexpoints);
	
	//Read in the neighbors
	for (int i = neighborposition; i < lines.size(); i++)
	{
		//Read in the neighbor value; check for a negative number afterwards (stoi does not support negative numbers from what I read)
		int neighbor = stoi(lines[i]);
		if (lines[i].at(0) == '-') neighbor = -neighbor;
	
		//Add the neighbor
		newTile.AddNeighbor(neighbor);
	}
	
	//Add the tile to our list
	Tiles.push_back(newTile);
}

//Parse the line and store the vertices
void ParseLine(vector<string> lines)
{
	//Check for a tile
	if (lines[0] == "tile")
	{
		ParseTile(lines);
	}
	else if (lines[0] == "tee")
	{
		ParseTeeCup(lines, Tee);
	}
	else if (lines[0] == "cup")
	{
		ParseTeeCup(lines, Cup);
	}
	//else 
	//{
	//	cout << "\n" << "Invalid Input!" << endl;
	//	exit(1);
	//}
}

//Read vertices from the file
void ReadVertices(fstream& file)
{
	while (file.eof() == false)
	{
		string line;

		//Read the line until you find a new line
		getline(file, line);

		//Split the string
		vector<string> lines = SplitString(line.c_str());

		//Parse the lines
		ParseLine(lines);
	}
}

//Check if the file exists
void ReadFile(string filename)
{
	fstream file;

	file.open(filename);

	//If it failed to open the file, send a message
	if (file.fail() == true)
	{
		cout << "Invalid input!" << endl;
	}
	//Otherwise, read the vertices
	else
	{
		ReadVertices(file);
	}

	file.close();
}

void CalculateNormals()
{

	//Initialize
	for (int i = 0; i < Tiles.size(); i++)
	{
		for (int j = 0; j < Tiles[i].Vertices.size() - 3; j++)
		{
			glm::vec3 a = Tiles[i].Vertices[j];
			glm::vec3 b = Tiles[i].Vertices[j + 1];
			glm::vec3 c = Tiles[i].Vertices[j + 2];

			glm::vec3 u = a - b;
			glm::vec3 v = a - c;

			glm::vec3 norm = glm::normalize(glm::cross(u, v));		//Face normal

			//Add previous vertex normal to face normal for new vertex normal
			Tiles[i].Normals[j] = glm::normalize(Tiles[i].Normals[j] + norm);
			Tiles[i].Normals[j + 1] = glm::normalize(Tiles[i].Normals[j + 1] + norm);
			Tiles[i].Normals[j + 2] = glm::normalize(Tiles[i].Normals[j + 2] + norm);
		}
	}
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


void initRendering(char** argv)
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 0.0, 5.0, 0.0, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//Create a an std::string using the arguments passed in (I don't like C strings)
	//string arguments(argv[1]);

	string arguments = "hole.00.db";

	ReadFile(arguments);
	CalculateNormals();
	Cup.CreateCup(0.1, 20, 0.05f);
	//Print all the tile vertices
	for (int i = 0; i < Tiles.size(); i++)
	{
		cout << "Tile: " << i << " ID = " << Tiles[i].TileID << endl;

		Tiles[i].PrintVertices();
		Tiles[i].PrintNormals();
		Tiles[i].PrintNeighbors();

		cout << endl;
	}

	//Print the tee and cup coordinates
	cout << "Tee:" << endl;
	Tee.PrintCoordinates();
	cout << endl;
	cout << "Cup:" << endl;
	Cup.PrintCoordinates();
	cout << endl;
	Cup.PrintVertices();
	Cup.PrintNormals();
}

void display()
{
	//Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 2.0, 3.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	glTranslatef(translate[0], translate[1], translate[2]);
	glRotatef(rotateVal[0], 1, 0, 0);
	glRotatef(rotateVal[1], 0, 1, 0);
	glRotatef(rotateVal[2], 0, 0, 1);
	glScalef(scaleVal[0], scaleVal[1], scaleVal[2]);

	//Tiles
	for (int i = 0; i < Tiles.size(); i++)
	{
		glBegin(GL_TRIANGLE_FAN);

		for (int j = 0; j < Tiles[i].Vertices.size(); j++)
		{
			glColor3f(0.0f, 1.0f, 0.0f);
			glNormal3f(Tiles[i].Normals[j].x, Tiles[i].Normals[j].y, Tiles[i].Normals[j].z);
			glVertex3f(Tiles[i].Vertices[j].x, Tiles[i].Vertices[j].y, Tiles[i].Vertices[j].z);
		}
		glEnd();
	}

	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < Cup.Vertices.size(); i++)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glNormal3f(Cup.Normals[i].x, Cup.Normals[i].y, Cup.Normals[i].z);
		glVertex3f(Cup.Vertices[i].x, Cup.Vertices[i].y, Cup.Vertices[i].z);
	}
	glEnd();

	//Send it to the screen
	glutSwapBuffers();
}

//Pass a file name into argv
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