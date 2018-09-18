#include "VertexBuffers.h"





/*
Create a vertex array object and vertex buffer object for vertices of size 3 (x, y, z)  along with colors of size 3: (r, g, b)
@param vaoID - address to store the vertex array object
@param vboID - address to store the vertex buffer objects. Note, TWO spaces are required to create buffers of vertices and colors.
@param vertices - pointer to an array containing vertices as [x0, y0, z0, x1, y1, z1, ...]
@param colors - pointer to an array containning color as [r0, g0, b0, r1, g1, b1, .....]
@param N - the number of vertices and colors, NOT THE LENGTH OF THE ARRAY. Note that the vector sizes MUST match.
*/
bool cs557::CreateVertexObjects33(int* vaoID, int* vboID, float* vertices, float* colors, int N)
{
	if (vertices == NULL || colors == NULL)
	{
		std::cout << "[ERROR] - CreateVertexObjects33: No vertices or color information given." << std::endl;
		return false;
	}

	glGenVertexArrays(1, (GLuint*)vaoID); // Create our Vertex Array Object
	glBindVertexArray(*vaoID); // Bind our Vertex Array Object so we can use it

	if (vaoID[1] == -1){
		std::cout << "[ERROR] - Vertex array object was not generated." << std::endl;
		return false;
	}

	glGenBuffers(2, (GLuint*)vboID); // Generate our Vertex Buffer Object


	if (vboID[0] == -1 || vboID[1] == -1){
		std::cout << "[ERROR] - One or both vertex buffer objects were not generated." << std::endl;
		return false;
	}


	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N *3* sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(0); // Disable our Vertex Array Object


								  //Color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * 3* sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(1); // Enable the second vertex attribute array

	glBindVertexArray(0); // Disable our Vertex Buffer Object



	return true;

}