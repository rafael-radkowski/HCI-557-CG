/*
This code is part of CS/CPRE/ME 557 - Computer Graphics

This example demonstrates the usage of transformation matrices and quaternions
to translate and rotate 3D models. 

The scene incorporates two 3D boxes. The user can transform the pose of one box
with prepared tranformations. Use the keys 1-6 to transform the object. 

- The box are part of the file ModelBox.h/.cpp. 
- The coordinate system is define in the file ModelCoordinateSystem.h/.cp
- The shader code is embedded in these files. 

Study the example to understand how transformations in OpenGL work. 

Rafael Radkowski
Iowa State University
November 2018
rafael@iastate.edu

MIT License
---------------------------------------------------------------------
*/


// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
//#define glm_gtx_quaternion
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

// glfw includes
#include <GLFW/glfw3.h>

// include local files
#include "Window.h" // the windows
#include "OpenGLDefaults.h" // some open gl and glew defaults
#include "VertexBuffers.h"  // create vertex buffer object
#include "ShaderProgram.h"  // create a shader program
#include "CommonTypes.h"  // types that all projects use

// Geometry
#include "ModelCoordinateSystem.h"
#include "ModelBox.h"


// namespace declaration to use the stl namespace. 
using namespace std;
using namespace cs557;

//-------------------------------------------------------------------
// Global variables
// The example uses several global variables.
// They are called global since they were defined outside any function or class. 
// They are accessible from anyhwere. 

// The handle to the GLFW window object. 
GLFWwindow*         window;

// The camera model declaration
// The matrix projectionMatrix stores the projection. 
// The matrix viewMatrix stores the camera's position and orientation. 
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix;       // Store the view matrix


// a global variable stores the key number the user pressed. 
int transform_idx = 1;

/*
This is the callback we'll be registering with GLFW for keyboard handling.
The callback is invoked when the user presses a button on the keyboard. 
Currently, it only reacts to the buttons 1-6. 

@param window - pointer to the window that invokes the callback. 
@param key - an ASCII index of the key pressed. 
@param action - a GLFW variable indicating the action. 
*/
void my_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //cout << key << endl << flush;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 49 && action == GLFW_PRESS) // 1
    {
        transform_idx = 1;
        cout << "Default position and orientation" << endl;
    }
    else if(key == 50 && action == GLFW_PRESS)//2
    {
        transform_idx = 2;
        cout << "Rotation first, translation second" << endl;
    }
    else if(key == 51 && action == GLFW_PRESS)  // 3
    {
        transform_idx = 3;
         cout << "Translation first, rotation second" << endl;
    }
    else if(key == 52 && action == GLFW_PRESS) // 4
    {
       transform_idx = 4;
       cout << "Rotation first, translation second, using a quaternion for the orientation." << endl;
    }
    else if(key == 53 && action == GLFW_PRESS) // 5
    {
      transform_idx = 5;

    }
    else if(key == 54 && action == GLFW_PRESS) // 6
    {
       transform_idx = 6;

    }
    else if(key == 84 && action == GLFW_PRESS) // t
    {
    

    }
}



