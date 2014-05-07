#include "Map.hpp"

vector<glm::vec3> TileVertices;
vector<glm::vec3> TileNormals;
vector<int> TileIndices;

vector<glm::vec3> TeeVertices;
vector<glm::vec3> TeeNormals;
vector<int> TeeIndices;

vector<glm::vec3> CupVertices;
vector<glm::vec3> CupNormals;
vector<int> CupIndices;

vector<Tile> Tiles;		//Our tiles
ImportObj Tee;				//The tee
ImportObj Cup;				//The cup

//Read/parse file and render the map (tiles, tee, and cup)
void ReadMap(string fileName)
{
	ReadFile(fileName);
	BuildTiles(Tiles, TileVertices, TileNormals, TileIndices);
	for (int i = 0; i < TileNormals.size(); i++)
	{
		cout << "i " << i << " norms " <<
			TileNormals[i].x << " " << TileNormals[i].y << " " << TileNormals[i].z << endl;
	}
}

void RenderMap()
{
	DisplayTiles(TileVertices, TileNormals, TileIndices);
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
		newTile.AddIndex(newTile.SizeofVertices() - 1);
	}

	//Get the current position in the line for easy access; at this point it starts reading in the neighbors
	int neighborposition = vertexpoints + (numedges * vertexpoints);

	//Read in the neighbors
	for (int i = neighborposition; i < lines.size(); i++)
	{
		if (lines[i] != "")
		{
			//Read in the neighbor value; check for a negative number afterwards (stoi does not support negative numbers from what I read)
			int neighbor = stoi(lines[i]);
			if (lines[i].at(0) == '-') neighbor = -neighbor;

			//Add the neighbor
			newTile.AddNeighbor(neighbor);
		}
	}
	newTile.CalculateNormal();
	//Add the tile to our list
	Tiles.push_back(newTile);
}

//Parses the tee/cup
void ParseTeeCup(vector<string> lines, ImportObj& obj)
{
	//Get the tile ID
	int tileID = stoi(lines[1]);

	glm::vec3 teecupcoordinates;

	//Read in the coordinates for the tee/cup
	for (int i = 0; i < obj.Coordinate.length(); i++)
	{
		istringstream(lines[i + 2]) >> teecupcoordinates[i];
	}

	//Change the coordinates accordingly
	obj.ChangeCoordinate(teecupcoordinates);
}

//Splits a string based on a specified character
//This function was obtained from here: http://stackoverflow.com/a/53878
vector<string> SplitString(const char *str, char c)
{
	vector<string> result;

	do
	{
		//Start at the first character in our current sequence; after each loop, this will be the first character after the delimiter (Ex. "h" the first time then "c" after one loop in the string "hello chris")
		const char *begin = str;

		//If the current character is not the delimiter (a space character in our case) and the pointer isn't null, move onto the next character
		while (*str != c && *str)
			str++;

		//We encountered the delimiter or a null character, so construct a new string based on the characters we had before this condition and add that to our vector
		result.push_back(string(begin, str));

		//Make sure the next character isn't a null character, which would end the string
	} while (*str++ != 0);

	return result;
}

/*FindVectorVec3
Searches list if vec3 already exists
Returns -1 if does not exist
Else, returns index*/
int FindVectorVec3(vector<glm::vec3> list, glm::vec3 key)
{
	//If list is empty, return -1
	if (list.empty())
		return -1;
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == key)
			return i;
	}
	return -1;
}

/*BuildTiles
Runs through list of Tiles and fills in the buffers for vertices, normals, and indices*/
void BuildTiles(vector<Tile> tiles, vector<glm::vec3>& verts, vector<glm::vec3>& norms, vector<int>& inds)
{
	vector<int> temp;
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[i].Vertices.size(); j++)
		{
			//Build Vertices list
			int index = FindVectorVec3(verts, tiles[i].Vertices[j]);
			if (index == -1)
			{
				verts.push_back(tiles[i].Vertices[j]);
				norms.push_back(tiles[i].Normal);
				temp.push_back(verts.size() - 1);
			}
			else
			{
				temp.push_back(index);
				norms[index] = glm::normalize(norms[index] + tiles[i].Normal);			}
		}
		inds.push_back(temp[0]);
		inds.push_back(temp[1]);
		inds.push_back(temp[2]);
		if (tiles[i].Vertices.size() > 3)
		{
			for (int k = 2; k < temp.size() - 1; k++)
			{
				inds.push_back(temp[0]);
				inds.push_back(temp[k]);
				inds.push_back(temp[k + 1]);
			}
		}
		temp.clear();
	}
}

/*DisplayTiles
Draws Tiles*/
void DisplayTiles(vector<glm::vec3> verts, vector<glm::vec3> norms, vector<int> inds)
{
	glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 1.0f, 0.0f);
		for (int i = 0; i < inds.size(); i++)
		{
			glNormal3f(norms[inds[i]].x, norms[inds[i]].y, norms[inds[i]].z);
			glVertex3f(verts[inds[i]].x, verts[inds[i]].y, verts[inds[i]].z);
		}
	glEnd();
}