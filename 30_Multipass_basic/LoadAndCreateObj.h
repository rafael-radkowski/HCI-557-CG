#pragma once


// stl include
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <experimental\filesystem>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



using namespace std;

class LoadAndCreateObj
{
public:

	static bool Load(const char* filename, vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<GLuint> &elements);


private:


	static bool extractNextFace1(string& in, string& out, int& pointIdx0, int& pointIdx1, int& pointIdx2);


	static bool extractNextFace3(string& in, string& out, int& pointIdx, int& texIdx, int& normalIdx);
};