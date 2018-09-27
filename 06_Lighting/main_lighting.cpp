
// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions

// glfw includes
#include <GLFW/glfw3.h>

// local
#include "Window.h" // the windows
#include "OpenGLDefaults.h" // some open gl and glew defaults
#include "VertexBuffers.h"  // create vertex buffer object
#include "ShaderProgram.h"  // create a shader program

// Geometry
#include "ModelCoordinateSystem.h"
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
glm::mat4 modelMatrix;      // Store the model matrix



//------------------------------------------------------------
//
//	Some model  to keep track of

// a corodinate system
cs557::CoordinateSystem coordinateSystem;


// a sphere
cs557::Sphere sphere;
int per_vertex_light_program = -1;

int         light_shader_loc = -1;
glm::vec3   light_position;

float       diffuse_intensity = 0.2;
float       ambient_intensity = 0.5;
float       specular_intensity = 0.9;
float       shininess = 10.5;

int mode = 0;


// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void my_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 84 && action == GLFW_PRESS) // t
    {
        mode = mode + 1;
        if(mode >=4)mode = 0;

        switch(mode)
        {
            case 0:
            cout << "Diffuse intensity" << endl;
            break;
            case 1:
            cout << "Specular intensity" << endl;
            break;
            case 2:
            cout << "Ambient intensity" << endl;
            break;
            case 3:
            cout << "shininess" << endl;
        }
    }
    else if(key == 46 && action == GLFW_PRESS)
    {
         switch(mode)
        {
            case 0:
            diffuse_intensity += 0.05;
            if(diffuse_intensity > 10)diffuse_intensity = 10;
            break;
            case 1:
            specular_intensity += 0.05;
            if(specular_intensity > 10)specular_intensity = 10;
            break;
            case 2:
             ambient_intensity += 0.05;
            if(ambient_intensity > 10)ambient_intensity = 10;
            break;
            case 3:
            shininess += 0.5;
            if(shininess > 150)shininess = 10;
            break;

            
        }
    }
    else if(key == 44 && action == GLFW_PRESS)
    {
         switch(mode)
        {
            case 0:
            diffuse_intensity -= 0.05;
            if(diffuse_intensity < 0 )diffuse_intensity = 0;
            break;
            case 1:
            specular_intensity -= 0.05;
            if(specular_intensity < 0)specular_intensity = 0;
            break;
            case 2:
            ambient_intensity-= 0.05;
            if(ambient_intensity < 0)ambient_intensity = 0;
            break;
            case 3:
            shininess -= 0.5;
            if(shininess < 1) shininess = 1;
        }
    }

    if(action == GLFW_PRESS)
        cout << "Parameters: diff: " << diffuse_intensity << " amb: " << ambient_intensity << " spec: " << specular_intensity << " shi: " << shininess << endl;
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


    //-----------------------------------------------------------------------------------------------------------------------
	// Projection transformations
	projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -4.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));



    // create a coordinate system
    coordinateSystem.create(2.5);



    //-----------------------------------------------------------------------------------------------------------------------
	// Load a shader program from a file
    // This function is part of the file ShaderProgram.cpp/.h
    per_vertex_light_program = LoadAndCreateShaderProgram("../per_vertex_light.vs", "../per_vertex_light.fs");

    glUseProgram(per_vertex_light_program);

    // create a sphere
    sphere.create(1.0, 40, 40, per_vertex_light_program);
    
    glm::vec3 diff_color(1.0, 1.0, 0.0);
    glUniform3fv(glGetUniformLocation(per_vertex_light_program, "diffusecolor"), 1, &diff_color[0]);

    glm::vec3 spec_color(1.0, 1.0, 1.0);
    glUniform3fv(glGetUniformLocation(per_vertex_light_program, "specularColor"), 1, &spec_color[0]);

    glm::vec3 ambi_color(1.0, 1.0, 0.0);
    glUniform3fv(glGetUniformLocation(per_vertex_light_program, "ambientcolor"), 1, &ambi_color[0]);

    glm::vec3 light_position(0.0, 4.0, 1.0);
    glUniform3fv(glGetUniformLocation(per_vertex_light_program, "locationLight"), 1, &light_position[0]);

    diffuse_intensity = 0.4f;
    glUniform1f(glGetUniformLocation(per_vertex_light_program, "diffuseIntensity"), diffuse_intensity);

    ambient_intensity = 0.5f;
    glUniform1f(glGetUniformLocation(per_vertex_light_program, "ambientIntensity"), ambient_intensity);

    specular_intensity = 0.5f;
    glUniform1f(glGetUniformLocation(per_vertex_light_program, "specular_intensity"), specular_intensity);
    
    shininess = 10.5f;
    glUniform1f(glGetUniformLocation(per_vertex_light_program, "speculuar_coeff"), shininess);


}

//------------------------------------------------------------
//
//	RENDER THE SCENE

void Draw(void)
{

    // Set up our green background color
    static const GLfloat clear_color[] = {0.6f, 0.7f, 1.0f, 1.0f};
    static const GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};

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

        // update values
        glUseProgram(per_vertex_light_program);
        glUniform1f(glGetUniformLocation(per_vertex_light_program, "diffuseIntensity"), diffuse_intensity);
        glUniform1f(glGetUniformLocation(per_vertex_light_program, "ambientIntensity"), ambient_intensity);
        glUniform1f(glGetUniformLocation(per_vertex_light_program, "specular_intensity"), specular_intensity);
         glUniform1f(glGetUniformLocation(per_vertex_light_program, "speculuar_coeff"), shininess);

        // draw the sphere
        sphere.draw(projectionMatrix, rotated_view, modelMatrix);

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
    cout << "This code example demonstrates a light source." << endl;
    cout << "Rafael Radkowski\nrafael@iastate.edu\nIowa State University" << endl;
    cout << "This example is part of ME/CS/CPRE557" << endl;

    cout << "\nUse the following keys to change the light" <<endl;
    cout << "\t't' - toggles between diffuce, specular, ambient, and shininess parameters " << endl;
    cout << "\t'.' - increase the value" << endl;
    cout << "\t',' - decrease the value" << endl;

    Init();

    Draw();

    return 1;
}
