/*
This code is part of CS/CPRE/ME 557 - Computer Graphics

This example demonstrates how one can set up multiple light sources with GLSL
per-fragment-light. The scene incorporates a plane and a teapot model 
along with three light sources. 

- The plane is a two-triangle plane to be found in ModelPlane.h/.cpp
- The teapot comes from a file teapot_t.obj, which is loaded in ModelOBJ.h/.cpp
- Shader code for per-fragment-light: lit_scene.vs/.fs

Light and material types are declared in CommonTypes.h.

Note that this example loads and creates one shader program per 3D model, 
although it is 3x the same model (feel free to change this).

Rafael Radkowski
Iowa State University
November 2018
rafael@iastate.edu

MIT License
*/


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
#include "CommonTypes.h"  // types that all projects use


// Geometry
#include "ModelCoordinateSystem.h"
#include "ModelPlane.h"
#include "ModelObj.h"
#include "ModelSphere.h"


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




//------------------------------------------------------------
//
//	Some model  to keep track of

// a corodinate system
cs557::CoordinateSystem coordinateSystem;
glm::mat4 modelMatrixCoord; 


// a plane
cs557::Plane plane0;
Material	 plane0mat;

// a teapot
cs557::OBJModel teapot0;
Material	 	teapot0mat;

// The light source
LightSource     light0;
LightSource     light1;
LightSource     light2;

// a light source.
// I render a sphere to indicate the location of the light source.
cs557::Sphere   sphere;
Material        spheremat;



// a helper plane to render the pre-render content
cs557::Plane display;
Material	 display_mat;
glm::mat4    display_p; // Store the projection matrix
glm::mat4    display_vm;       // Store the view matrix

//------------------------------------------------------------
//
// Set up our green background color
//GLfloat clear_color[] = {0.6f, 0.7f, 1.0f, 1.0f};
GLfloat clear_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};



// keyboard helpers
float kx = 0.0;
float ky = -3.0;
float kz = 3.0;
float delta = 0.5;

// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void my_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //cout << key << endl << flush;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 87 && action == GLFW_PRESS) // w
    {
        ky += delta;
        if(ky  > 5.0) ky = 5.0;
        else if(ky < -5.0) ky = -5.0;
    }
    else if(key == 83 && action == GLFW_PRESS)//s
    {
        ky -= delta;
        if(ky  > 5.0) ky = 5.0;
        else if(ky < -5.0) ky = -5.0;
    }
    else if(key == 82 && action == GLFW_PRESS)  // r
    {
        kz += delta;
        if(kz  > 5.0) kz = 5.0;
        else if(kz < 0.0) kz = 0.0;
    }
    else if(key == 70 && action == GLFW_PRESS) // f
    {
        kz -= delta;
        if(kz  > 5.0) kz = 5.0;
        else if(kz < 0.0) kz = 0.0;
    }
    else if(key == 65 && action == GLFW_PRESS) // a
    {
        kx += delta;
        if(kx  > 5.0) kx = 5.0;
        else if(kx < -5.0) kx = -5.0;

    }
    else if(key == 68 && action == GLFW_PRESS) // d
    {
        kx -= delta;
        if(kx  > 5.0) kx = 5.0;
        else if(kx < -5.0) kx = -5.0;

    }
     else if(key == 84 && action == GLFW_PRESS) // t
    {
    

    }

    // Update the position of the light. 
    light0.pos =  glm::vec3(kx, ky, kz);
    light0.setAllUniform(teapot0.getProgram());
    light0.setAllUniform(plane0.getProgram());
    light0.setAllUniform(sphere.getProgram());
}




 /*
Create light sources
Light sources on the host system are just parameters that get passed
to the shader programs to fed the light and reflection models. 
You can find the light source and material type declarations 
in CommonTypes.h.
Both types come with a method: setAllUniform(int program),
which passes all type variables as uniform variables to the program. 
*/
void CreateLights(void)
{
    //---------------------------------------------------------
    // Create a first light.
    light0.index = 0;
    light0.pos =  glm::vec3(0.0f, -3.0f, 3.0f);
    light0.dir =  glm::vec3(0.0f, 0.0f, 0.0f);
    light0.k1 = 0.1;

    //------------------------------------------------
    // Create a sphere to indicate the light

    // Load the shader program
    int program = cs557::LoadAndCreateShaderProgram("../lit_scene.vs", "../lit_scene.fs");
    sphere.create(0.1, 30, 40, program);

    // This aligns the light position with the sphere position.
    // Note that the light has not graphical representation.
    // Means, the model must be moved to the light locatoin manually.
    spheremat.model_matrix = glm::translate(glm::mat4(1.0f), light0.pos); 
    spheremat.diffuse_mat = glm::vec3(1.0, 1.0, 0.0);
    spheremat.diffuse_int = 1.5;
    spheremat.ambient_mat = glm::vec3(1.0, 1.0, 0.0);
    spheremat.ambient_int = 20.2;
    spheremat.specular_mat = glm::vec3(1.0, 1.0, 1.0);
    spheremat.specular_int = 0.6;
    spheremat.specular_s = 6.0;

    // This function passes all uniform varibles to the program
    spheremat.setAllUniform(sphere.getProgram());
    light0.setAllUniform(sphere.getProgram());

    glUseProgram(0);


    //---------------------------------------------------------
    // Create a second light. 
    light1.index = 1;
    light1.pos =  glm::vec3(0.0f, 3.0f, 3.0f);
    light1.dir =  glm::vec3(0.0f, 0.0f, 0.0f);
    light1.color = glm::vec3(0.0f, 0.0f, 1.0f);


    //---------------------------------------------------------
    // Create a third light.
    // It is a spotlight in this case. 
    light2.index = 2;
    light2.pos =  glm::vec3(3.0f, 0.0f, 4.0f);
    light2.dir =  glm::vec3(0.0f, 0.0f, 0.0f);
    light2.color = glm::vec3(1.0f, 1.0f, 1.0f);

    // inner and outer cutoff angle must be equal to realize a sharp edge.
    // cutoff_in <= cutoff_out, you invert the effect otherwise. 
    light2.cutoff_in = 0.1;
    light2.cutoff_out = 0.1;
    light2.intensity = 1.0;
    light2.type = SPOT;
}




