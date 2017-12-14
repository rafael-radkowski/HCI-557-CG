//
//  main.cpp
//  OpenGL4Test
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// locals 
#include "Shaders.h"
#include "LoadBitMapFile.h"
#include "LoadAndCreateTexture.h"
#include "LoadAndCreateObj.h"
#include "CreateRenderToTexture.h"

using namespace std;



//---------------------------------------------------------------------
// Parameters for the hidden scene
GLuint program_hidden;
unsigned int fboHidden;
GLuint color_texture_idx;
GLuint depth_texture_idx;



//---------------------------------------------------------------------
// Parameters for the visible scene
// Define some of the global variables we're using for this sample
GLuint program_visible;


//---------------------------------------------------------------------
//  vertex buffer objects
unsigned int vaoID[2]; // Our Vertex Array Object
unsigned int vboID[3]; // Our Vertex Buffer Object




glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix



// This is the callback we'll be registering with GLFW for errors.
// It'll just print out the error to the STDERR stream.
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}


// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}




void create_prerender_scene(void)
{
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<GLuint> elements;


	//-----------------------------------------------------------------------------------------
	// Load and start the shader program 
	program_hidden = LoadAndCreateShaderProgram("scene_hidden.vs", "scene_hidden.fs");
	GLint params;
	glGetProgramiv(program_hidden, GL_LINK_STATUS, &params);
	if (params == GL_FALSE)
	{
		cerr << "Program " << program_hidden << " has not been linked. " << endl;
	}

	// enable the program
	glUseProgram(program_hidden);


	int projectionMatrixLocation = glGetUniformLocation(program_hidden, "projectionMatrix"); // Get the location of our projection matrix in the shader
	int viewMatrixLocation = glGetUniformLocation(program_hidden, "viewMatrix"); // Get the location of our view matrix in the shader
	int modelMatrixLocation = glGetUniformLocation(program_hidden, "modelMatrix"); // Get the location of our model matrix in the shader

	viewMatrix = glm::lookAt(glm::vec3(0.0f, -3.0f, 4.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::rotate(glm::mat4(1.0f), 00.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader


	//-----------------------------------------------------------------------------------------
	// Load and init the object

	LoadAndCreateObj::Load("teapot.obj", vertices, normals, elements);


	glGenVertexArrays(1, &vaoID[1]); // Create our Vertex Array Object
	glBindVertexArray(vaoID[1]); // Bind our Vertex Array Object so we can use it

	glGenBuffers(2, &vboID[1]); // Generate our Vertex Buffer Object


	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat) * 3, &vertices[0][0], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW


	int p1_in = glGetAttribLocation(program_hidden, "in_Position");
	glVertexAttribPointer((GLuint)p1_in, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(p1_in); // Disable our Vertex Array Object


	// normals
	glBindBuffer(GL_ARRAY_BUFFER, vboID[2]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat) * 3, &normals[0][0], GL_STATIC_DRAW);

	int p2_in = glGetAttribLocation(program_hidden, "in_Normals");
	glVertexAttribPointer((GLuint)p2_in, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(p2_in); // Disable our Vertex Array Object

	glBindVertexArray(0);
	glUseProgram(0);


	// Create the frame buffer object for the hidden scene.
	CreateRenderToTexture(512, 512, fboHidden, color_texture_idx, depth_texture_idx);


	return;

}






// This creates the visible scene.
// A square, showing a texture. 
void create_visible_scene(void)
{

	//-----------------------------------------------------------------------------------------
	// Load and start the shader program 
	program_visible = LoadAndCreateShaderProgram("scene.vs", "scene.fs");
	GLint params;
	glGetProgramiv(program_visible, GL_LINK_STATUS, &params);
	if (params == GL_FALSE)
	{
		cerr << "Program " << program_visible << " has not been linked. " << endl;
	}

	// enable the program
	glUseProgram(program_visible);


	int projectionMatrixLocation = glGetUniformLocation(program_visible, "projectionMatrix"); // Get the location of our projection matrix in the shader
	int viewMatrixLocation = glGetUniformLocation(program_visible, "viewMatrix"); // Get the location of our view matrix in the shader
	int modelMatrixLocation = glGetUniformLocation(program_visible, "modelMatrix"); // Get the location of our model matrix in the shader

	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader


	//x, y, z, u, v, r, g, b
	float vertices[48] = {	-1.5f, -1.0f, 0.5f, 0.0, 0.0, 1.0, 0.8, 0.0,
							-1.5f, 1.0f, 0.5f, 0.0, 1.0, 1.0, 0.8, 0.0,
							1.5f, 1.0f, 0.5f, 1.0, 1.0, 1.0, 0.8, 0.0,
							1.5f, -1.0f, 0.5f, 1.0, 0.0, 1.0, 0.8, 0.0,
							-1.5f, -1.0f, 0.5f, 0.0, 0.0, 1.0, 0.8, 0.0,
							1.5f, 1.0f, 0.5f, 1.0, 1.0, 1.0, 0.8, 0.0};


    glGenVertexArrays(1, &vaoID[0]); // Create our Vertex Array Object
    glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    glGenBuffers(1, vboID); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
  
	int p1_in = glGetAttribLocation(program_visible, "in_Position");
    glVertexAttribPointer((GLuint)p1_in, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(p1_in); // Disable our Vertex Array Object

	int p3_in = glGetAttribLocation(program_visible, "in_TexCoord");
	glVertexAttribPointer((GLuint)p3_in, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid*)( 3 * sizeof(GLfloat))); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(p3_in); 
    
	int p2_in = glGetAttribLocation(program_visible, "in_Color");
    glVertexAttribPointer((GLuint)p2_in, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat))); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(p2_in); 



	// Load and create a texture from file

	GLuint texture_idx = LoadAndCreateTexture( "texture_sun_512_512.bmp");

	int p4_in = glGetUniformLocation(program_visible, "in_Texture");


    
    glBindVertexArray(0); 

}


void setupScene(void) {

	// create the prerenderr scene
	create_prerender_scene();

	// This creates the visible scene
	create_visible_scene();

}




int main(int argc, const char * argv[])
{
    // Initialize GLFW, and if it fails to initialize for any reason, print it out to STDERR.
    if (!glfwInit()) {
        fprintf(stderr, "Failed initialize GLFW.");
        exit(EXIT_FAILURE);
    }
    
    // Set the error callback, as mentioned above.
    glfwSetErrorCallback(error_callback);
    
    // Set up OpenGL options.
    // Use OpenGL verion 4.1,
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // GLFW_OPENGL_FORWARD_COMPAT specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in the requested version of OpenGL is removed.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Indicate we only want the newest core profile, rather than using backwards compatible and deprecated features.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Make the window resize-able.
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
	
    
    // Create a window to put our stuff in.
    GLFWwindow* window = glfwCreateWindow(1600, 1200, "Hello OpenGL", NULL, NULL);
    
    // If the window fails to be created, print out the error, clean up GLFW and exit the program.
    if(!window) {
        fprintf(stderr, "Failed to create GLFW window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // Use the window as the current context (everything that's drawn will be place in this window).
    glfwMakeContextCurrent(window);
    
    // Set the keyboard callback so that when we press ESC, it knows what to do.
    glfwSetKeyCallback(window, key_callback);
    
    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
    
    // Makes sure all extensions will be exposed in GLEW and initialize GLEW.
    glewExperimental = GL_TRUE;
    glewInit();
    
    // Shaders is the next part of our program. Notice that we use version 410 core. This has to match our version of OpenGL we are using, which is the core profile in version 4.1, thus 410 core.
    

    
    
    
 
    
    projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
   
	
    
     
    setupScene();
   

	// Set up our green background color
	static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
	static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 0.0f};
    
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
		
     
		//-----------------------------------------------------------------------------
		// Render the hidden object

		// Bind the frame buffer object
		glBindFramebuffer(GL_FRAMEBUFFER, fboHidden);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set the viewport. It must match the texture size.
		glViewport(0, 0, 512, 512);

		// Clear the entire buffer with our green color (sets the background to be green).
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);

		
		glUseProgram(program_hidden);

		glBindVertexArray(vaoID[1]);

		glDrawArrays(GL_TRIANGLES, 0, 3072);

		// Reset to the regular buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

        
		//-----------------------------------------------------------------------------
		// Render the visible object

		// reset the viewport to the previouse size
		glViewport(0, 0, 1600, 1200);

		// Clear the entire buffer with our green color (sets the background to be green).
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);

        glUseProgram(program_visible);

		// Bind the texture to the current active texture unit.
		// This will replace the texture which is currently tied.
		glBindTexture(GL_TEXTURE_2D, color_texture_idx);

        glBindVertexArray(vaoID[0]); 
        
        glDrawArrays(GL_TRIANGLES, 0, 6); 

		
		//-----------------------------------------------------------------------------
        
        glBindVertexArray(0); // Unbind our Vertex Array Object
        
        glUseProgram(0);
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Program clean up when the window gets closed.
    glDeleteVertexArrays(2, &vaoID[0]);
    glDeleteProgram(program_visible);
}

