#include "myobjects.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

//#include <glad/glad.h>
#include <GLFW/glfw3.h>

//using namespace std;
//using namespace glm;

bool loadOBJ(
    const char * path,
    std::vector<glm::vec3> & vertices,
    std::vector<glm::vec3> & vnormals,
    std::vector<std::vector<std::pair<int,int>>> faces
)
{
	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}
	int count = 0;
	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		else
			count++;
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			vertices.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			vnormals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3, vertex4;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3] );			
			std::vector<std::pair<int, int>> f;
			if (matches == 9) {
				f.push_back(std::make_pair(vertexIndex[0],normalIndex[0]));
				f.push_back(std::make_pair(vertexIndex[1],normalIndex[1]));
				f.push_back(std::make_pair(vertexIndex[2],normalIndex[2]));
			}
			else if (matches == 12) {
				f.push_back(std::make_pair(vertexIndex[0],normalIndex[0]));
				f.push_back(std::make_pair(vertexIndex[1],normalIndex[1]));
				f.push_back(std::make_pair(vertexIndex[2],normalIndex[2]));
				f.push_back(std::make_pair(vertexIndex[3],normalIndex[3]));
			}
			else {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			faces.push_back(f);
		}
	}
	fclose(file);
	return true;
}
