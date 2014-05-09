#include "Map.hpp"

vector<Tile> Tiles;			//Our tiles
MapObject TileBuffer;		//Tile buffer object
ImportObj Tee;				//The tee
ImportObj Cup;				//The cup

//Read/parse file and render the map (tiles, tee, and cup)
void ReadMap(string fileName)
{
	ReadFile(fileName);
	BuildTiles(Tiles, TileBuffer.Vertices, TileBuffer.Normals, TileBuffer.Indices);
	load_obj("BallSmall.obj", Tee.Vertices, Tee.Indices, glm::vec3(Tee.Coordinate.x, Tee.Coordinate.y + 0.1f, Tee.Coordinate.z));
	load_obj("Cup.obj", Cup.Vertices, Cup.Indices, glm::vec3(Cup.Coordinate.x, Cup.Coordinate.y - 0.05f, Cup.Coordinate.z));
	Tee.CalculateNormals();
	Cup.CalculateNormals();
}

void RenderMap()
{
	DisplayMap(0, TileBuffer.Indices.size());
	//DisplayMap(1, Tee.Indices.size());
	DisplayMap(2, Cup.Indices.size());
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

void load_obj(const char* filename, vector<glm::vec3> &vertices, vector<int> &elements, glm::vec3 coor)
{
	ifstream in(filename, ios::in);
	if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }

	string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			istringstream s(line.substr(2));
			glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
			v += coor;
			vertices.push_back(v);
		}
		else if (line.substr(0, 2) == "f ")
		{
			istringstream s(line.substr(2));
			int a, b, c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			elements.push_back(a); elements.push_back(b); elements.push_back(c);
		}
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
	}
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

/*Display things on map*/
void DisplayMap(int num, int size)
{
	GLuint* vao = getVAO();
	GLuint shader = getShader();

	glm::vec4 greenColor(0.0f, 1.0f, 0.0f, 1.0);
	glm::vec4 whiteColor(1.0f, 1.0f, 1.0f, 1.0);
	glm::vec4 redColor(1.0f, 0.0f, 0.0f, 1.0);
	glm::vec3 at(0.0f, 0.0f, 0.0f);
	glm::vec3 eye(0.0f, 5.0f, 5.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	GLuint ModelView = getModelView();
	glm::mat4 model_view = getmodel_view();
	glm::vec3 newCoor;
	switch (num)
	{
	case 0:
		glUniform4fv(glGetUniformLocation(shader, "MatColor"), 1, (GLfloat*)&greenColor);
		break;
	case 1:
		glUniform4fv(glGetUniformLocation(shader, "MatColor"), 1, (GLfloat*)&whiteColor);
		break;
	case 2:
		glUniform4fv(glGetUniformLocation(shader, "MatColor"), 1, (GLfloat*)&redColor);
		break;
	case 3:
		newCoor = getNewCoor();
		model_view *= glm::translate(newCoor);
		glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(model_view));
		glUniform4fv(glGetUniformLocation(shader, "MatColor"), 1, (GLfloat*)&whiteColor);
		glutPostRedisplay();
		break;
	}
	glBindVertexArray(vao[num]);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

MapObject getTileBuffer()
{
	return TileBuffer;
}

ImportObj getTeeBuffer()
{
	return Tee;
}

ImportObj getCupBuffer()
{
	return Cup;
}

vector<Tile> getTiles()
{
	return Tiles;
}