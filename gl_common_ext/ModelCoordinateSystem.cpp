#include "ModelCoordinateSystem.h"



namespace cs557_cs
{

	static const string vs_string_CoordSystem_410 =
		"#version 410 core                                                 \n"
		"                                                                   \n"
		"uniform mat4 projectionMatrix;                                    \n"
		"uniform mat4 viewMatrix;                                           \n"
		"uniform mat4 modelMatrix;                                          \n"
		"in vec3 in_Position;                                               \n"
		"                                                                   \n"
		"in vec3 in_Color;                                                  \n"
		"out vec3 pass_Color;                                               \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
		"    pass_Color = in_Color;                                         \n"
		"}                                                                 \n";

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	static const string fs_string_CoordSystem_410 =
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"in vec3 pass_Color;                                                 \n"
		"out vec4 color;                                                    \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(pass_Color, 1.0);                               \n"
		"}                                                                 \n";



};





using namespace cs557_cs;


/*
Create a simple coordinate system in the centroid
@param length - the length of each unit axis
*/
void cs557::CoordinateSystem::create(float length)
{

	float vertices[] = { 0.0, 0.0, 0.0, length, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, length, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, length };


	float colors[] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 1.0, 0.0, 0.0, 1.0 };



	// create a vertex buffer object
	CreateVertexObjects33(vaoID, vboID, vertices, colors, 6);


	// create a shader program
	program = cs557::CreateShaderProgram(vs_string_CoordSystem_410, fs_string_CoordSystem_410);



	// Find the id's of the related variable name in your shader code. 
	projMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
	modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
	int pos_location = glGetAttribLocation(program, "in_Position");
	int col_location = glGetAttribLocation(program, "in_Color");


	glBindAttribLocation(program, pos_location, "in_Position");
	glBindAttribLocation(program, col_location, "in_Color");

}


/*
Draw the coordinate system
@param viewMatrix - a view matrix object
@param modelMatrix - a model matrix object.
*/
void cs557::CoordinateSystem::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix)
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
	glDrawArrays(GL_LINES, 0, 6);

	// Unbind our Vertex Array Object
	glBindVertexArray(0);

	// Unbind the shader program
	glUseProgram(0);


}