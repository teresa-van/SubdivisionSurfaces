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
);
