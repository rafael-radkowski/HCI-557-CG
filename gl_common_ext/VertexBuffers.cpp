#include "VertexBuffers.h"




/*
Create a vertex array object and vertex buffer object for vertices of size 3 (x, y, z)  along with colors of size 3: (r, g, b)
@param vaoID - address to store the vertex array object
@param vboID - address to store the vertex buffer objects. Note, TWO spaces are required to create buffers of vertices and colors.
@param vertices - pointer to an array containing vertices as [x0, y0, z0, x1, y1, z1, ...]
@param colors - pointer to an array containning color as [r0, g0, b0, r1, g1, b1, .....]
@param N - the number of vertices and colors, NOT THE LENGTH OF THE ARRAY. Note that the vector sizes MUST match.
*/
bool cs557::CreateVertexObjects33(int* vaoID, int* vboID, float* vertices, float* colors, int N, int vertices_location , int normals_location )
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

	glVertexAttribPointer((GLuint)vertices_location, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(vertices_location); // Disable our Vertex Array Object


								  //Color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * 3* sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)normals_location, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(normals_location); // Enable the second vertex attribute array

	glBindVertexArray(0); // Disable our Vertex Buffer Object



	return true;

}



/*
	Create vertex buffer object for points and normal vectors + an index list
	@param vaoID - address to store the vertex array object
	@param vboID - address to store the vertex buffer objects. Note, THREE spaces are required to create buffers of vertices and normals and texture coordinate. 
	@param iboID - address to store the index buffer object
	@param vertices - pointer to an array containing vertices as [x0, y0, z0, x1, y1, z1, ...]
	@param normals - pointer to an array containning color as [n0, n0, n0, n1, n1, n1, .....]
	@param N - the number of vertices and colors, NOT THE LENGTH OF THE ARRAY. Note that the vector sizes MUST match. 
	@param indices - pointer to an array with indices
	@param I - the number of indices
	@param vertices_location - the GLSL vertices location 
	@param normals_location - the GLSL normal vectors locations
	*/
bool cs557::CreateVertexObjectsIndexed33( int* vaoID, int* vboID, int* iboID, float* points, float* normals, int N, int* indices, int I,
	 						int vertices_location, int normals_location)
{
	if (points == NULL || normals == NULL)
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
	glGenBuffers(1, (GLuint*)iboID);

	if (vboID[0] == -1 || vboID[1] == -1 ||  iboID[0] == -1){
		std::cout << "[ERROR] - One or both vertex buffer objects were not generated." << std::endl;
		return false;
	}


	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N *3* sizeof(GLfloat), points, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)vertices_location, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(vertices_location); // Disable our Vertex Array Object


	 //Color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * 3* sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)normals_location, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(normals_location); // Enable the second vertex attribute array


	// Index buffer
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID[0]);
  	glBufferData(GL_ELEMENT_ARRAY_BUFFER, I * sizeof(unsigned int),
              	indices, GL_STATIC_DRAW);


	glBindVertexArray(0); // Disable our Vertex Buffer Object



	return true;

}




/*
Create vertex buffer object for points and normal vectors + an index list
@param vaoID - address to store the vertex array object
@param vboID - address to store the vertex buffer objects. Note, TWO array elements are required to create buffers of vertices and normals and texture coordinate. 
@param iboID - address to store the index buffer object
@param vertices - pointer to an array containing vertices as [x0, y0, z0, x1, y1, z1, ...]
@param normals - pointer to an array containning color as [n0, n0, n0, n1, n1, n1, .....]
@param N - the number of vertices and colors, NOT THE LENGTH OF THE ARRAY. Note that the vector sizes MUST match. 
@param indices - pointer to an array with indices
@param I - the number of indices
@param vertices_location - the GLSL vertices location 
@param normals_location - the GLSL normal vectors locations
*/
bool cs557::CreateVertexObjectsIndexed53( int* vaoID, int* vboID, int* iboID, float* points, float* normals, int N, int* indices, int I,
							int vertices_location, int tex_coord_location, int normals_location)
{
	if (points == NULL || normals == NULL)
	{
		std::cout << "[ERROR] - CreateVertexObjectsIndexed323: No vertices or normal given." << std::endl;
		return false;
	}

	glGenVertexArrays(1, (GLuint*)vaoID); // Create our Vertex Array Object
	glBindVertexArray(*vaoID); // Bind our Vertex Array Object so we can use it

	if (vaoID[0] == -1){
		std::cout << "[ERROR] - Vertex array object was not generated." << std::endl;
		return false;
	}

	glGenBuffers(2, (GLuint*)vboID); // Generate our Vertex Buffer Object for points, normals, and texture coordinates
	glGenBuffers(1, (GLuint*)iboID);

	if (vboID[0] == -1 || vboID[1] == -1 ||  iboID[0] == -1){
		std::cout << "[ERROR] - One or both vertex buffer objects were not generated." << std::endl;
		return false;
	}


	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N *  5 * sizeof(GLfloat), points, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)vertices_location, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(vertices_location); // Enable the Vertex Array Object

	// Texture Coordinates
	glVertexAttribPointer((GLuint)tex_coord_location, 2, GL_FLOAT, GL_FALSE, 5 *sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat))); // Set up our texture attributes pointer
	glEnableVertexAttribArray(tex_coord_location); // Enable the Vertex Array Object

	//Normals
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * 3* sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)normals_location, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(normals_location); // Enable the second vertex attribute array


	// Index buffer
  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID[0]);
  	glBufferData(GL_ELEMENT_ARRAY_BUFFER, I * sizeof(unsigned int),
              	indices, GL_STATIC_DRAW);


	glBindVertexArray(0); // Disable our Vertex Buffer Object



	return true;

}




/*
Create a vertex array object and vertex buffer object for vertices and texture coordinages of size 5 (x, y, z, u, v)  
along with normal vectors of size 3: (nx, ny, nz)
@param vaoID - address to store the vertex array object
@param vboID - address to store the vertex buffer objects. Note, TWO spaces are required to create buffers of vertices and colors. 
@param vertices_texture_coord - pointer to an array containing vertices and texture coords as [x0, y0, z0, u0, v0, x1, y1, z1, u1, v1, ...]
@param normals - pointer to an array containning normal vectors as [nx0, ny0, nz0, nx1, ny1, nz1, .....]
@param N - the number of vertices and colors, NOT THE LENGTH OF THE ARRAY. Note that the vector sizes MUST match. 
*/
bool cs557::CreateVertexObjects53(int* vaoID, int* vboID, float* vertices_texture_coord, float* normals, int N,
 int vertices_location, int tex_coord_location, int normals_location)
{

	if (vertices_texture_coord == NULL || normals == NULL)
	{
		std::cout << "[ERROR] - CreateVertexObjects53: No vertices or color information given." << std::endl;
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
	glBufferData(GL_ARRAY_BUFFER, N *  5 * sizeof(GLfloat), vertices_texture_coord, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)vertices_location, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(vertices_location); // Enable the Vertex Array Object

	// Texture Coordinates
	glVertexAttribPointer((GLuint)tex_coord_location, 2, GL_FLOAT, GL_FALSE, 5 *sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat))); // Set up our texture attributes pointer
	glEnableVertexAttribArray(tex_coord_location); // Enable the Vertex Array Object

	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, N * 3* sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)normals_location, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(normals_location); // Enable the second vertex attribute array

	glBindVertexArray(0); // Disable our Vertex Buffer Object



	return true;

}



