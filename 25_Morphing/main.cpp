
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
#include "Texture2D.h"


// Geometry
#include "ModelCoordinateSystem.h"
#include "ModelPlane.h"
#include "ModelOBJ.h"
#include "ModelBSMorphing.h"

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
glm::mat4 modelMatrixPlane0;// Store the model matrix
glm::mat4 modelMatrixPlane1;// Store the model matrix
glm::mat4 modelMatrixCoord; //


// For projective texturing
glm::mat4 camera_projection;    // Store the projection matrix
glm::mat4 camera_viewmatrix;       // Store the view matrix
glm::mat4 camera_position;      // Store the model matrix


glm::vec3 camera_eye = glm::vec3(0.0f, 0.0f, -4.f);
glm::vec3 camera_center = glm::vec3(0.0f, 0.0f, 0.0);

int mode = 1;

//------------------------------------------------------------
//
//	Some model  to keep track of

// a corodinate system
cs557::CoordinateSystem coordinateSystem;





cs557::Plane plane0;
cs557::Plane plane1;


cs557::ModelBSMorphing dolphine1;
int shader_program = -1;

// Set up our green background color
GLfloat clear_color[] = {0.6f, 0.7f, 1.0f, 1.0f};
static const GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};

void update_projection( int mode, float x, float y, float z)
{
    switch(mode)
    {
        case 0:
            camera_eye.x += x;
            camera_eye.y += y;
            camera_eye.z += z;
        break;
        case 1:
            camera_center.x += x;
            camera_center.y += y;
            camera_center.z += z;
        break;
    }
}


// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void my_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    float delta = 0.1;
   // cout << key << endl; ;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 87 && action == GLFW_PRESS) // w
    {
        update_projection(mode, 0.0, delta, 0.0);
    }
    else if(key == 83 && action == GLFW_PRESS) // s
    {
        update_projection(mode, 0.0, -delta, 0.0);
    }
    else if(key == 65 && action == GLFW_PRESS) // a
    {
       update_projection(mode,  delta, 0.0, 0.0);  
    }
    else if(key == 68 && action == GLFW_PRESS) // d
    {
        update_projection(mode,  -delta, 0.0, 0.0);
    }
    else if(key == 82 && action == GLFW_PRESS) // r
    {
        update_projection(mode,   0.0, 0.0, delta);
    }
    else if(key == 70 && action == GLFW_PRESS) // f
    {
        update_projection(mode,   0.0, 0.0, -delta);
    }


    if(key == 81 && action == GLFW_RELEASE) // q
    {
        if (mode == 1) mode = 0;
        else mode = 1;
    }

}


//------------------------------------------------------------
//
//	INIT THE PROJECTIVE TEXTURE







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


    //-----------------------------------------------------------------------------------------------------------------------
	// Projection transformations
	projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
	modelMatrixPlane0 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
    modelMatrixPlane1 = glm::translate( glm::vec3(0.0f, 2.0f, 0.0f)) * glm::rotate( 1.57f, glm::vec3(1.0f, 0.0f, 0.0f));  
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -8.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrixCoord = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 


    // create a coordinate system
    coordinateSystem.create(2.5);

    // Load the shader program
    shader_program = cs557::LoadAndCreateShaderProgram("../morphing_program.vs", "../morphing_program.fs");


    dolphine1.create("../models/dphn1.obj", "../models/dphn2.obj", shader_program);


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

        //----------------------------------------------------------------------------------------------------------------------------
        // Object 0
        // This draws a coordinate system
        coordinateSystem.draw(projectionMatrix, rotated_view, modelMatrixCoord);

        glUseProgram(shader_program);
        camera_viewmatrix =  glm::lookAt(camera_eye, camera_center, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader_program, "camera_ViewMatrix"), 1, GL_FALSE, &camera_viewmatrix[0][0]); 
	
        // Get an animation time index
        float time = 0.5 * (sin( float( 60.0 * clock())/ float(CLOCKS_PER_SEC) )  + 1.0 );

       
        // draw the plane
        dolphine1.draw(projectionMatrix, rotated_view, modelMatrixPlane1 , time);

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
    cout << "This code example demonstrates projective texture mapping." << endl;
    cout << "Rafael Radkowski\nrafael@iastate.edu\nIowa State University" << endl;
    cout << "This example is part of ME/CS/CPRE557" << endl;



    Init();

    Draw();

    return 1;
}
