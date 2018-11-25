
// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions

// glfw includes
#include <GLFW/glfw3.h>

// local
#define GLM_ENABLE_EXPERIMENTAL
#include "Window.h" // the windows
#include "OpenGLDefaults.h" // some open gl and glew defaults
#include "VertexBuffers.h"  // create vertex buffer object
#include "ShaderProgram.h"  // create a shader program
#include "Texture2D.h"      // for regular 2D texturing
#include "RenderToTexture.h" // for the prerender texture

// Geometry
#include "ModelCoordinateSystem.h"
#include "ModelPlane.h"
#include "ModelObj.h"


using namespace std;
using namespace cs557;
//------------------------------------------------------------
//
//	Some global variables to keep track of

// The handle to the window object
GLFWwindow *window = NULL;

// Transformation pipeline variables
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix;       // Store the view matrix





//---------------------------------------------------------------------
// Parameters for the hidden scene
GLuint program_hidden = -1;
unsigned int fboHidden = -1;
GLuint color_texture_idx = -1;
GLuint depth_texture_idx = -1;

// Transformation pipeline variables for the prerenderer scene
glm::mat4 projectionMatrixPrerenderer; // Store the projection matrix
glm::mat4 viewMatrixPrerenderer;       // Store the view matrix



/*
This struct is a helper which contains additonial model data.
*/
typedef struct _ModelData
{
	int shader_program_id;
	unsigned int texture_id;
	glm::mat4 model_matrix;

	_ModelData(){
		shader_program_id = -1;
		texture_id = -1;
		model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 
	}

} ModelData;



//------------------------------------------------------------
//
//	Some model  to keep track of

// a corodinate system
cs557::CoordinateSystem coordinateSystem;
glm::mat4 modelMatrixCoord; 


// a plane
cs557::Plane plane0;
ModelData	 plane0d;

// a teapot
cs557::OBJModel teapot0;
ModelData	 	teapot0d;



//------------------------------------------------------------
//
// Set up our green background color
GLfloat clear_color[] = {0.6f, 0.7f, 1.0f, 1.0f};
GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};




// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void my_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   //cout << key ;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 84 && action == GLFW_PRESS) // t
    {
        
    }
    else if(key == 46 && action == GLFW_PRESS)
    {
        
    }
    else if(key == 44 && action == GLFW_PRESS)
    {
       
        
    }
    else if(key == 66 && action == GLFW_PRESS)
    {
    

    }
}


/*
This function creates the scene with all invisble objects. 
The are rendered into a texture which can be used as 2d map in the visible scene. 
*/
void CreatePrerendererScene(void)
{

	// Projection transformations
	projectionMatrixPrerenderer = glm::perspective(1.57f, (float)400 / (float)400, 0.1f, 100.f);
	viewMatrixPrerenderer = glm::lookAt(glm::vec3(0.0f, -2.0f, 2.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Load the shader program
    teapot0d.shader_program_id = cs557::LoadAndCreateShaderProgram("../scene_hidden.vs", "../scene_hidden.fs");

	 // load a teapot from file 
    teapot0.create("../teapot_t.obj", teapot0d.shader_program_id );
	teapot0d.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 



	// This function is part of RenderToTexture.h
	// It creates a texture as target for a pre-render step. 
	// The texture is linked to a framebuffer object (fbo), memory to store the image into.
	// This fbo is linked to a texture, so that we can use it in the  scene.
	CreateRenderToTexture(512, 512, fboHidden, color_texture_idx, depth_texture_idx);

	// Reset to the regular buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(0);

}

/*
This function creates the scene with all visble objects, those, which are part of this scene.
*/
void CreateVisibleScene(void)
{

	// Projection transformations
	projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -4.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    


    // create a coordinate system
    coordinateSystem.create(2.5);
	modelMatrixCoord = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 


    // Load the shader program
    plane0d.shader_program_id = cs557::LoadAndCreateShaderProgram("../scene.vs", "../scene.fs");

    // create a plane
    plane0.create(4.0, 4.0, plane0d.shader_program_id);
	plane0d.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 

    // Load the texture and create it.
    // Note that this function bind the texture to texture unit GL_TEXTURE0 
    glUseProgram(plane0d.shader_program_id);
    
	// This is just a test-texture. It has no purpose in the scene other than to test whether 
	// the texture functionality works properly. 
    cs557::LoadAndCreateTexture2D("../texture_sun_512_512.bmp", &plane0d.texture_id);

	plane0d.texture_id = color_texture_idx;

    // Activate the texture unit and bind the texture. 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, plane0d.texture_id);

    // Fetch the texture location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    int texture_location = glGetUniformLocation(plane0d.shader_program_id, "tex");
    glUniform1i(texture_location, 0);


	glUseProgram(0);

}





//------------------------------------------------------------
//
//	INIT THE SCENE

void Init(void)
{

    // Init the GLFW Window
    window = cs557::initWindow();

    // set a keyboard callback
    glfwSetKeyCallback(window, my_key_callback);

    // Initialize the GLEW apis
    cs557::initGlew();


	// Create the prerenderer scene. 
	// The content is rendered into a texture
	CreatePrerendererScene();

	// Create the scene with all visible objects
    CreateVisibleScene();

}

//------------------------------------------------------------
//
//	RENDER THE SCENE

void Draw(void)
{

    

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



		//-----------------------------------------------------------------------------
		// 
		// Draw the hidden scene

		// Bind the frame buffer object
		glBindFramebuffer(GL_FRAMEBUFFER, fboHidden);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set the viewport. It must match the texture size.
		glViewport(0, 0, 512, 512);

		// Clear the entire buffer with our green color.
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);

		// activate the shader program
		glUseProgram(teapot0d.shader_program_id);

		double t = glfwGetTime();
		teapot0d.model_matrix = glm::rotate(  (float)2.0 * (float) sin((double)t), glm::vec3(0.0f, 0.0f, 1.0f)); 


		// draw the teapot
		teapot0.draw(projectionMatrixPrerenderer, viewMatrixPrerenderer, teapot0d.model_matrix);

		// Reset to the regular buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//-----------------------------------------------------------------------------
		//
		// Draw the visible scene
        // The plane



		// reset the viewport to the previouse size
		// Note that the opperating system zoom factor may interfer with this setting. 
		// Check your system scale and set it accordingly 
		int OSZoom = 2.0;
		glViewport(0, 0, OSZoom * 1280, OSZoom *1024); // the defaut window is set to this size


        // Object 0
        // This draws a coordinate system
        coordinateSystem.draw(projectionMatrix, rotated_view, modelMatrixCoord);


		// activate and bind the texture
		glUseProgram(plane0d.shader_program_id);
		glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, plane0d.texture_id);

        // draw the plane
        plane0.draw(projectionMatrix, rotated_view, plane0d.model_matrix);


        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}








//------------------------------------------------------------
//
//	RENDER THE SCENE
int main(int argc, const char *argv[])
{
    cout << "This code example demonstrates a multipass renderer." << endl;
    cout << "Rafael Radkowski\nrafael@iastate.edu\nIowa State University" << endl;
    cout << "This example is part of ME/CS/CPRE557" << endl;


	// Init the scene 
    Init();

	// Draw the scene
    Draw();

    return 1;
}
