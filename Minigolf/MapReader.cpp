/*#include "MapReader.hpp"

void ReadFile(string filename, vector<Tile>& Tiles, ImportObj& Tee, ImportObj& Cup)
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
		ReadVertices(file, Tiles, Tee, Cup);
	}

	file.close();
}

//Read vertices from the file
void ReadVertices(fstream& file, vector<Tile>& Tiles, ImportObj& Tee, ImportObj& Cup)
{
	while (file.eof() == false)
	{
		string line;

		//Read the line until you find a new line
		getline(file, line);

		//Split the string
		vector<string> lines = SplitString(line.c_str());

		//Parse the lines
		ParseLine(lines, Tiles, Tee, Cup);
	}
}

void ParseLine(vector<string> lines, vector<Tile>& Tiles, ImportObj& Tee, ImportObj& Cup)
{
	//Check for a tile
	if (lines[0] == "tile")
	{
		ParseTile(lines, Tiles);
	}
	else if (lines[0] == "tee")
	{
		ParseTeeCup(lines, Tee);
	}
	else if (lines[0] == "cup")
	{
		ParseTeeCup(lines, Cup);
	}
}

void ParseTile(vector<string> lines, vector<Tile>& Tiles)
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
	newTile.CalculateLocation();
	//Add the tile to our list
	Tiles.push_back(newTile);
}

//Parses the tee/cup
void ParseTeeCup(vector<string> lines, ImportObj& obj)
{
	//Get the tile ID
	int tileID = stoi(lines[1]);
	obj.TileID = tileID;
	obj.TileID = tileID;
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

//Imports a 3D model from Blender
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
		}*/
		//else if (line[0] == '#') { /* ignoring this line */ }
		//else { /* ignoring this line */ }
	//}
//}