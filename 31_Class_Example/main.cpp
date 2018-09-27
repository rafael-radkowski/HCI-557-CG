
// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>			// transformation
#include <glm/gtx/quaternion.hpp>			// quaternions

// glfw includes
#include <GLFW/glfw3.h>

// local
#include "Window.h"					// the windows
#include "OpenGLDefaults.h"			// some open gl and glew defaults
#include "VertexBuffers.h"			// create vertex buffer object

#include "ShaderProgram.h"			// create a shader program


// Geometry
#include "ModelCoordinateSystem.h"
#include "ModelBox.h"

using namespace std;


//------------------------------------------------------------
// 
//	Some global variables to keep track of 

// The handle to the window object
GLFWwindow*         window = NULL;




// Memory to store the vertex array objects and buffer object ids

int vaoID[2]; // Our Vertex Array Object

int vboID[4]; // Our Vertex Buffer Object

int program;


// Transformation pipeline variables

glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix



//------------------------------------------------------------
// 
//	Some model  to keep track of 

// a corodinate system
cs557::CoordinateSystem coordinateSystem;


// some boxes
cs557::Box box0;
cs557::Box box1;
cs557::Box box2;
cs557::Box box3;

// transformations
glm::mat4 modelMatrixbox0; 
glm::mat4 modelMatrixbox1;
glm::mat4 modelMatrixbox2;
glm::mat4 modelMatrixbox3;


//------------------------------------------------------------
// 
//	INIT THE SCENE

void Init(void)
{

	// Init the GLFW Window
	window = cs557::initWindow();


	// Initialize the GLEW apis
	cs557::initGlew();



	float* vertices = new float[18];  // Vertices for our square
	float *colors = new float[18]; // Colors for our vertices

	vertices[0] = -0.5; vertices[1] = -0.5; vertices[2] = 0.5; // Bottom left corner
	colors[0] = 1.0; colors[1] = 1.0; colors[2] = 1.0; // Bottom left corner

	vertices[3] = -0.5; vertices[4] = 0.5; vertices[5] = 0.5; // Top left corner
	colors[3] = 1.0; colors[4] = 0.0; colors[5] = 0.0; // Top left corner

	vertices[6] = 0.5; vertices[7] = 0.5; vertices[8] = 0.5; // Top Right corner
	colors[6] = 0.0; colors[7] = 1.0; colors[8] = 0.0; // Top Right corner

	vertices[9] = 0.5; vertices[10] = -0.5; vertices[11] = 0.5; // Bottom right corner
	colors[9] = 0.0; colors[10] = 0.0; colors[11] = 1.0; // Bottom right corner

	vertices[12] = -0.5; vertices[13] = -0.5; vertices[14] = 0.5; // Bottom left corner
	colors[12] = 1.0; colors[13] = 1.0; colors[14] = 1.0; // Bottom left corner

	vertices[15] = 0.5; vertices[16] = 0.5; vertices[17] = 0.5; // Top Right corner
	colors[15] = 0.0; colors[16] = 1.0; colors[17] = 0.0; // Top Right corner


	// create a vertex buffer object
	cs557::CreateVertexObjects33(vaoID, vboID, vertices,colors, 6);


	// create a shader program
	program = cs557::CreateShaderProgram(cs557::vs_string_simple_shader_410, cs557::fs_string_simple_shader_410);



	//-----------------------------------------------------------------------------------------------------------------------
	// Projection transformations
	projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -2.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//-----------------------------------------------------------------------------------------------------------------------


	// Find the id's of the related variable name in your shader code. 
	int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
	int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
	int pos_location = glGetAttribLocation(program, "in_Position");
	int col_location = glGetAttribLocation(program, "in_Color");


	// Update all variables
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
	glBindAttribLocation(program, pos_location, "in_Position");
	glBindAttribLocation(program, col_location, "in_Color");



	//----------------------------------------------------------------------------------
	// create a coordinate system
	coordinateSystem.create(2.0);

	//----------------------------------------------------------------------------------
	// create models
	box0.create(0.4, 0.5, 0.6);
	box1.create(0.5, 0.4, 0.6);
	box2.create(0.6, 0.4, 0.5);
	box3.create(0.4, 0.4, 0.4);


}