/*
Creates a teapot model
*/
void CreateTeapotModel(void)
{
    // Define some teapot material parameters
    teapot0mat.diffuse_mat = glm::vec3(0.0, 0.4, 0.6);
    teapot0mat.diffuse_int = 1.5;
    teapot0mat.ambient_mat = glm::vec3(0.0, 0.4, 0.6);
    teapot0mat.ambient_int = 0.2;
    teapot0mat.specular_mat = glm::vec3(1.0, 1.0, 1.0);
    teapot0mat.specular_int = 0.6;
    teapot0mat.specular_s = 6.0;

	
    // Load the shader program
    int program = cs557::LoadAndCreateShaderProgram("../lit_scene.vs",  "../lit_scene.fs");

    // load a teapot from file 
    teapot0.create("../teapot_t.obj", program );
	teapot0mat.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 

    // Add light and material to the program. 
    // The function passes all uniform varibles to the program
    teapot0mat.setAllUniform(teapot0.getProgram());
    light0.setAllUniform(teapot0.getProgram());
    light1.setAllUniform(teapot0.getProgram());
    light2.setAllUniform(teapot0.getProgram());

	glUseProgram(0);

}

/*
Create the plan
*/
void CreatePlane(void)
{

    //------------------------------------------------
    // Create a plane

    plane0mat.diffuse_mat = glm::vec3(1.0, 0.4, 0.0);
    plane0mat.diffuse_int = 1.0;
    plane0mat.ambient_mat = glm::vec3(1.0, 0.4, 0.0);
    plane0mat.ambient_int = 0.2;
    plane0mat.specular_mat = glm::vec3(1.0, 1.0, 1.0);
    plane0mat.specular_int = 0.6;
    plane0mat.specular_s = 6.0;

    // Load the shader program
    int program = cs557::LoadAndCreateShaderProgram("../lit_scene.vs", "../lit_scene.fs");

    // create the geometry
    plane0.create(10.0, 10.0, program);
	plane0mat.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 

    // Add light and material to the program. 
    // The function passes all uniform varibles to the program
    plane0mat.setAllUniform(plane0.getProgram());
    light0.setAllUniform(plane0.getProgram());
    light1.setAllUniform(plane0.getProgram());
    light2.setAllUniform(plane0.getProgram());

	glUseProgram(0);

}


//------------------------------------------------------------
//
// CREATE THE SCENE
void CreateScene(void)
{

    // Projection transformations
	projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
	viewMatrix = glm::lookAt(glm::vec3(1.0f, -4.0f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    

    // create a coordinate system
    // That's a tiny helper to keep orientation
    coordinateSystem.create(2.5);
	modelMatrixCoord = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 


    // Create light sources
    CreateLights();

	// Create the prerenderer scene  with the object that casts the shadow
	// The content is rendered into a texture
	CreateTeapotModel();

	// Create the scene with all visible objects which receive the shadow. 
    CreatePlane();

}





//------------------------------------------------------------
//
//	INIT THE APPLICATION
void Init(void)
{

    // Init the GLFW Window
    window = cs557::initWindow();

    // set a keyboard callback
    glfwSetKeyCallback(window, my_key_callback);

    // Initialize the GLEW apis
    cs557::initGlew();

    // Create the scene
    CreateScene();

}

//------------------------------------------------------------
//
//	RENDER THE SCENE

void Draw(void)
{

    // Init the view matrix. 
    cs557::InitControlsViewMatrix(viewMatrix);
    
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
        glm::mat4 rotated_view =    cs557::GetCamera().getViewMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)) ;


		//-----------------------------------------------------------------------------
		//
		// Draw the scene

        // Object 0
        // This draws a coordinate system
        coordinateSystem.draw(projectionMatrix, rotated_view, modelMatrixCoord);

		 // draw the plane
        plane0.draw(projectionMatrix, rotated_view, plane0mat.model_matrix);

         // draw the teapot
		teapot0.draw(projectionMatrix, rotated_view, teapot0mat.model_matrix);

        // draw the sphere
        spheremat.model_matrix = glm::translate(glm::mat4(1.0f), light0.pos);  // need to be updated in case the light moves
        sphere.draw(projectionMatrix, rotated_view, spheremat.model_matrix);

        glUseProgram(0);

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
    cout << "This code example demonstrates the application of multiple lights." << endl;
    cout << "Rafael Radkowski\nrafael@iastate.edu\nIowa State University\n" << endl;
    cout << "Use the keys a, s, d, w, r, f to move one light source around.\n" << endl;
    cout << "This example is part of ME/CS/CPRE557" << endl;


	// Init the scene 
    Init();

	// Draw the scene
    Draw();

    return 1;
}
