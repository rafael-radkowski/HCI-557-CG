#include "ModelPlane.h"



namespace cs557_plane
{

	static const string vs_string_plane_410 =
		"#version 410 core                                                 \n"
		"                                                                   \n"
		"uniform mat4 projectionMatrix;                                    \n"
		"uniform mat4 viewMatrix;                                           \n"
		"uniform mat4 modelMatrix;                                          \n"
		"in vec3 in_Position;                                               \n"
		"in vec2 in_Texture;                                                 \n"
		"in vec3 in_Normal;                                                  \n"
		"out vec3 pass_Normal;                                              \n"
		"out vec2 pass_Texture;												\n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
		"    pass_Normal = in_Normal;                                         \n"
		"	 pass_Texture = in_Texture;										\n"
		"}                                                                 \n";

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	static const string fs_string_plane_410 =
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"in vec3 pass_Normal;                                                 \n"
		"in vec2 pass_Texture;												\n"
		"out vec4 color;                                                    \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(pass_Texture, 0.0, 1.0);                               \n"
		"}                                                                 \n";



};



using namespace cs557_plane;


/*
Create a simple coordinate system in the centroid
@param length - the length of each unit axis
*/
void cs557::Plane::create(float width, float height, int shader_program)
{

	// This overwrite the default shader program
	if(shader_program != -1){
		program = shader_program;
	}
	else
	{
		program = -1;
	}

	_width = width;
	_height = height; 

	float center_x = 0.0;
	float center_y = 0.0;
	float center_z = 0.0;


	float vertices[] = { 
		//--------------------------------------------------------
		// xy-plane, positive z direction, texture coordinates
		-_width / 2.0f + center_x, -_height / 2.0f + center_y, 0.0f, 0.0f, 0.0f,
		-_width / 2.0f + center_x, _height / 2.0f + center_y, 0.0f, 0.0f, 1.0f,
		_width / 2.0f + center_x, -_height / 2.0f + center_y, 0.0f, 1.0f, 0.0f,
		_width / 2.0f + center_x, _height / 2.0f + center_y,  0.0f, 1.0f, 1.0f
		
	};





	float normals[] = { 0.0, 0.0, 1.0, //
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0,
	};



	// create a shader program only if the progrm was not overwritten. 
	if(program == -1)
		program = cs557::CreateShaderProgram(vs_string_plane_410, fs_string_plane_410);

	glUseProgram(program);

	int pos_location = glGetAttribLocation(program, "in_Position");
	int normal_location = glGetAttribLocation(program, "in_Normal");
	int texture_location = glGetAttribLocation(program, "in_Texture");


	// create a vertex buffer object
	CreateVertexObjects53(vaoID, vboID, vertices, normals, 4, 
						  pos_location, texture_location, normal_location);


	// Find the id's of the related variable name in your shader code. 
	projMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
	modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader

	

	glBindAttribLocation(program, pos_location, "in_Position");
	glBindAttribLocation(program, texture_location, "in_Texture");
	glBindAttribLocation(program, normal_location, "in_Normal");

	glUseProgram(0);
}


/*
Draw the coordinate system
@param viewMatrix - a view matrix object
@param modelMatrix - a model matrix object.
*/
void cs557::Plane::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix)
{


	// Enable the shader program
	glUseProgram(program);



	// this changes the camera location
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // send the view matrix to our shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // send the model matrix to our shader
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // send the projection matrix to our shader


	 // Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[0]);

	glLineWidth((GLfloat)3.0);

	// Draw the triangles
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	// Unbind our Vertex Array Object
	glBindVertexArray(0);

	// Unbind the shader program
	glUseProgram(0);


}