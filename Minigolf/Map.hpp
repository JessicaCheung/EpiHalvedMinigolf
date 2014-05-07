#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <GL/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

class MapObject
{
public:
	int TileID;						//Tile ID of object
	vector<glm::vec3> Vertices;		//List of vertices
	vector<int> Indices;			//Indices to draw vertices+normals

	/*Adds a vertex to the list of vertices and an empty normal*/
	void AddVertex(glm::vec3 vert)
	{
		Vertices.push_back(vert);
	}

	/*Get size of list of vertices*/
	int SizeofVertices()
	{
		return Vertices.size();
	}

	/*Adds an index to the list of indices*/
	void AddIndex(int num)
	{
		Indices.push_back(num);
	}

	/*Prints the list of vertices*/
	void PrintVertices()
	{
		for (int i = 0; i < Vertices.size(); i++)
		{
			cout << "Vertex: " << i << endl;
			cout << Vertices[i].x << ", " << Vertices[i].y << ", " << Vertices[i].z << endl;
		}
	}

	/*Prints the list of indices*/
	void PrintIndices()
	{
		for (int i = 0; i < Indices.size(); i++)
		{
			cout << "Index: " << i << endl;
			cout << Indices[i] << ", " << Indices[i] << ", " << Indices[i] << endl;
		}
	}
};

class ImportObj : public MapObject
{
public:
	glm::vec3 Coordinate;		//Position of Tee
	vector<glm::vec3> Normals;	//List of normals

	//Constructors
	ImportObj()
	{
		TileID = -1;
	}
	ImportObj(int id)
	{
		TileID = id;
	}

	/*Changes the coordinate*/
	void ChangeCoordinate(glm::vec3 coor)
	{
		Coordinate = coor;
	}

	/*Initializes all normals to 0*/
	void InitNormals()
	{
		for (int i = 0; i < Vertices.size(); i++)
		{
			Normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));		//Initialize all normals to 0
		}
	}

	/*Calculates normals per vertex*/
	void CalculateNormals()
	{
		InitNormals();
		for (int i = 0; i < Indices.size(); i += 3)
		{
			glm::vec3 a = glm::vec3(Vertices[Indices[i]]);
			glm::vec3 b = glm::vec3(Vertices[Indices[i + 1]]);
			glm::vec3 c = glm::vec3(Vertices[Indices[i + 2]]);

			glm::vec3 u = a - b;
			glm::vec3 v = a - c;

			glm::vec3 normal = glm::normalize(glm::cross(u, v));

			for (int j = i; j < i + 3; j++)
			{
				Normals[Indices[j]] = glm::normalize(Normals[Indices[j]] + normal);
			}
		}
	}

	/*Prints the coordinate position*/
	void PrintCoor()
	{
		cout << "Coordinate:" << endl;
		for (int i = 0; i < Coordinate.length(); i++)
			cout << Coordinate[i] << ", ";
		cout << endl;
	}

	/*Prints the list of normals*/
	void PrintNormals()
	{
		for (int i = 0; i < Normals.size(); i++)
		{
			cout << "Normals: " << i << endl;
			cout << Normals[i].x << ", " << Normals[i].y << ", " << Normals[i].z << endl;
		}
	}
};

class Tile : public MapObject
{
public:
	vector<int> Neighbors;		//List of neighbors
	glm::vec3 Normal;			//Face normal
	//Constructors
	Tile()
	{
		TileID = -1;
	}
	Tile(int id)
	{
		TileID = id;
	}

	/*Adds the TileID of a neighbor to the neighbor list*/
	void AddNeighbor(int num)
	{
		Neighbors.push_back(num);
	}

	/*Calculates face normal*/
	void CalculateNormal()
	{
		glm::vec3 a = glm::vec3(Vertices[0]);
		glm::vec3 b = glm::vec3(Vertices[1]);
		glm::vec3 c = glm::vec3(Vertices[2]);

		glm::vec3 u = a - b;
		glm::vec3 v = a - c;

		Normal = glm::normalize(glm::cross(u, v));
		PrintNormal();
	}

	/*Prints the list of neighbors by TileID*/
	void PrintNeighbors()
	{
		for (int i = 0; i < Neighbors.size(); i++)
		{
			cout << "Neighbor: " << i << endl;
			cout << Neighbors[i] << endl;
		}
	}

	/*Prints the face normal*/
	void PrintNormal()
	{
		cout << "Normal: " << endl;
		cout << Normal.x << ", " << Normal.y << ", " << Normal.z << endl;
	}
};

void handleResize(int w, int h);
void handle_menu(int ID);
void handle_motion(int x, int y);
void handle_mouse(int b, int s, int x, int y);
void initRendering(char** argv);
void display();

void ReadMap(string fileName);
void RenderMap();
void ReadFile(string filename);
void ReadVertices(fstream& file);
void ParseLine(vector<string> lines);
void ParseTile(vector<string> lines);
void ParseTeeCup(vector<string> lines, ImportObj& obj);
vector<string> SplitString(const char *str, char c = ' ');
void BuildTiles(vector<Tile> tiles, vector<glm::vec3>& verts, vector<glm::vec3>& norms, vector<int>& inds);
//void BuildImportedObj(ImportObj obj, vector<glm::vec3>& verts, vector<glm::vec3>& norms, vector<int>& inds);
void DisplayTiles(vector<glm::vec3> verts, vector<glm::vec3> norms, vector<int> inds);
//void DisplayImportedObj(vector<glm::vec3> verts, vector<glm::vec3> norms, vector<int> inds);