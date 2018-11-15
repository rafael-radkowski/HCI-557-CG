
#ifndef BSMORPHING_INCLUDED
#define BSMORPHING_INCLUDED

// stl include
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

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

	class ModelBSMorphing {
	
	public:
		/*
		Load an OBJ model from file
		@param path_and_filename1 - path and filename of the first model to load
		@param path_and_filename2 - path and filename of the second model to load. 
		@param shader_program - overwrite the default shader program by passing a hander to the constructor
		*/
		void create(string path_and_filename_1, string path_and_filename_2, unsigned int shader_program = -1);


		/*
		Draw the obj model
		@param viewMatrix - a view matrix object
		@param modelMatrix - a model matrix object.
		*/
		void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix, float time_index = 0.0);


	private:


		typedef struct _morphdata{
			glm::vec3 p1;
			glm::vec2 t1;
			glm::vec3 n1;

			glm::vec3 p2;
			glm::vec2 t2;
			glm::vec3 n2;
		}MorphData;



		int vaoID[1]; // Our Vertex Array Object
		int vboID[2]; // Our Vertex Buffer Object
		int iboID[1]; // Our Index  Object
		int program;
		int program_backup;

		int viewMatrixLocation;
		int modelMatrixLocation;
		int projMatrixLocation;

		// blending index for morphing
		float blending_index;
		int blending_index_location; 

	
		int _N; // number of vertices
		int _I; // number indices
	};
}


#endif