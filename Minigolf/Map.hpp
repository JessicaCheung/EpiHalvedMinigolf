#pragma once
#include "main.hpp"

class MapObject
{
public:
	int TileID;						//Tile ID of object
	vector<glm::vec3> Vertices;		//List of vertices
	vector<glm::vec3> Normals;		//List of normals
	vector<int> Indices;			//Indices to draw vertices+normals

	//Default constructor
	MapObject() { }

	//Constructor (For Buffer creation)
	MapObject(vector<glm::vec3> verts, vector<glm::vec3> norms, vector<int> inds)
	{
		Vertices = verts;
		Normals = norms;
		Indices = inds;
	}

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


	/*Prints the list of normals*/
	void PrintNormals()
	{
		for (int i = 0; i < Normals.size(); i++)
		{
			cout << "Normals: " << i << endl;
			cout << Normals[i].x << ", " << Normals[i].y << ", " << Normals[i].z << endl;
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

	glm::vec3 GetMinPoints(vector<glm::vec3> vertices)
	{
		int x, y, z;
		x = y = z = INFINITY;

		for (int i = 0; i < vertices.size(); i++)
		{
			if (vertices[i].x < x) x = vertices[i].x;
			if (vertices[i].y < y) y = vertices[i].y;
			if (vertices[i].z < z) z = vertices[i].z;
		}

		return glm::vec3(x, y, z);
	}

	glm::vec3 GetMaxPoints(vector<glm::vec3> vertices)
	{
		int x, y, z;
		x = y = z = -INFINITY;

		for (int i = 0; i < vertices.size(); i++)
		{
			if (vertices[i].x > x) x = vertices[i].x;
			if (vertices[i].y > y) y = vertices[i].y;
			if (vertices[i].z > z) z = vertices[i].z;
		}

		return glm::vec3(x, y, z);
	}
};

class ImportObj : public MapObject
{
public:
	glm::vec3 Coordinate;		//Position of Tee

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

	//Renders the import object
	void Render()
	{

	}
};

class Tile : public MapObject
{
public:
	vector<int> Neighbors;		//List of neighbors
	glm::vec3 Normal;			//Face normal
	glm::vec3 Location;			//Location of the tile

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

	//Gets the Tile's position, or its center point
	void CalculateLocation()
	{
		//Get min points and max points
		glm::vec3 minpoints = /*Physics::*/GetMinPoints(Vertices);
		glm::vec3 maxpoints = /*Physics::*/GetMaxPoints(Vertices);

		//Find difference and divide by 2
		glm::vec3 difference = (maxpoints - minpoints) / 2.0f;

		//Final location; the center of the tile
		Location = (maxpoints - difference);
	}
};

//extern ImportObj Tee;

void ReadMap(string fileName);
void RenderMap();
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
void ReadFile(string filename);
void ReadVertices(fstream& file);
void ParseLine(vector<string> lines);
void ParseTile(vector<string> lines);
void ParseTeeCup(vector<string> lines, ImportObj& obj);
vector<string> SplitString(const char *str, char c = ' ');
void load_obj(const char* filename, vector<glm::vec3> &vertices, vector<int> &elements, glm::vec3 coor);
void BuildTiles(vector<Tile> tiles, vector<glm::vec3>& verts, vector<glm::vec3>& norms, vector<int>& inds);
void BuildWalls(vector<Tile> tiles, vector<glm::vec3>& verts, vector<int>& inds);
void DisplayMap(int num, int size);
MapObject getTileBuffer();
ImportObj getTeeBuffer();
ImportObj getCupBuffer();
ImportObj getWallsBuffer();
ImportObj getPointer();
vector<Tile> getTiles();