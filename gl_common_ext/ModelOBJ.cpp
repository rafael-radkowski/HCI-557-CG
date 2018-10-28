#include "ModelOBJ.h"

#include "OBJLoader.h"

namespace cs557_OBJModel
{

	static const string vs_string_410 =
		"#version 410 core                                                 \n"
		"                                                                   \n"
		"uniform mat4 projectionMatrix;                                    \n"
		"uniform mat4 viewMatrix;                                           \n"
		"uniform mat4 modelMatrix;                                          \n"
		"in vec3 in_Position;                                               \n"
		"in vec2 in_Texture;                                                \n"
		"in vec3 in_Normal;                                                  \n"
		"out vec3 pass_Color;                                               \n"
		"out vec2 pass_Texture;												\n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
		"    pass_Color = in_Normal;                                         \n"
		"    pass_Texture = in_Texture;                                         \n"
		"}                                                                 \n";

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	static const string fs_string_410 =
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"in vec3 pass_Color;                                                 \n"
		"in vec2 pass_Texture;												\n"
		"out vec4 color;                                                    \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(pass_Color, 1.0);                               \n"
		"}                                                                 \n";



};



using namespace cs557_OBJModel;



	
/*
Load an OBJ model from file
@param path_and_filename - number of rows
@param shader_program - overwrite the default shader program by passing a hander to the constructor
*/
void cs557::OBJModel::create(string path_and_filename, unsigned int shader_program)
{
	program = -1;
	_N = 0;
	_I = 0;

	// This overwrite the default shader program
	if(shader_program != -1){
		program = shader_program;
	}


	float center_x = 0.0;
	float center_y = 0.0;
	float center_z = 0.0;

	std::vector<std::pair<glm::vec3, glm::vec2> > points; // points and texture coordinates
	std::vector<glm::vec3> normals;
	std::vector<int> indices;

	

	// create a shader program only if the progrm was not overwritten. 
	if(program == -1)
		program = cs557::CreateShaderProgram(vs_string_410, fs_string_410);


	// Find the id's of the related variable name in your shader code. 
	projMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
	modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
	int pos_location = glGetAttribLocation(program, "in_Position");
	int norm_location = glGetAttribLocation(program, "in_Normal");
	int tex_location = glGetAttribLocation(program, "in_Texture");


	// Load the geometry from file. 
	objl::Loader loader;
	loader.LoadFile(path_and_filename);

	int size = loader.LoadedMeshes.size();

	if(size > 0)
	{

		objl::Mesh curMesh = loader.LoadedMeshes[0];

		for (int j = 0; j < curMesh.Vertices.size(); j++)
		{
			points.push_back(make_pair( glm::vec3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z),
			 							glm::vec2(curMesh.Vertices[j].TextureCoordinate.X, curMesh.Vertices[j].TextureCoordinate.Y )));
			normals.push_back(glm::vec3(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z) );
		}
		

		for (int j = 0; j < curMesh.Indices.size(); j++)
		{
			indices.push_back(curMesh.Indices[j]);
		}

		_I = curMesh.Indices.size();
		_N = points.size();


		// create a vertex buffer object
		cs557::CreateVertexObjectsIndexed53(vaoID, vboID, iboID, &points[0].first.x, &normals[0].x, _N, &indices[0], _I, pos_location, tex_location, norm_location );


		//glBindAttribLocation(program, pos_location, "in_Position");
		//glBindAttribLocation(program, norm_location, "in_Normal");
		//glBindAttribLocation(program, tex_location, "in_Texture");
	}
}


/*
Draw the coordinate system
@param viewMatrix - a view matrix object
@param modelMatrix - a model matrix object.
*/
void cs557::OBJModel::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix )
{

	// Enable the shader program
	glUseProgram(program);



	// this changes the camera location
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // send the view matrix to our shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // send the model matrix to our shader
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // send the projection matrix to our shader


	 // Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[0]);

	// Draw the triangles
 	glDrawElements(GL_TRIANGLES, _N, GL_UNSIGNED_INT, 0);
	

	// Unbind our Vertex Array Object
	glBindVertexArray(0);

	// Unbind the shader program
	glUseProgram(0);


}



