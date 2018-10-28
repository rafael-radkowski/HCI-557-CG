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
	@param vertices_location - the GLSL vertices location 
	@param normals_location - the GLSL normal vectors locations
	*/
	bool CreateVertexObjects33(int* vaoID, int* vboID, float* vertices, float* colors, int N,
								int vertices_location = 0, int normals_location = 1);

	/*
	Create vertex buffer object for points and normal vectors + an index list
	@param vaoID - address to store the vertex array object
	@param vboID - address to store the vertex buffer objects. Note, TWO spaces are required to create buffers of vertices and normals. 
	@param iboID - address to store the index buffer object
	@param vertices - pointer to an array containing vertices as [x0, y0, z0, x1, y1, z1, ...]
	@param normals - pointer to an array containning color as [n0, n0, n0, n1, n1, n1, .....]
	@param N - the number of vertices and colors, NOT THE LENGTH OF THE ARRAY. Note that the vector sizes MUST match. 
	@param indices - pointer to an array with indices
	@param I - the number of indices
	@param vertices_location - the GLSL vertices location 
	@param normals_location - the GLSL normal vectors locations
	*/
	bool CreateVertexObjectsIndexed33( int* vaoID, int* vboID, int* iboID, float* points, float* normals, int N, int* indices, int I,
	 							int vertices_location = 0, int normals_location = 1);	



	/*
	Create vertex buffer object for points and normal vectors + an index list
	@param vaoID - address to store the vertex array object
	@param vboID - address to store the vertex buffer objects. Note, THREE spaces are required to create buffers of vertices and normals and texture coordinate. 
	@param iboID - address to store the index buffer object
	@param vertices - pointer to an array containing vertices  and texture coordinates as [x0, y0, z0, u0, v0, x1, y1, z1, u1, v1, ...]
	@param normals - pointer to an array containning color as [n0, n0, n0, n1, n1, n1, .....]
	@param N - the number of vertices and colors, NOT THE LENGTH OF THE ARRAY. Note that the vector sizes MUST match. 
	@param indices - pointer to an array with indices
	@param I - the number of indices
	@param vertices_location - the GLSL vertices location 
	@param tex_coord_location - the GLSL location of the texture coordinates.
	@param normals_location - the GLSL normal vectors locations
	*/
	bool CreateVertexObjectsIndexed53( int* vaoID, int* vboID, int* iboID, float* points, float* normals, int N, int* indices, int I,
							int vertices_location = 0, int tex_coord_location = 1, int normals_location = 2);	


	/*
	Create a vertex array object and vertex buffer object for vertices and texture coordinages of size 5 (x, y, z, u, v)  
	along with normal vectors of size 3: (nx, ny, nz)
	@param vaoID - address to store the vertex array object
	@param vboID - address to store the vertex buffer objects. Note, TWO spaces are required to create buffers of vertices and colors. 
	@param vertices_texture_coord - pointer to an array containing vertices and texture coords as [x0, y0, z0, u0, v0, x1, y1, z1, u1, v1, ...]
	@param normals - pointer to an array containning normal vectors as [nx0, ny0, nz0, nx1, ny1, nz1, .....]
	@param N - the number of vertices and colors, NOT THE LENGTH OF THE ARRAY. Note that the vector sizes MUST match. 
	@param vertices_location - the GLSL vertices location 
	@param normals_location - the GLSL normal vectors locations
	*/
	bool CreateVertexObjects53( int* vaoID, int* vboID, float* vertices_texture_coord, float* normals, int N,
	 							int vertices_location = 0, int tex_coord_location = 1, int normals_location = 2);




							 

}