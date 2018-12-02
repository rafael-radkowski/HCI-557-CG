#pragma once

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