/*
The main program. 
*/
int main(int argc, const char * argv[])
{
    
    
    ///////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    // This function hides the window initialization and 
    window = cs557::initWindow();
    
    // Register the keyboard callback with the window. 
    // Here, the window 'window' will call the function 
    // 'my_key_callback' when the user presses a button on the keyboard. 
    glfwSetKeyCallback(window, my_key_callback);
    
    // Init the glew api
    initGlew();
    

    /////////////////////////////////////////////////////////
    //// The next block creates all models and coordinate systems. 
    
    // The global world reference coordinate system. 
    // Calling its member create create the vertices, the vertex buffer object, and 
    // prepares all memory. 
    cs557::CoordinateSystem cs;
    cs.create(3.5);

    // A model matrix for the coordinate system. It will not move, etc. 
    glm::mat4 cs_matrix = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    
    // This example shows two 3D boxes at different locations
    // The first box 'box_original' is at its local origin. It acts as a visiual reference
    // The second box 'box_transformed' shows the box after a transformation was applied. 
    
    // Create the original box.
    // It is part of the class Box. Calling the function create creates the box. 
    Box box_original;
    box_original.create(0.5,1.0,0.5);

    // The box also gets an coordinate system. 
    cs557::CoordinateSystem cs_box_original;
    cs_box_original.create(1.5);
    
    // This is the second 3D box. We use the same class but create a new instance
    // of the class Box, named 'box_transformed'.
    Box box_transformed;
    box_transformed.create(0.5,1.0,0.5);

    // The box also gets an coordinate system. 
    cs557::CoordinateSystem cs_box_transformed;
    cs_box_transformed.create(1.5);



    /////////////////////////////////////////////////////////
    //// This section creates all transformations. 
    // Note that the variables are only defined and declared in this section.
    // They are used within the rendering loop. 

    // some model matrix variables to store different results. 
    glm::mat4 model_matrix1 = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

    // This defines two matrices, one for translation and one for rotations
    // The two functions glm::translate and glm::rotate are part of the glm::gtx namespace
    glm::mat4 translation = glm::translate(glm::vec3(2.0f, 0.0f, 0.0f));
    glm::mat4 rotation = glm::rotate( 1.57f/2.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    // The first quaternions. 
    // RotationAngle is in radians
    glm::vec3 rotation_axis1(0.0, 0.0, 1.0);
    float rotation_angle1 = 0.785f; // 45 degree
    float x1 = rotation_axis1.x * sin(rotation_angle1 / 2);
    float y1 = rotation_axis1.y * sin(rotation_angle1 / 2);
    float z1 = rotation_axis1.z * sin(rotation_angle1 / 2);
    float w1 = cos(rotation_angle1 / 2);
    glm::quat q1 = glm::quat(w1, x1, y1, z1);

    // The second quaternions. 
    glm::vec3 rotation_axis2(0.0, 1.0, 0.0);
    float rotation_angle2 = 1.57f; // 90 degree
    float x2 = rotation_axis2.x * sin(rotation_angle2 / 2);
    float y2 = rotation_axis2.y * sin(rotation_angle2 / 2);
    float z2 = rotation_axis2.z * sin(rotation_angle2 / 2);
    float w2 = cos(rotation_angle2 / 2);
    glm::quat q2 = glm::quat(w2, x2, y2, z2);

    // The third quaternions. 
    glm::vec3 rotation_axis3(0.707, 0.707, 0.0); // NOTE THAT THE AXIS IS OF LENGTH 1.0
    float rotation_angle3 = 1.57f; // 90 degree
    float x3 = rotation_axis3.x * sin(rotation_angle3 / 2);
    float y3 = rotation_axis3.y * sin(rotation_angle3 / 2);
    float z3 = rotation_axis3.z * sin(rotation_angle3 / 2);
    float w3 = cos(rotation_angle3 / 2);
    glm::quat q3 = glm::quat(w3, x3, y3, z3);

    // Two additional translations. 
     // The two functions glm::translate and glm::rotate are part of the glm::gtx namespace
    glm::mat4 translation_q1 = glm::translate(glm::vec3(-2.0f, 0.0f, 0.0f));
    glm::mat4 translation_q3 = glm::translate(glm::vec3(1.0f, 1.0f, 0.0f));

    
    
    ///////////////////////////////////////////////////////////
    //// This section creates a view matrix and a projection matrix. 
    // Both are part of the camera model. 
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    
    /*
     Init the view matrix. 
     The function glm::lookAt is part of glm::matrix_transform
     The view matrix is defined by its eye-point, its center-point, and an up vector. 
     */
    viewMatrix = glm::lookAt(glm::vec3(1.0f, -2.0f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    cs557::InitControlsViewMatrix(viewMatrix); // This inits the virtual trackball. 


    /*
     Init the projection matrix
     The function glm::perspective is part of glm::matrix_transform
     It takes the y-field-of-veiw, the aspect ratio, and the near- and far-clipping plane distance
     as input parameters. 
    */
	projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);

    // Note that both variables viewMatrix and projectionMatrix are defined in the global application namespace. 


    ///////////////////////////////////////////////////////////
    //// Main render loop
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Branching with if-statements
        // the if-statement branches the code depending on the user's input. 
        // The key the user hit is stored in the variable transform_idx.

        glm::mat4 box_model_matrix = glm::mat4();
        if(transform_idx == 1){
            // the default model_matrix. 
            box_model_matrix = model_matrix1;
        }else if(transform_idx == 2){
            // rotation first, translation second
            box_model_matrix = translation *  rotation;
        }else if(transform_idx == 3){
            // translation first, rotation second
            box_model_matrix = rotation * translation;
        }else if(transform_idx == 4){
            // rotation first using a quaternion, translation second. 
            box_model_matrix =  translation_q1 * glm::toMat4(q1);
        }else if(transform_idx == 5){
            // two concatenated rotations
            box_model_matrix =  translation_q1 * glm::toMat4(q2) * glm::toMat4(q1);
        }else if(transform_idx == 6){
            // rotation around an arbitrary axis. 
            glm::quat q3n = glm::normalize(q3);
            box_model_matrix = translation_q3 * glm::toMat4(q3n);
        }

         ///////////////////////////////////////////////////////
        //// Rendering  the objects

               
        // Set the trackball locatiom
        glm::mat4 rotated_view =    cs557::GetCamera().getViewMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)) ;
        
        // draw the coordinate frame
        cs.draw(projectionMatrix, rotated_view, cs_matrix);
        

        // Render the original box and its coordinate system
        box_original.draw(projectionMatrix, rotated_view, model_matrix1);
        cs_box_original.draw(projectionMatrix, rotated_view, model_matrix1);

        box_transformed.draw(projectionMatrix, rotated_view, box_model_matrix);
        cs_box_transformed.draw(projectionMatrix, rotated_view, box_model_matrix);

        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    


}

