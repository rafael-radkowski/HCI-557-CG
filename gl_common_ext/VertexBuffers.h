#pragma once


// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// glfw includes
#include <GLFW/glfw3.h>


using namespace std;


namespace cs557
{

	/*
	Create a vertex array object and vertex buffer object for vertices of size 3 (x, y, z)  along with colors of size 3: (r, g, b)
	@param vaoID - address to store the vertex array object
	@param vboID - address to store the vertex buffer objects. Note, TWO spaces are required to create buffers of vertices and colors. 
	@param vertices - pointer to an array containing vertices as [x0, y0, z0, x1, y1, z1, ...]
	@param colors - pointer to an array containning color as [r0, g0, b0, r1, g1, b1, .....]
	@param N - the number of vertices and colors, NOT THE LENGTH OF THE ARRAY. Note that the vector sizes MUST match. 
	*/
	bool CreateVertexObjects33(int* vaoID, int* vboID, float* vertices, float* colors, int N);


}