//------------------------------------------------------------
// 
//	ReNDER THE SCENE

void Draw(void)
{

	// Set up our green background color
	static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
	static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };


	// Enable depth test
	glEnable(GL_DEPTH_TEST); // ignore this line

	while (!glfwWindowShouldClose(window))
	{

		// Clear the entire buffer with our green color (sets the background to be green).
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// This renders the objects

		// update the camera values. 
		// Note that this line changes the view matrix.
		glm::mat4 rotated_view = viewMatrix * cs557::GetTrackball().getRotationMatrix();


		//----------------------------------------------------------------------------------------------------------------------------
		// Object 0
		// This draws a coordinate system
		coordinateSystem.draw(projectionMatrix, rotated_view, modelMatrix);

		

		//----------------------------------------------------------------------------------------------------------------------------
		// Object 1
		glUseProgram(program);

		glUniformMatrix4fv( glGetUniformLocation(program, "viewMatrix") , 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shade

		glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object

		// This is a plane, you can render it by removing the comment (//).
		// But I just added the code to demostrate the difference between some structure in the code and a long listing. 
		//glDrawArrays(GL_TRIANGLES, 0, 6); // Draw our square

		glBindVertexArray(0); // Unbind our Vertex Array Object

		glUseProgram(0);


		//----------------------------------------------------------------------------------------------------------------------------
		// Object 2
		box0.draw(projectionMatrix, rotated_view, modelMatrix);


		//----------------------------------------------------------------------------------------------------------------------------
		// Object 3

		glm::mat4 translation_0 = glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotation_0 = glm::rotate( (float)glm::radians(45.0), glm::vec3(0.0f, 0.0f, 1.0f));

		modelMatrixbox1 = translation_0 * rotation_0;

		// rotated_view * modelMatrixbox1, here 'modelMatrixbox1' is multiplied from the right since 'rotated_view' is a view matrix. 
		// Along with the shader code, this line results in
		// gl_Position = projectionMatrix * viewMatrix * (rotated_view * modelMatrixbox1) * vec4(in_Position, 1.0); 
		box1.draw(projectionMatrix, rotated_view, modelMatrixbox1);


		//----------------------------------------------------------------------------------------------------------------------------
		// Object 4

		modelMatrixbox2 = rotation_0 *  translation_0;
		
	/*	glm::mat4 translation_2 = glm::translate(glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rotation_2_1 = glm::rotate((float)glm::radians(45.0), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rotation_2_2 = glm::rotate((float)glm::radians(90.0), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixbox2 = translation_2 * rotation_2_2 * rotation_2_1;*/

		box2.draw(projectionMatrix,  rotated_view, modelMatrixbox2);


		//----------------------------------------------------------------------------------------------------------------------------
		// Object 5
		float angle1 = 45.0;
		float angle2 = 90.0;
		glm::mat4 translation_5 = glm::translate(glm::vec3(0.0f, 0.0f, 1.0f));
		glm::quat quat_5_1( (float)cos(glm::radians(angle1 / 2.0f)) * 1.0f, 0, 0, (float)sin(glm::radians(angle1 / 2.0f)) * 1.0f );
		glm::quat quat_5_2((float)cos(glm::radians(angle2 / 2.0f)) * 1.0f, (float)sin(glm::radians(angle2 / 2.0f)) * 1.0f, 0,0);
		// or 
		// glm::quat(sqrt(1/2),0, 0, sqrt(1/2));
		
		modelMatrixbox3 = translation_5 *glm::toMat4(quat_5_2 * quat_5_1 );

		//box3.draw(projectionMatrix, rotated_view, modelMatrixbox3);

		//// This renders the objects
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Swap the buffers so that what we drew will appear on the screen.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}




int main(int argc, char * argv[])
{

	// Initialization
	Init();

	// Rendering loop
	Draw();

	return 1;
}