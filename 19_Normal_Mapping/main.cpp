
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
#include "ModelOBJ.h"

// Geometry
#include "ModelCoordinateSystem.h"
#include "ModelPlane.h"

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
glm::mat4 modelMatrixTeapot;// Store the model matrix
glm::mat4 modelMatrixCoord; //


int mode = 0;

//------------------------------------------------------------
//
//	Some model  to keep track of

// a corodinate system
cs557::CoordinateSystem coordinateSystem;


unsigned int texture_program_plane = -1;
unsigned int texture_program_teapot = -1;

cs557::Plane        plane0;
cs557::OBJModel     obj0;


unsigned int texture_id_plane;
unsigned int texture_id_teapot;
unsigned int texture_id_plane_norm;
unsigned int texture_id_teapot_norm;


int use_normalmap = 1;
int use_normalmap_location;

// Set up our green background color
//GLfloat clear_color[] = {0.6f, 0.7f, 1.0f, 1.0f};
GLfloat clear_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
static const GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};




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
        mode +=1;
        if (mode > 5) mode = 0;

        glUseProgram(texture_program_teapot);
        glUniform1i(glGetUniformLocation(texture_program_teapot, "mode"), mode);
        glUseProgram(texture_program_plane);
        glUniform1i(glGetUniformLocation(texture_program_plane, "mode"), mode);
        glUseProgram(0);
    }
    else if(key == 83 && action == GLFW_PRESS) // s
    {
       
    }
    else if(key == 65 && action == GLFW_PRESS) // a
    {
    
    }
    else if(key == 68 && action == GLFW_PRESS) // d
    {
     
    }
    else if(key == 82 && action == GLFW_PRESS) // r
    {
        mode =0 ;
        

        glUseProgram(texture_program_teapot);
        glUniform1i(glGetUniformLocation(texture_program_teapot, "mode"), mode);
        glUseProgram(texture_program_plane);
        glUniform1i(glGetUniformLocation(texture_program_plane, "mode"), mode);
        glUseProgram(0);
     
    }
    else if(key == 70 && action == GLFW_PRESS) // f
    {
       
    }


    if(key == 81 && action == GLFW_RELEASE) // q
    {
        if (use_normalmap == 1) use_normalmap = 0;
        else use_normalmap = 1;

        glUseProgram(texture_program_teapot);
        glUniform1i(use_normalmap_location, use_normalmap);
        glUseProgram(texture_program_plane);
        glUniform1i(glGetUniformLocation(texture_program_plane, "use_normalmap"), use_normalmap);
        glUseProgram(0);
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
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrixCoord = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 


    // create a coordinate system
    coordinateSystem.create(2.5);


    //-----------------------------------------------------------------------
    // The ground plane

    // Load the shader program
    texture_program_plane = cs557::LoadAndCreateShaderProgram("../normal_map_program.vs", "../normal_map_program.fs");
    glUseProgram(texture_program_plane);

    // create the planes
    plane0.create(8.0, 8.0, texture_program_plane);


    // Load the texture and create it.
    // Note that this function bind the texture to texture unit GL_TEXTURE0 
    LoadAndCreateTexture2D("../textures/water-texture.bmp", &texture_id_plane, GL_REPEAT, GL_TEXTURE0);

    // Activate the texture unit and bind the texture. 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_plane);

    // Fetch the texture location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    int texture_location = glGetUniformLocation(texture_program_plane, "tex");
    glUniform1i(texture_location, 0);


      // Load the normal map texture and create it.
    // Note that this function bind the texture to texture unit GL_TEXTURE0 
    glActiveTexture(GL_TEXTURE2);
    LoadAndCreateTexture2D("../textures/water-texture_NRM.bmp", &texture_id_plane_norm, GL_REPEAT, GL_TEXTURE2);

     // Activate the texture unit and bind the texture. 
    glUseProgram(texture_program_plane);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_id_plane_norm);

    // Fetch the texture location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    texture_location = glGetUniformLocation(texture_program_plane, "tex_normalmap");
    glUniform1i(texture_location, 2);

    
    glUniform1i(glGetUniformLocation(texture_program_plane, "use_normalmap"), use_normalmap);
    glUniform1i(glGetUniformLocation(texture_program_plane, "mode"), mode);

    


    //-----------------------------------------------------------------------
    // The teapot

     // Load the shader program
    texture_program_teapot = cs557::LoadAndCreateShaderProgram("../normal_map_program.vs", "../normal_map_program.fs");
    glUseProgram(texture_program_teapot);

    // Create the teapot
    obj0.create("../teapot_t.obj", texture_program_teapot);

    // Load the texture and create it.
    // Note that this function bind the texture to texture unit GL_TEXTURE0 
    LoadAndCreateTexture2D("../textures/texture_cracked_clay_2048x2048.bmp", &texture_id_teapot, GL_REPEAT, GL_TEXTURE0);

    // Activate the texture unit and bind the texture. 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_teapot);

    // Fetch the texture location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    texture_location = glGetUniformLocation(texture_program_teapot, "tex");
    glUniform1i(texture_location, 0);


     // Load the normal map texture and create it.
    // Note that this function bind the texture to texture unit GL_TEXTURE0 
    LoadAndCreateTexture2D("../textures/texture_cracked_clay_2048x2048_NRM.bmp", &texture_id_teapot_norm, GL_REPEAT, GL_TEXTURE1);

     // Activate the texture unit and bind the texture. 
    glUseProgram(texture_program_teapot);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_id_teapot_norm);

    // Fetch the texture location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    texture_location = glGetUniformLocation(texture_program_teapot, "tex_normalmap");
    glUniform1i(texture_location, 1);


    use_normalmap_location = glGetUniformLocation(texture_program_teapot, "use_normalmap");
    glUniform1i(use_normalmap_location, use_normalmap);
    glUniform1i(glGetUniformLocation(texture_program_teapot, "mode"), mode);

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


        //----------------------------------------------------------------------------------------------------------------------------
        // Object 1
        
        // Use the texture
        glUseProgram(texture_program_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_plane);

        // draw the plane
        plane0.draw(projectionMatrix, rotated_view, modelMatrixPlane0);


        //----------------------------------------------------------------------------------------------------------------------------
        // Object 2

       // Use the texture
        glUseProgram(texture_program_teapot);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_teapot);

        // draw the teapot
        obj0.draw(projectionMatrix, rotated_view, modelMatrixPlane0);

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
