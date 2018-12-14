#pragma once

// stl include
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <strstream>
#include <string>

#ifdef _WIN32
#include <experimental\filesystem>
#endif


// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// locals
#include "ShaderSource.h"


using namespace std;


namespace cs557 {

	/*
	Create a shader program
	@param vertex source - the vertex shader source code as string. 
	@param fragment_source - the fragment shader source code as string
	@return - Gluint of the shader program, must be larger than 0. 
	0 indicates a problem
	*/
	GLuint CreateShaderProgram(string vertex_source, string fragment_source);


	/*!
	Load a shader program from a file and creates the related program
	@param vertex_file -  the file which stores the vertex shader code
	@param fragment_file -  the file which stores the fragment shader code
	@return - Gluint of the shader program, must be larger than 0. 
	0 indicates a problem
	*/
	GLuint LoadAndCreateShaderProgram(string vertex_file, string fragment_file);


	/*
	Create a shader program
	@param vertex source - the vertex shader source code as string. 
	@param geometry source - the geometry shader source code as string. 
	@param fragment_source - the fragment shader source code as string
	@return - Gluint of the shader program, must be larger than -1. 
	0 indicates a problem
	*/
	GLuint CreateShaderProgram(string vertex_source, string geometry_source, string fragment_source);



	/*!
	Load a shader program from a file and creates the related program
	@param vertex_file -  the file which stores the vertex shader code
	@param geometry_file -  the file which stores the geometry shader code
	@param fragment_file -  the file which stores the fragment shader code
	@return - Gluint of the shader program, must be larger than 0. 
	0 indicates a problem
	*/
	GLuint LoadAndCreateShaderProgram(string vertex_file, string geometry_file, string fragment_file);


	class ShaderProgramUtils
	{
	public:
		/*!
		 Check the shader code for errors after compiling
		 @param shader: the shader program id
		 @param shader_type, vertex or framgment shader
		 */
		static bool CheckShader(GLuint shader, GLenum shader_type);


		/*!
		 Check the shader code for errors after linking
		 @param shader: the shader program id
		 */
		static bool CheckLinker(GLuint shader);


		/*!
		 Verifies wheterh a file [name] exits
		 @param name - the path and the name of the file.
		 @return - true if the file exits.
		 */
		static bool Exists(const std::string& name);



		/*!
		 Opens a file and loads the code from this file.
		 @param path_and_file - the path and the filename of the file
		 @return the content as string.
		 */
		static string LoadFromFile(string path_and_file);
	};

}



