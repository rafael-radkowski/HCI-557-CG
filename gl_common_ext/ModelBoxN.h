#pragma once
/*
@file: ModelBox.h/.cpp

This file creates and renders a box model

Features:
- It determines the vertices, triangles, and normal vectors for the box.
- Creates the vertex buffer object and the vertex buffer array
- Loads the content to the graphics card.
- Links the shader uniform and attribute variables to the content.
- Renders the box.

Uniform and attributes variables:
Note that this file expects to find the uniform variables
	uniform mat4 projectionMatrix;                                   
	uniform mat4 viewMatrix;                                         
	uniform mat4 modelMatrix;  
as part of the shader code

The attribute variables are:
	in vec3 in_Position;                                               
	in vec3 in_Normal;    

This file is part of CS/CPRE/ME 557 Computer Graphics at Iowa State University

Rafael Radkowski
Iowa State University
November 2018
rafael@iastate.edu

MIT License
------------------------------------------------
Last changes:

Oct 10, 2019, RR
- Fixed a bug that resulted in a compiler error; float-double conflicts. 

Oct 19, 2019, RR
- Fixed a bug: the attribute location and the vertices/normal target locations were
	incorrect associated which prevented the object from being correctly rendered in some cases. 

*/

// stl include
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// local
#include "VertexBuffers.h"			// create vertex buffer object
#include "ShaderProgram.h"			// create a shader program


using namespace std;


namespace cs557
{

	class BoxN {
	
	public:
		/*
		Create a simple coordinate system in the centroid
		@param length - the length of each unit axis
		*/
		void create(float width, float height, float length, unsigned int shader_program = -1);


		/*
		Draw the coordinate system
		@param viewMatrix - a view matrix object
		@param modelMatrix - a model matrix object.
		*/
		void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix);


		/*
		Return the shader program
		@return - int containing the shader program
		*/
		int getProgram(void){return program;}

	private:


		int vaoID[1]; // Our Vertex Array Object
		int vboID[2]; // Our Vertex Buffer Object
		int program;

		int viewMatrixLocation;
		int modelMatrixLocation;
		int projMatrixLocation;


		float _width;
		float _height; 
		float _length;
	};
}