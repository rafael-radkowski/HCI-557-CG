#include "ModelBox.h"



namespace cs557_box
{

	static const string vs_string_box_410 =
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
	static const string fs_string_box_410 =
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"in vec3 pass_Color;                                                 \n"
		"out vec4 color;                                                    \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(pass_Color, 1.0);                               \n"
		"}                                                                 \n";



};



using namespace cs557_box;


/*
Create a simple coordinate system in the centroid
@param length - the length of each unit axis
*/
void cs557::Box::create(float width, float height, float length)
{

	_width = width;
	_height = height;
	_length = length; 

	float center_x = 0.0;
	float center_y = 0.0;
	float center_z = 0.0;


	float vertices[] = { 
		//--------------------------------------------------------
		// xy-plane, positive z direction
		-_width / 2.0 + center_x, -_height / 2.0 + center_y, _length / 2.0 + center_z,
		-_width / 2.0 + center_x, _height / 2.0 + center_y, _length / 2.0 + center_z,
		_width / 2.0 + center_x, -_height / 2.0 + center_y,  _length / 2.0 + center_z,
		_width / 2.0 + center_x, _height / 2.0 + center_y, _length / 2.0 + center_z,
		//--------------------------------------------------------
		// xy-plane, negative z direction
		_width / 2.0 + center_x, -_height / 2.0 + center_y, -_length / 2.0 + center_z,
		_width / 2.0 + center_x, _height / 2.0 + center_y,  -_length / 2.0 + center_z,
		-_width / 2.0 + center_x, -_height / 2.0 + center_y,  -_length / 2.0 + center_z,
		-_width / 2.0 + center_x, _height / 2.0 + center_y, -_length / 2.0 + center_z,
		//--------------------------------------------------------
		// yz-plane, positive x direction
		_width / 2.0 + center_x, -_height / 2.0 + center_y,  _length / 2.0 + center_z,
		_width / 2.0 + center_x, _height / 2.0 + center_y, _length / 2.0 + center_z,
		_width / 2.0 + center_x, -_height / 2.0 + center_y, -_length / 2.0 + center_z,
		_width / 2.0 + center_x,  _height / 2.0 + center_y, -_length / 2.0 + center_z,
		//--------------------------------------------------------
		// yz-plane, negative x direction
		-_width / 2.0 + center_x, -_height / 2.0 + center_y,  _length / 2.0 + center_z,
		-_width / 2.0 + center_x, _height / 2.0 + center_y, _length / 2.0 + center_z,
		-_width / 2.0 + center_x, -_height / 2.0 + center_y, -_length / 2.0 + center_z,
		-_width / 2.0 + center_x,  _height / 2.0 + center_y, -_length / 2.0 + center_z,
		//--------------------------------------------------------
		// xz-plane, positive y direction
		-_width / 2.0 + center_x, _height / 2.0 + center_y, _length / 2.0 + center_z,
		-_width / 2.0 + center_x, _height / 2.0 + center_y, -_length / 2.0 + center_z,
		_width / 2.0 + center_x, _height / 2.0 + center_y,  _length / 2.0 + center_z,
		_width / 2.0 + center_x, _height / 2.0 + center_y, -_length / 2.0 + center_z,
		//--------------------------------------------------------
		// xz-plane, negative y direction
		-_width / 2.0 + center_x, -_height / 2.0 + center_y, _length / 2.0 + center_z,
		-_width / 2.0 + center_x, -_height / 2.0 + center_y, -_length / 2.0 + center_z,
		_width / 2.0 + center_x, -_height / 2.0 + center_y,  _length / 2.0 + center_z,
		_width / 2.0 + center_x, -_height / 2.0 + center_y, -_length / 2.0 + center_z,
	};





	float colors[] = { 1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0, 
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0 ,
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0 ,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
		1.0, 1.0, 0.0 ,
		1.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
		1.0, 0.0, 1.0,
		1.0, 0.0, 1.0 ,
		1.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		0.2, 0.5, 0.5,
		0.2, 0.5, 0.5 ,
		0.2, 0.5, 0.5,
		0.2, 0.5, 0.5
	};




	// create a vertex buffer object
	CreateVertexObjects33(vaoID, vboID, vertices, colors, 24);


	// create a shader program
	program = cs557::CreateShaderProgram(vs_string_box_410, fs_string_box_410);



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
void cs557::Box::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix)
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
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

	// Unbind our Vertex Array Object
	glBindVertexArray(0);

	// Unbind the shader program
	glUseProgram(0);


}